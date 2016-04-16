//
// Created by Gregory_Compte on 15/12/2015.
//

#include "Math/Point4.hpp"
#include "Math/Vector4.hpp"
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <iostream>

#undef max

namespace Croissant
{
	namespace Math
	{
		namespace
		{
			// TODO : extraire ce morceau de code dans un header utilitaire
			// from http://www.cygnus-software.com/papers/comparingfloats/Comparing%20floating%20point%20numbers.htm
			// Usable AlmostEqual function
			bool AlmostEqual2sComplement(float A, float B, int maxUlps)
			{
				// Make sure maxUlps is non-negative and small enough that the
				// default NAN won't compare as equal to anything.
				assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
				int aInt = *reinterpret_cast<int*>(&A);
				// Make aInt lexicographically ordered as a twos-complement int
				if (aInt < 0)
					aInt = 0x80000000 - aInt;
				// Make bInt lexicographically ordered as a twos-complement int
				int bInt = *reinterpret_cast<int*>(&B);
				if (bInt < 0)
					bInt = 0x80000000 - bInt;
				int intDiff = abs(aInt - bInt);
				if (intDiff <= maxUlps)
					return true;
				return false;
			}

			bool EqualWithEpsilon(float a, float b)
			{
				return std::fabs(a - b) < 0.005;
			}
		}

		Point4::Point4()
			: Point4(0.0f, 0.0f, 0.0f)
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

		Point4::type Point4::DotProduct(Vector4 const& v) const
		{
			return (m_elements[0] * v.X())
				+ (m_elements[1] * v.Y())
				+ (m_elements[2] * v.Z())
				+ (m_elements[3] * v.W());
		}

		bool Point4::operator==(Point4 const& right) const
		{
			//auto t = std::mismatch(m_elements.begin(), m_elements.end(), right.m_elements.begin(), right.m_elements.end(), [](float const& l, float const& r) { return AlmostEqual2sComplement(l, r, 100000); });
			auto t = std::mismatch(m_elements.begin(), m_elements.end(), right.m_elements.begin(), right.m_elements.end(), EqualWithEpsilon);

			return t.first == m_elements.end();
			//for (auto i = 0; i < m_elements.size(); ++i)
			//{
			//	if (!AlmostEqual2sComplement(m_elements[i], right.m_elements[i], 1))
			//	//if (!EqualWithEpsilon(m_elements[i], right.m_elements[i]))
			//	{
			//		return false;
			//	}
			//}

			return true;
			//return m_elements == right.m_elements;
		}

		Point4& Point4::operator+=(Vector4 const& right)
		{
			m_elements[0] += right.X();
			m_elements[1] += right.Y();
			m_elements[2] += right.Z();
			m_elements[3] += right.W();
			return *this;
		}

		std::ostream& operator<<(std::ostream& out, Point4 const& point)
		{
			auto precision = out.precision();
			auto oldSetf = out.setf(std::ios_base::fixed, std::ios_base::floatfield);

			out.precision(6);
			out << "(" << point.X() << ", " << point.Y() << ", " << point.Z() << ", " << point.W() << ")";
			out.setf(oldSetf);
			out.precision(precision);
			return out;
		}
	}
}
