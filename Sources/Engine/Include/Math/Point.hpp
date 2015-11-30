#pragma once
#ifndef POINT_H_
#define POINT_H_

#include "Engine.hpp"

namespace Croissant
{
	namespace Math
	{
	    class Vector;

		class ENGINE_API Point
		{
			friend class Vector;

		public:
			Point();
			Point(float x, float y, float z);
			Point(const Point& source);
			Point(Point&& source);
			~Point();
			float X() const;
			float Y() const;
			float Z() const;
			Point& operator=(const Point& source);
			Point& operator=(Point&& source);
			bool operator==(const Point& point) const;
			bool operator!=(const Point& point) const;
			Vector operator-(const Point& point) const;


		private:
			float m_values[3];
		};
	}
}

#endif
