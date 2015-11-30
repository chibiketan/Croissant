/*
 * String.hpp
 *
 *  Created on: 10 juil. 2013
 *      Author: gcompte
 */

#pragma once

#ifndef STRING_HPP_
#define STRING_HPP_

#include "Engine.hpp"
#include "Core/Pimpl.hpp"
#include <ostream>

namespace Croissant
{
	namespace Core
	{
		enum class EncodingType
		{
			ASCII = 0,
			LATIN1,
			UTF8,
			UTF16,
			UTF32,
			DETECT,
			ENCODING_TYPE_SIZE
		};

		class ENGINE_API String : public Pimpl<String>::Detail
		{
		public:
			class ENGINE_API ConstIterator
			{
			public:
				using SelfType = ConstIterator;
				using ReturnValueType = uint32_t;
				using ReturnReference = uint32_t&;
				using ReturnPointer = uint32_t*;
				using Pointer = uint8_t*;
				using DifferenceType = uint32_t;
				using IteratorCategory = std::forward_iterator_tag;

				ConstIterator(const ConstIterator& src);
				ConstIterator(ConstIterator&& src);
				SelfType& operator=(const ConstIterator& src);
				SelfType& operator=(ConstIterator&& src);
				SelfType& operator++();
				SelfType operator++(int);
				SelfType& operator--();
				SelfType operator--(int);
				SelfType operator+(int right) const;
				SelfType operator-(int right) const;
				ReturnValueType operator*() const;
				bool operator==(const SelfType& rhs);
				bool operator!=(const SelfType& rhs);

				friend class Pimpl<String>::Implementation;

			private:
				ConstIterator(Pointer ptr, Pointer minPtr);
				Pointer m_ptr;
				Pointer m_minPtr;
			};

			class ENGINE_API ConstReverseIterator
			{
			public:
				using SelfType = ConstReverseIterator;
				using IteratorType = ConstIterator;
				using ReturnValueType = IteratorType::ReturnValueType;
				using ReturnReference = IteratorType::ReturnReference;
				using ReturnPointer = IteratorType::ReturnPointer;
				using Pointer = IteratorType::Pointer;
				using DifferenceType = IteratorType::DifferenceType;
				using IteratorCategory = std::forward_iterator_tag;

				ConstReverseIterator(IteratorType it);
				SelfType& operator++();
				SelfType operator++(int);
				SelfType& operator--();
				SelfType operator--(int);
				SelfType operator+(int right) const;
				SelfType operator-(int right) const;
				const IteratorType Base() const;
				ReturnValueType operator*() const;
				bool operator==(const SelfType& rhs);
				bool operator!=(const SelfType& rhs);
			private:
				IteratorType m_it;
			};

			const static uint32_t Npos;

			String();
			String(const char* str);
			String(const char* str, EncodingType encoding);
			String(const ConstIterator& first, const ConstIterator& end);
			String(const String& src);
			String(String&& src);
			virtual ~String();

			const char* Str() const;
			int Compare(const String& str) const;
			int Compare(const char* str) const;
			size_t Length() const;
			size_t ByteLength() const;
			size_t ReservedLength() const;
			ConstIterator Begin() const;
			ConstIterator End() const;
			ConstReverseIterator RBegin() const;
			ConstReverseIterator REnd() const;
			void Reserve(size_t size);
			void Append(size_t repeat, char c);
			void Append(const String& str);
			void Append(const ConstIterator& first, const ConstIterator& end);
			String SubString(uint32_t start, uint32_t length) const;
			String SubString(uint32_t start) const;
			uint32_t FindLastIndexOf(uint8_t c) const;
			String& Replace(const String& search, const String& replace);

			String& operator=(const String& str);
			bool operator==(const String& str) const;
			bool operator<(const String& str) const;
			bool operator<=(const String& str) const;
			String& operator+=(const String& str);
			uint32_t operator[](const uint32_t index) const;
			friend String operator+(const String& left,const String& right);
			friend String operator+(const char* left, const String& right);
			friend std::ostream& operator<<(std::ostream& stream, const String& str);

			// Trim
			// Empty
			// IndexOf

		private:
		};

		ENGINE_API String operator+(const String& left,const String& right);
		ENGINE_API String operator+(const char* left, const String& right);
		ENGINE_API std::ostream& operator<<(std::ostream& stream, const String& str);

	}
}

#endif /* STRING_HPP_ */
