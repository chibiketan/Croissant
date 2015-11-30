#include "stdafx.hpp"
#include "Core/DebugMemoryManager.hpp"
#include "Math/Point.hpp"
#include "Math/Vector.hpp"

#include <string.h>

namespace Croissant
{
	namespace Math
	{
		Point::Point()
		{
			memset(m_values, 0, sizeof(m_values));
		}

		Point::Point(float x, float y, float z)
		{
			m_values[0] = x;
			m_values[1] = y;
			m_values[2] = z;
		}

		Point::Point(const Point& source)
		{
			memcpy(m_values, source.m_values, sizeof(m_values));
		}

		Point::Point(Point&& source)
		{
			memcpy(m_values, source.m_values, sizeof(m_values));
			memset(source.m_values, 0, sizeof(source.m_values));
		}

		Point::~Point()
		{
		}

		float Point::X() const
		{
			return m_values[0];
		}

		float Point::Y() const
		{
			return m_values[1];
		}

		float Point::Z() const
		{
			return m_values[2];
		}

		Point& Point::operator=(const Point& source)
		{
			memcpy(m_values, source.m_values, sizeof(m_values));
			return *this;
		}

		Point& Point::operator=(Point&& source)
		{
			memcpy(m_values, source.m_values, sizeof(m_values));
			memset(source.m_values, 0, sizeof(source.m_values));
			return *this;
		}

		bool Point::operator==(const Point& point) const
		{
			return memcmp(m_values, point.m_values, sizeof(m_values)) == 0;
		}

		bool Point::operator!=(const Point& point) const
		{
			return !(*this == point);
		}

		Vector Point::operator-(const Point& point) const
		{
			return Vector(m_values[0] - point.m_values[0],
				m_values[1] - point.m_values[1],
				m_values[2] - point.m_values[2]);
		}
	}
}
