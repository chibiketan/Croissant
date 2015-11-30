#include "stdafx.hpp"
#include "Math/Vector.hpp"

namespace Croissant
{
	namespace Math
	{
		Vector::Vector(float x, float y, float z)
		{
			m_values[0] = x;
			m_values[1] = y;
			m_values[2] = z;
			m_values[3] = 0.0f;
		}

		Vector::~Vector()
		{
		}

		float Vector::X() const
		{
			return m_values[0];
		}

		float Vector::Y() const
		{
			return m_values[1];
		}

		float Vector::Z() const
		{
			return m_values[2];
		}
	}
}
