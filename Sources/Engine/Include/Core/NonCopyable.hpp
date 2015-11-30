#pragma once
#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

#include "Engine.hpp"

namespace Croissant
{
	namespace Core
	{
		class ENGINE_API NonCopyable
		{
        public:
			NonCopyable(const NonCopyable&) = delete;
			NonCopyable& operator=(const NonCopyable&) = delete;

		protected:
			NonCopyable() = default;
			~NonCopyable() = default;
		};
	}
}

#endif
