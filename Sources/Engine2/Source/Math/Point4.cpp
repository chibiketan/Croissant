//
// Created by Gregory_Compte on 15/12/2015.
//

#include "Math/Point4.hpp"
#include "Math/Matrix4.hpp"
#include <assert.h>

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
		}

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

		Point4 Point4::operator*(Matrix4 const& right) const
		{
			// Le nombre de colonne de la matrice de gauche est égal au nombre de ligne de celle de droite
			//	                    |--------------|
			//  |--------------|    |xx, xy, xz, xw|    
			// P|xp, yp, zp, wp| X M|yx, yy, yz, yw|
			//  |--------------|    |zx, zy, zz, zw|    
			//                      |wx, wy, wz, ww| 
			//                      |--------------|
			// multiplication
			//   |------------------------------------------------------------------------------------------------|
			// P'|xp*xx+yp*yx+zp*zx+wp*wx, xp*xy+yp*yy+zp*zy+wp*wy, xp*xz+yp*yz+zp*zz+wp*wz, xp*xw+yp*yw+zp*zw+wp*ww|
			//   |------------------------------------------------------------------------------------------------|
			return Point4{
				X()*right(0, 0) + Y()*right(1, 0) + Z()*right(2, 0) + W()*right(3, 0),
				X()*right(0, 1) + Y()*right(1, 1) + Z()*right(2, 1) + W()*right(3, 1),
				X()*right(0, 2) + Y()*right(1, 2) + Z()*right(2, 2) + W()*right(3, 2),
				X()*right(0, 3) + Y()*right(1, 3) + Z()*right(2, 3) + W()*right(3, 3)
			};
		}

		bool Point4::operator==(Point4 const& right) const
		{
			for (auto i = 0; i < m_elements.size(); ++i)
			{
				if (!AlmostEqual2sComplement(m_elements[i], m_elements[i], 1))
				{
					return false;
				}
			}

			return true;
			//return m_elements == right.m_elements;
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
