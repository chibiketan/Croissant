#pragma once
#ifndef DELEGATE_H_
#define DELEGATE_H_

#include "Core/DebugMemoryManager.hpp"
#include "Engine.hpp"

#include <memory>

namespace Croissant
{
	namespace Core
	{
		template<class Function>
		class ENGINE_API Delegate
		{
		public:
			Delegate(Function function)
				: m_function(CROISSANT_NEW Function(function))
			{
			}

			template<class Fun>
			Delegate(Fun function)
				: m_function(CROISSANT_NEW Function(function))
			{
			}

			Delegate(const Delegate& right)
				: m_function(right.m_function)
			{
			}

			bool operator==(const Delegate& right)
			{
				return m_function.get() == right.m_function.get();
			}

			bool operator!=(const Delegate& right)
			{
				return m_function.get() != right.m_function.get();
			}

			template<typename... Args>
			void operator()(Args&&... args)
			{
				(*m_function)(args...);
			}

		private:
			std::shared_ptr<Function> m_function;
		};
	}
}


#endif /* DELEGATE_H_ */
