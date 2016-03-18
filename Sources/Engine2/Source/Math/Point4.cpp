//
// Created by Gregory_Compte on 15/12/2015.
//

#include "Math/Point4.hpp"

namespace Croissant
{
	namespace Math
	{
		Point4::Point4()
			: m_elements { type(0.0f), type(0.0f), type(0.0f), type(0.0f) }
		{
		}

		Point4::Point4(type x, type y, type z, type w)
			: m_elements { x, y, z, w }
		{
		}

		Point4::type_ref Point4::X()
		{
			return m_elements[0];
		}

		Point4::type Point4::X() const
		{
			return m_elements[0];
		}

		Point4::type_ref Point4::Y()
		{
			return m_elements[1];
		}

		Point4::type Point4::Y() const
		{
			return m_elements[1];
		}

		Point4::type_ref Point4::Z()
		{
			return m_elements[2];
		}

		Point4::type Point4::Z() const
		{
			return m_elements[2];
		}

		Point4::type_ref Point4::W()
		{
			return m_elements[3];
		}

		Point4::type Point4::W() const
		{
			return m_elements[3];
		}

		std::ostream& operator<<(std::ostream& out, Point4 const& point)
		{
			auto precision = out.precision();
			auto oldSetf = out.setf(std::ios_base::fixed, std::ios_base::floatfield);

			out.precision(2);
			out << "(" << point.X() << ", " << point.Y() << ", " << point.Z() << ", " << point.W() << ")";
			out.setf(oldSetf);
			out.precision(precision);
			return out;
		}
	}
}