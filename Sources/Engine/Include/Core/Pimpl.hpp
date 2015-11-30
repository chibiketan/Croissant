#pragma once
#ifndef PIMPL_H_
#define PIMPL_H_

#include "Engine.hpp"
#include "Core/DebugMemoryManager.hpp"
#include <memory>
#include <type_traits>

#include "Tpl/TypeTraits.hpp"

namespace Croissant
{
	namespace Core
	{


	    template<class U>
	    class ENGINE_API Pimpl
	    {
        public:

            class ENGINE_API Detail;
            class ENGINE_API Implementation;

	    };

        template<class U>
        class ENGINE_API Pimpl<U>::Detail
        {
        public:
            typedef Pimpl<U>::Detail Base;
            typedef Pimpl<U>::Implementation Implementation;

        protected:
            inline Detail(Detail& right)
                : m_ptr(right.m_ptr)
            {

            }

            inline Detail(Detail&& right)
                : m_ptr(std::move(right.m_ptr))
            {
                m_ptr->SetRef(static_cast<U*>(this));
            }

            inline Detail()
                : m_ptr(CROISSANT_NEW Implementation(static_cast<U*>(this)))
            {
            }

            inline Detail(const Implementation& ref)
                : m_ptr(CROISSANT_NEW Implementation(static_cast<U*>(this), ref))
            {
            }

            inline Detail(Implementation&& ref)
                : m_ptr(CROISSANT_NEW Implementation(static_cast<U*>(this), std::move(ref)))
            {
            }

            template<typename T1>
            inline Detail(T1 p1)
                : m_ptr(CROISSANT_NEW Implementation(static_cast<U*>(this), std::forward<T1>(p1)))
            {
            }

            template<typename T1, typename T2>
            inline Detail(T1 p1, T2 p2)
                : m_ptr(CROISSANT_NEW Implementation(static_cast<U*>(this), std::forward<T1>(p1), std::forward<T2>(p2)))
            {

            }

//			template<typename T1, typename T2>
//			inline Detail(typename Croissant::Tpl::TypeTraits<T1>::ParamType p1, typename Croissant::Tpl::TypeTraits<T2>::ParamType p2)
//                : m_ptr(CROISSANT_NEW Implementation(static_cast<U*>(this), p1, p2))
//			{
//			}

            template<typename T1, typename T2, typename T3>
            inline Detail(T1 p1, T2 p2, T3 p3)
                : m_ptr(CROISSANT_NEW Implementation(static_cast<U*>(this), std::forward<T1>(p1), std::forward<T2>(p2), std::forward<T3>(p3)))
            {

            }

            inline Implementation& operator*()
            {
                return *m_ptr.get();
            }

            inline const Implementation& operator*() const
            {
                return *m_ptr.get();
            }
        private:
            std::unique_ptr<Implementation> m_ptr;
        };

	}
}

#endif
