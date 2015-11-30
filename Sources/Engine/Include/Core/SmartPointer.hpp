//#ifndef SMARTPOINTER_HPP_
//#define SMARTPOINTER_HPP_
//#pragma once
//
//#include <memory>
//#include <utility>
////#pragma once
////
////#include <stdint.h>
////#include <assert.h>
////#include "Core\DebugMemoryManager.h"
////
////namespace Croissant
////{
////	namespace Core
////	{
////		template<typename T>
////		class SmartPointer
////		{
////		public:
////			SmartPointer() : m_internal(NULL) { }
////			SmartPointer(T* obj) : m_internal(NULL) { m_internal = CROISSANT_NEW SmartPointerInternal(obj);  m_internal->AddRef(); }
////			SmartPointer(const SmartPointer& o) : m_internal(o.m_internal) { if (NULL != m_internal) m_internal->AddRef(); }
////			~SmartPointer() { if (NULL != m_internal) {m_internal->Release(); m_internal = NULL; }}
////
////			bool IsValid() const { return NULL != m_internal && m_internal->GetRefCount() > 0; }
////			T& operator*() {
////				assert(NULL != m_internal);
////				return m_internal->GetReference();
////			}
////
////			T* operator->(){
////				assert(NULL != m_internal);
////				return m_internal->GetPointer();
////			}
////
////			SmartPointer& operator=(const SmartPointer& o)
////			{
////				if (NULL != m_internal)
////				{
////					m_internal->Release();
////					m_internal = NULL;
////				}
////
////				m_internal = o.m_internal;
////				if (NULL != m_internal)
////				{
////					m_internal->AddRef();
////				}
////
////				return *this;
////			}
////
////			bool operator==(const SmartPointer& left) const
////			{
////				if (NULL == m_internal || NULL == left.m_internal)
////				{
////					return false;
////				}
////
////				return m_internal->GetPointer() == left.m_internal->GetPointer();
////			}
////
////			bool operator!=(const SmartPointer& left) const
////			{
////				if (NULL == m_internal || NULL == left.m_internal)
////				{
////					return true;
////				}
////
////				return m_internal->GetPointer() != left.m_internal->GetPointer();
////			}
////
////			int GetRefCount() const { assert(NULL != m_internal); return m_internal->GetRefCount(); }
////		private:
////
////			class SmartPointerInternal
////			{
////			public:
////				SmartPointerInternal(T* obj) : m_obj(obj), m_refCount(0) { assert(NULL != obj); }
////				SmartPointerInternal(const SmartPointerInternal& o) { assert(0 && _T("The copy constructor must not be used")); }
////				inline void AddRef() { assert(NULL != m_obj); ++m_refCount; }
////				inline void Release() {
////					assert(m_refCount > 0);
////					--m_refCount;
////					if (m_refCount == 0)
////					{
////						CROISSANT_DELETE(m_obj);
////						m_obj = NULL;
////						CROISSANT_DELETE(this); // dead or alive ?
////					}
////				}
////
////				inline T& GetReference() {
////					return *m_obj;
////				}
////
////				inline T* GetPointer() {
////					return m_obj;
////				}
////
////				inline int GetRefCount() const { return m_refCount; }
////
////			private:
////				T* m_obj;
////				int m_refCount;
////			};
////
////			SmartPointerInternal* m_internal;
////
////		};
////	}
////}
////
////
//
//template<typename TType>
//std::unique_ptr<TType> makeUnique()
//{
//	return std::unique_ptr<TType>(new TType());
//}
//
//template<typename TType, typename TFirst>
//std::unique_ptr<TType> makeUnique(TFirst&& first)
//{
//	return std::unique_ptr<TType>(new TType(std::forward(first)));
//}
//
//template<typename TType, typename TFirst, typename TSecond>
//std::unique_ptr<TType> makeUnique(TFirst&& first, TSecond&& second)
//{
//	return std::unique_ptr<TType>(new TType(std::forward(first), std::forward(second)));
//}
//
//#endif
