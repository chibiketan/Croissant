#pragma once
#ifndef VECTOR_H_
#define VECTOR_H_

#include "Engine.hpp"

namespace Croissant
{
	namespace Math
	{
		class ENGINE_API Vector
		{
			friend class Point;
		public:
			Vector(float x, float y, float z);
			~Vector();
			float X() const;
			float Y() const;
			float Z() const;


		private:
			float m_values[4];
		};
	}
}

#endif
