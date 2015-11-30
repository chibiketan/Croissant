/*
 * String.cpp
 *
 *  Created on: 10 juil. 2013
 *      Author: gcompte
 */

#include <iostream>




#include "Core/String.hpp"
#include <string.h>

#if defined(__linux)
#  include <endian.h>
#else
#  define  __LITTLE_ENDIAN 1234
#  define  __BIG_ENDIAN 4321
#  define  __BYTE_ORDER __LITTLE_ENDIAN
#endif

#include "Exception/NotImplementedException.hpp"

#define MASK_ASCII(ch) ((ch & 0b10000000) == 0)
#define MASK_2CHAR(ch) ((ch & 0b11100000) == 0b11000000)
#define MASK_3CHAR(ch) ((ch & 0b11110000) == 0b11100000)
#define MASK_4CHAR(ch) ((ch & 0b11111000) == 0b11110000)

using ConstIterator = Croissant::Core::String::ConstIterator;
using ConstReverseIterator = Croissant::Core::String::ConstReverseIterator;

namespace Croissant
{
	namespace Core
	{
		template<>
		class ENGINE_API Pimpl<String>::Implementation
		{
		public:
			Implementation(String* ref);
			Implementation(String* ref, const char* str, EncodingType = EncodingType::LATIN1);
			Implementation(String* ref, const ConstIterator& begin, const ConstIterator& end);
			Implementation(String* ref, const Implementation& src); // copie
			Implementation(String* ref, Implementation&& src); // move
			~Implementation();

			int Compare(const Implementation& str) const;
			const char* Str() const;
			size_t Length() const;
			size_t ByteLength() const;
			size_t ReservedLength() const;
			void Reserve(size_t size);
			void Append(size_t repeat, char c);
			void Append(const Implementation& str);
			void Append(const ConstIterator& first, const ConstIterator& end);
			void Replace(const Implementation& search, const Implementation& replace);
			ConstIterator Begin() const;
			ConstIterator End() const;
			ConstReverseIterator RBegin() const;
			ConstReverseIterator REnd() const;

			Implementation& operator=(const Implementation& str);
			uint32_t operator[](const uint32_t index) const;

		private:

			uint8_t* m_str;
			size_t m_charCount;
			size_t m_byteCount;
			size_t m_reservedLength;
			String* m_ref;
		};

		using StringImpl = Pimpl<String>::Implementation;


		namespace
		{
			void ConvertNotImplemented(const char* src, uint8_t*& dst, size_t& charNum, size_t& byteNum, size_t& reservedLength)
			{
				throw Croissant::Exception::NotImplementedException();
			}

			// ANSI string contain only US-ASCII char (0 to 127 as value)
			void ConvertASCIIToUTF8(const char* src, uint8_t*& dst, size_t& charNum, size_t& byteNum, size_t& reservedLength)
			{
				charNum = strlen(src);
				byteNum = charNum + 1; // ne pas oublier le '\0'...
				reservedLength = byteNum;
				dst = CROISSANT_NEW uint8_t[byteNum];
				memcpy(dst, src, byteNum);
			}

			void ConvertLatin1ToUTF8(const char* src, uint8_t*& dst, size_t& charNum, size_t& byteNum, size_t& reservedLength)
			{
				auto tmpCharCount = strlen(src);
				auto tmpSrc = src;

				charNum = 0;
				byteNum = 0;
				dst = CROISSANT_NEW uint8_t[2 * (tmpCharCount + 1 + sizeof(uint8_t))];
				for (;tmpCharCount > 0;--tmpCharCount)
				{
					auto c = static_cast<uint8_t>(*tmpSrc);

					++tmpSrc;
					if (c <= 127)
					{
						// ASCII char
						dst[byteNum++] = c;
					}
					else
					{
						// 5 bits pour le premier
						dst[byteNum++] = 0b11000000 | ((0b11000000 & c) >> 6);
						// 6 bits pour le second
						dst[byteNum++] = 0b10000000 | (0b00111111 & c);
					}

					++charNum;
				}

				dst[byteNum++] = '\0';
				reservedLength = byteNum;
			}


			typedef void (*convertFunc)(const char* src, uint8_t*& dst, size_t& charNum, size_t& byteNum, size_t& reservedLength);

			const convertFunc conversionTab[(uint32_t)EncodingType::ENCODING_TYPE_SIZE] = {
			                                                                     ConvertASCIIToUTF8, // ASCII
			                                                                     ConvertLatin1ToUTF8, // LATIN1
			                                                                     ConvertNotImplemented, // UTF8
			                                                                     ConvertNotImplemented, // UTF16
			                                                                     ConvertNotImplemented, // UTF32
			                                                                     ConvertNotImplemented // DETECT
			};

			struct LittleEndian {};
			struct BigEndian {};

#if __BYTE_ORDER == __LITTLE_ENDIAN
			using Endianness = LittleEndian;
#else
			using Endianness = BigEndian;
#endif

			template<uint32_t Size, class Endian = Endianness>
			inline typename std::enable_if<std::is_same<BigEndian, Endian>::value, uint32_t>::type GetChar(uint8_t* ptr)
			{
				uint32_t result = 0;
				uint8_t* resultPtr = reinterpret_cast<uint8_t*>(&result) + (4 - Size);

				memcpy(resultPtr, ptr, Size);
				return result;
			}

			template<uint32_t Size, class Endian = Endianness>
			inline typename std::enable_if<std::is_same<LittleEndian, Endian>::value, uint32_t>::type GetChar(uint8_t* ptr)
			{
				uint32_t result = 0;
				uint8_t* resultPtr = reinterpret_cast<uint8_t*>(&result) + (Size - 1);

				for (int i = 0; i < Size; ++i)
				{
					*resultPtr = *ptr;
					++ptr;
					--resultPtr;
				}

				return result;
			}
		
		}


		// ----------------------------------------- String Pimpl implementation

		StringImpl::Implementation(String* ref)
			:	m_str { CROISSANT_NEW uint8_t[1] }, m_charCount(0), m_byteCount(1), m_reservedLength { 1 }, m_ref(ref)
		{
			m_str[0] = '\0';
		}

		StringImpl::Implementation(String* ref, const char* str, EncodingType encoding)
			:	m_str(nullptr), m_charCount(0), m_byteCount(0), m_reservedLength { 0 }, m_ref(ref)
		{
			conversionTab[(uint32_t)encoding](str, m_str, m_charCount, m_byteCount, m_reservedLength);
		}

		StringImpl::Implementation(String* ref, const ConstIterator& begin, const ConstIterator& end)
			: m_str { nullptr }, m_charCount { 0 }, m_byteCount { 0 }, m_reservedLength { 0 }, m_ref { ref }
		{
			auto byteLength = (end.m_ptr - begin.m_ptr);

			if (byteLength <= 0)
			{
				m_byteCount = 1;
				m_charCount = 0;
				m_reservedLength = m_byteCount;
				m_str = CROISSANT_NEW uint8_t[m_reservedLength];
			}
			else
			{
				m_byteCount = byteLength + 1;
				m_reservedLength = m_byteCount;
				m_str = CROISSANT_NEW uint8_t[m_reservedLength];
				memcpy(m_str, begin.m_ptr, byteLength);
				m_str[m_byteCount - 1] = '\0';

				auto tmpBegin = begin;

				while (tmpBegin != end)
				{
					++m_charCount;
					++tmpBegin;
				}
			}
		}

		StringImpl::Implementation(String* ref, const StringImpl& src)
			:	m_str(nullptr), m_charCount(src.m_charCount), m_byteCount(src.m_byteCount), m_reservedLength { src.m_reservedLength }, m_ref(ref)
		{
			m_str = CROISSANT_NEW uint8_t[m_reservedLength];
			memcpy(m_str, src.m_str, m_byteCount);
		}

		StringImpl::Implementation(String* ref, StringImpl&& src)
			:	m_str { src.m_str }, m_charCount { src.m_charCount }, m_byteCount { src.m_byteCount }, m_reservedLength { src.m_reservedLength }, m_ref { ref }
		{
			src.m_reservedLength = 1;
			src.m_str = CROISSANT_NEW uint8_t[src.m_reservedLength];
			src.m_byteCount = src.m_reservedLength;
			src.m_charCount = 0;
		}

		StringImpl::~Implementation()
		{
			if (nullptr != m_str)
			{
				CROISSANT_DELETE_ARRAY_SAFE(m_str);
			}
		}

		const char* StringImpl::Str() const
		{
			return reinterpret_cast<char*>(m_str);
		}

		size_t StringImpl::Length() const
		{
			return m_charCount;
		}

		size_t StringImpl::ByteLength() const
		{
			return m_byteCount;
		}

		size_t StringImpl::ReservedLength() const
		{
			return m_reservedLength;
		}

		void StringImpl::Reserve(size_t size)
		{
			if (size <= m_reservedLength)
			{
				return;
			}

			uint8_t* tmp = m_str;
			uint8_t* newStr = CROISSANT_NEW uint8_t[size];

			memcpy(newStr, m_str, m_byteCount);
			m_str = newStr;
			m_reservedLength = size;
			CROISSANT_DELETE_ARRAY(tmp);
		}

		void StringImpl::Append(size_t repeat, char c)
		{
			int charSize = 1;
			uint8_t value[2];
			uint8_t* memToCopy = value;

			if (MASK_2CHAR(c))
			{
				charSize = 2;
				value[0] = 0b11000000 | ((0b11000000 & c) >> 6);
				value[1] = 0b10000000 | (0b00111111 & c);
			}
			else
			{
				value[0] = c;
			}

			uint32_t ByteCount = repeat * charSize;
			uint32_t newByteCount = m_byteCount + ByteCount;

			if (newByteCount> m_reservedLength)
			{
				Reserve(newByteCount + ByteCount);
			}
	
			uint8_t* curChar = m_str + (m_byteCount - 1); // on se place sur le '\0'

			for (uint32_t i = 0; i < repeat; ++i, curChar += charSize)
			{
				memcpy(curChar, memToCopy, charSize);
			}

			*curChar = '\0';
			m_byteCount = newByteCount;
			m_charCount += repeat;
		}

		void StringImpl::Append(const StringImpl& str)
		{
			auto newSize = m_byteCount + str.m_byteCount - 1;

			if (newSize >= m_reservedLength)
			{
				Reserve(m_reservedLength + (2 * str.m_byteCount));
			}

			auto ptr = m_str + m_byteCount - 1;

			memcpy(ptr, str.m_str, str.m_byteCount);
			m_charCount += str.m_charCount;
			m_byteCount += str.m_byteCount - 1;
		}

		void StringImpl::Append(const ConstIterator& first, const ConstIterator& end)
		{
			auto memToCopy = first.m_ptr;
			auto ByteCount = (end.m_ptr - first.m_ptr);
			auto newByteCount = m_byteCount + ByteCount;
			
			if (ByteCount < 0)
			{
				return;
			}

			if (newByteCount> m_reservedLength)
			{
				Reserve(newByteCount + (ByteCount * 2));
			}

			uint8_t* curChar = m_str + (m_byteCount - 1); // on se place sur le '\0'

			memcpy(curChar, memToCopy, ByteCount);
			curChar += ByteCount;
			*curChar = '\0';
			m_byteCount = newByteCount;

			auto tmpCur = first;

			while (tmpCur != end)
			{
				++m_charCount;
				++tmpCur;
			}
		}

		int StringImpl::Compare(const StringImpl& str) const
		{
			int result = memcmp(m_str, str.m_str, std::min(m_byteCount, str.m_byteCount));

			if (0 != result)
			{
				return result;
			}

			if (m_byteCount < str.m_byteCount)
			{
				return -1;
			}

			if (m_byteCount > str.m_byteCount)
			{
				return 1;
			}

			return 0;
		}

		void StringImpl::Replace(const StringImpl& search, const StringImpl& replace)
		{
			if (0 == search.m_charCount)
			{
				return;
			}

			auto ptr = m_str;
			int32_t diff = search.m_byteCount - replace.m_byteCount;

			if (diff == 0)
			{
				auto byteCount = search.m_byteCount - 1u;

				std::cout << "byteCount : " << byteCount << std::endl;
				for (uint32_t i = 0u, max = m_byteCount - search.m_byteCount; i < max; ++i, ++ptr)
				{
					if (memcmp(ptr, search.m_str, byteCount) == 0)
					{
						memcpy(ptr, replace.m_str, byteCount);
						ptr += (byteCount - 1u);
					}
				}
			}
			else if (diff < 0)
			{
				auto newReservedLength = m_byteCount * 2u;
				uint32_t newByteCount = 0u;
				auto newBuffer = CROISSANT_NEW uint8_t[newReservedLength];
				auto byteCount = search.m_byteCount - 1u;
				auto replaceByteCount = replace.m_byteCount - 1u;
				auto targetPtr = newBuffer;

				for (uint32_t i = 0u, max = m_byteCount - search.m_byteCount; i < max; ++i, ++ptr, ++targetPtr)
				{
					if (memcmp(ptr, search.m_str, byteCount) == 0)
					{
						memcpy(targetPtr, replace.m_str, replaceByteCount);
						ptr += (byteCount - 1u);
						targetPtr += (replaceByteCount - 1u);
						newByteCount += (replaceByteCount - 1u);
					}
					else
					{
						*targetPtr = *ptr;
						++newByteCount;
					}
				}

				newByteCount += search.m_byteCount + 1;
				memcpy(targetPtr, ptr, search.m_byteCount);
				CROISSANT_DELETE_ARRAY_SAFE(m_str);
				m_str = newBuffer;
				m_byteCount = newByteCount;
				m_reservedLength = newReservedLength;
				m_charCount = 0u;
				for (auto begin = Begin(), end = End(); begin != end; ++begin)
				{
					++m_charCount;
				}
			}
			else
			{
				throw Croissant::Exception::NotImplementedException("Replace avec diff > 0");
			}
		}

		ConstIterator StringImpl::Begin() const
		{
			return ConstIterator { m_str, m_str };
		}

		ConstIterator StringImpl::End() const
		{
			return ConstIterator { m_str + (m_byteCount - 1), m_str };
		}

		ConstReverseIterator StringImpl::RBegin() const
		{
			return ConstReverseIterator { End() };
		}

		ConstReverseIterator StringImpl::REnd() const
		{
			return ConstReverseIterator { Begin() };
		}

		uint32_t StringImpl::operator[](const uint32_t index) const
		{
			if (index > m_charCount)
			{
				return 0;
			}

			auto begin = Begin();

			for (uint32_t i = 0; i < index; ++i)
			{
				++begin;
			}

			return *begin;
		}

		StringImpl& StringImpl::operator=(const StringImpl& str)
		{
			if (nullptr != m_str)
			{
				CROISSANT_DELETE_ARRAY_SAFE(m_str);
			}

			m_byteCount = str.m_byteCount;
			m_charCount = str.m_charCount;
			m_reservedLength = str.m_reservedLength;
			m_str = CROISSANT_NEW uint8_t[m_reservedLength];
			memcpy(m_str, str.m_str, m_byteCount);
			return *this;
		}

		// ----------------------------------------- String implementation

		const uint32_t String::Npos = -1u;

		String::String()
			:	Pimpl<String>::Detail()
		{

		}

		String::String(const char* str)
			:	Pimpl<String>::Detail(str)
		{

		}

		String::String(const char* str, EncodingType encoding)
			:	Pimpl<String>::Detail(str, encoding)
		{

		}

		String::String(const ConstIterator& first, const ConstIterator& end)
			: Pimpl<String>::Detail(first, end)
		{

		}

		String::String(const String& src)
			:	Pimpl<String>::Detail(*src)
		{

		}

		String::String(String&& src)
			:	Pimpl<String>::Detail(std::move(*src))
		{

		}

		String::~String()
		{

		}

		const char* String::Str() const
		{
			const StringImpl& tmp = **this;

			return tmp.Str();
		}

		int String::Compare(const String& str) const
		{
			const StringImpl& tmp = **this;
			const StringImpl& strTmp = *str;

			return tmp.Compare(strTmp);
		}

		int String::Compare(const char* str) const
		{
			const StringImpl& tmp = **this;
			String string { str, EncodingType::LATIN1 };
			const StringImpl& strTmp = *string;

			return tmp.Compare(strTmp);
		}

		size_t String::Length() const
		{
			const StringImpl& tmp = **this;

			return tmp.Length();
		}

		size_t String::ByteLength() const
		{
			const StringImpl& tmp = **this;

			return tmp.ByteLength();
		}

		size_t String::ReservedLength() const
		{
			const StringImpl& tmp = **this;

			return tmp.ReservedLength();
		}

		ConstIterator String::Begin() const
		{
			const StringImpl& tmp = **this;

			return tmp.Begin();
		}

		ConstIterator String::End() const
		{
			const StringImpl& tmp = **this;

			return tmp.End();
		}

		ConstReverseIterator String::RBegin() const
		{
			const StringImpl& tmp = **this;

			return tmp.RBegin();
		}

		ConstReverseIterator String::REnd() const
		{
			const StringImpl& tmp = **this;

			return tmp.REnd();
		}

		String String::SubString(uint32_t start) const
		{
			if (start >= Length())
			{
				throw Croissant::Exception::CroissantException();
			}

			return SubString(start, Length() - start);
		}

		String String::SubString(uint32_t start, uint32_t length) const
		{
			if (start + length > Length())
			{
				throw Croissant::Exception::CroissantException();
			}

			auto begin = Begin() + start;
			auto end = begin + length;

			return String(begin, end);
		}

		uint32_t String::FindLastIndexOf(uint8_t c) const
		{
			auto result = String::Npos;
			auto begin = Begin();
			auto end = End();
			ConstIterator::ReturnValueType val;

			if (MASK_ASCII(c))
			{
				val = c;
			}
			else
			{
				val = (0b11000000 | ((0b11000000 & c) >> 6)) << 8;
				val = val | (0b10000000 | (0b00111111 & c));
			}

			for (uint32_t charIndex = 0; begin != end; ++begin, ++charIndex)
			{
				if (*begin == val)
				{
					result = charIndex;
				}
			}

			return result;
		}

		void String::Reserve(size_t size)
		{
			StringImpl& impl = **this;

			impl.Reserve(size);
		}

		void String::Append(size_t repeat, char c)
		{
			StringImpl& impl = **this;

			impl.Append(repeat, c);
		}

		void String::Append(const String& str)
		{
			StringImpl& impl = **this;
			const StringImpl& strImpl = *str;

			impl.Append(strImpl);
		}

		void String::Append(const ConstIterator& first, const ConstIterator& end)
		{
			StringImpl& tmp = **this;

			tmp.Append(first, end);
		}

		String& String::Replace(const String& search, const String& replace)
		{
			StringImpl& tmp = **this;
			const StringImpl& searchImpl = *search;
			const StringImpl& replaceImpl = *replace;

			tmp.Replace(searchImpl, replaceImpl);
			return *this;
		}

		bool String::operator==(const String& str) const
		{
			const StringImpl& tmp = **this;
			const StringImpl& tmpStr = *str;

			return 0 == tmp.Compare(tmpStr);
		}

		bool String::operator<(const String& str) const
		{
			const StringImpl& tmp = **this;
			const StringImpl& strImp = *str;

			return tmp.Compare(strImp) < 0;
		}

		bool String::operator<=(const String& str) const
		{
			const StringImpl& tmp = **this;
			const StringImpl& strImp = *str;

			return tmp.Compare(strImp) <= 0;
		}

		String& String::operator+=(const String& str)
		{
			StringImpl& impl = **this;
			const StringImpl& strImp = *str;

			impl.Append(strImp);
			return *this;
		}

		uint32_t String::operator[](const uint32_t index) const
		{
			StringImpl const& impl = **this;

			return impl[index];
		}

		String& String::operator=(const String& str)
		{
			StringImpl& tmp = **this;
			const StringImpl& tmpStr = *str;

			tmp = tmpStr;
			return *this;
		}

		// ----------------------------------------- String::ConstIterator implementation
		ConstIterator::ConstIterator(ConstIterator::Pointer ptr, ConstIterator::Pointer minPtr)
			: m_ptr(ptr), m_minPtr(--minPtr)
		{
		}

		ConstIterator::ConstIterator(const ConstIterator& src)
                        : m_ptr(src.m_ptr), m_minPtr(src.m_minPtr)
                {
                }

		 ConstIterator::ConstIterator(ConstIterator&& src)
                        : m_ptr(src.m_ptr), m_minPtr(src.m_minPtr)
                {
				src.m_ptr = nullptr;
				src.m_minPtr = nullptr;
                }

		ConstIterator::SelfType& ConstIterator::operator=(const ConstIterator& src)
		{
			m_ptr = src.m_ptr;
			return *this;
		}

		ConstIterator::SelfType& ConstIterator::operator=(ConstIterator&& src)
                {
                        m_ptr = src.m_ptr;
			src.m_ptr = nullptr;
			return *this;
                }


		ConstIterator::SelfType& ConstIterator::operator++()
		{
			if (m_ptr == m_minPtr)
			{
				++m_ptr;
				return *this;
			}

			auto c = *m_ptr;

			if (MASK_ASCII(c))
			{
				++m_ptr;
			}
			else if (MASK_2CHAR(c))
			{
				m_ptr += 2;
			}
			else if (MASK_3CHAR(c))
			{
				m_ptr += 3;
			}
			else if (MASK_4CHAR(c))
			{
				m_ptr += 4;
			}

			return *this;
		}

		ConstIterator::SelfType&  ConstIterator::operator--()
		{
			--m_ptr;
			if (m_ptr != m_minPtr && !MASK_ASCII(*m_ptr))
			{
				--m_ptr;
				if (!MASK_2CHAR(*m_ptr))
				{
					--m_ptr;
					if (!MASK_3CHAR(*m_ptr))
					{
						--m_ptr;
						if (!MASK_4CHAR(*m_ptr))
						{
							std::cout << "Avant l'exception" << std::endl;
							throw Croissant::Exception::CroissantException();
						}
					}
				}
			}

			return *this;
		}

		ConstIterator::SelfType ConstIterator::operator++(int)
		{
			auto tmp = *this;

			++(*this);
			return tmp;
		}

		ConstIterator::SelfType ConstIterator::operator--(int)
		{
			auto tmp = *this;

			--(*this);
			return tmp;
		}

		ConstIterator::SelfType ConstIterator::operator+(int right) const
		{
			SelfType result { *this };

			for (int i = 0; i < right; ++i)
			{
				++result;
			}

			return result;
		}

		ConstIterator::SelfType ConstIterator::operator-(int right) const
		{
			SelfType result { *this };

			for (int i = 0; i < right; ++i)
			{
				--result;
			}

			return result;
		}

		ConstIterator::ReturnValueType ConstIterator::operator*() const
		{
			ConstIterator::ReturnValueType result = 0;
			auto c = *m_ptr;

			if (MASK_ASCII(c))
			{
				result = GetChar<1>(m_ptr);
				//result = c;
			}
			else if (MASK_2CHAR(c))
			{
				//result = *reinterpret_cast<uint16_t*>(m_ptr + 1);
				result = GetChar<2>(m_ptr);
			}
			else if (MASK_3CHAR(c))
			{
				//uint8_t* tmp = reinterpret_cast<uint8_t*>(&result);

				//memcpy(tmp + 1, m_ptr, 3);
				//result = *reinterpret_cast<uint32_t>(m_ptr + 2);
				result = GetChar<3>(m_ptr);
			}
			else if (MASK_4CHAR(c))
			{
				//result = *reinterpret_cast<uint32_t*>(m_ptr + 3);
				result = GetChar<4>(m_ptr);
			}

			return result;
		}

		bool ConstIterator::operator==(const SelfType& rhs)
		{
			return m_ptr == rhs.m_ptr;
		}

		bool ConstIterator::operator!=(const SelfType& rhs)
		{
			return m_ptr != rhs.m_ptr;
		}

		// ----------------------------------------- String::ConstReverseIterator implementation
		ConstReverseIterator::ConstReverseIterator(ConstReverseIterator::IteratorType it)
			: m_it { it - 1 }
		{
		}

		ConstReverseIterator::SelfType& ConstReverseIterator::operator++()
		{
			--m_it;
			return *this;
		}

		ConstReverseIterator::SelfType ConstReverseIterator::operator++(int)
		{
			auto tmp = *this;

			--m_it;
			return tmp;
		}

		ConstReverseIterator::SelfType& ConstReverseIterator::operator--()
		{
			++m_it;
			return *this;
		}

		ConstReverseIterator::SelfType ConstReverseIterator::operator--(int)
		{
			auto tmp = *this;

			++m_it;
			return tmp;
		}

		ConstReverseIterator::SelfType ConstReverseIterator::operator+(int right) const
		{
			return ConstReverseIterator { m_it - (right - 1) };
		}

		ConstReverseIterator::SelfType ConstReverseIterator::operator-(int right) const
		{
			return ConstReverseIterator { m_it + (right + 1) };
		}

		const ConstReverseIterator::IteratorType ConstReverseIterator::Base() const
		{
			return m_it + 1;
		}

		ConstReverseIterator::ReturnValueType ConstReverseIterator::operator*() const
		{
			return *m_it;
		}

		bool ConstReverseIterator::operator==(const ConstReverseIterator::SelfType& rhs)
		{
			return m_it == rhs.m_it;
		}

		bool ConstReverseIterator::operator!=(const ConstReverseIterator::SelfType& rhs)
		{
			return m_it != rhs.m_it;
		}

		// ----------------------------------------- free function implementation

		String operator+(const String& left,const String& right)
		{
			return String(left) += right;
		}

		String operator+(const char* left, const String& right)
		{
			return String(left) += right;
		}

		std::ostream& operator<<(std::ostream& stream, const Croissant::Core::String& str)
		{
			return stream << str.Str();
		}


	}
}
