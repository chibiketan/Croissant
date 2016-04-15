#include "Math/Vector4.hpp"
#include "Math/Point4.hpp"
#include "Math/Matrix4.hpp"
#include <cmath>

namespace Croissant
{
	namespace Math
	{
		Vector4 const Vector4::UnitX{ 1.0f, 0.0f, 0.0f };
		Vector4 const Vector4::UnitY{ 0.0f, 1.0f, 0.0f };
		Vector4 const Vector4::UnitZ{ 0.0f, 0.0f, 1.0f };
		Vector4 const Vector4::Zero{ 0.0f, 0.0f, 0.0f };

		Vector4::Vector4(type x, type y, type z, type w)
		{
			this->m_components[0] = x;
			this->m_components[1] = y;
			this->m_components[2] = z;
			this->m_components[3] = w;
		}

		Vector4::const_reference Vector4::X() const
		{
			return m_components.at(0);
		}

		Vector4::const_reference Vector4::Y() const
		{
			return m_components.at(1);
		}

		Vector4::const_reference Vector4::Z() const
		{
			return m_components.at(2);
		}

		Vector4::const_reference Vector4::W() const
		{
			return m_components.at(3);
		}

		Vector4::type Vector4::Length() const
		{
			return std::sqrt(this->m_components[0] * this->m_components[0]
				+ this->m_components[1] * this->m_components[1]
				+ this->m_components[2] * this->m_components[2]
				+ this->m_components[3] * this->m_components[3]);
		}

		void Vector4::MakeUnit()
		{
			auto len = this->Length();

			if (len != 1.0f)
			{
				auto invLen = 1.0f / len;
				m_components[0] *= invLen;
				m_components[1] *= invLen;
				m_components[2] *= invLen;
				m_components[3] *= invLen;
			}
		}

		Vector4::type Vector4::DotProduct(Vector4 const& v) const
		{
			return (m_components[0] * v.m_components[0])
				+ (m_components[1] * v.m_components[1])
				+ (m_components[2] * v.m_components[2])
				+ (m_components[3] * v.m_components[3]);
		}

		Vector4::type Vector4::DotProduct(Point4 const& p) const
		{
			return (m_components[0] * p.X())
				+ (m_components[1] * p.Y())
				+ (m_components[2] * p.Z())
				+ (m_components[3] * p.W());
		}

		Vector4 Vector4::operator*(Matrix4 const& right) const
		{
			return Vector4{
				X()*right(0, 0) + Y()*right(1, 0) + Z()*right(2, 0) + W()*right(3, 0),
				X()*right(0, 1) + Y()*right(1, 1) + Z()*right(2, 1) + W()*right(3, 1),
				X()*right(0, 2) + Y()*right(1, 2) + Z()*right(2, 2) + W()*right(3, 2),
				X()*right(0, 3) + Y()*right(1, 3) + Z()*right(2, 3) + W()*right(3, 3)
			};
		}

		Vector4 Vector4::operator-() const
		{
			return Vector4{
				-m_components[0],
				-m_components[1],
				-m_components[2],
				1
			};
		}

		Vector4 Vector4::operator*(type val) const
		{
			return Vector4{
				m_components[0] * val,
				m_components[1] * val,
				m_components[2] * val,
				1
			};
		}

		Vector4 Vector4::operator+(Vector4 const& right) const
		{
			return Vector4{
				X() + right.X(),
				Y() + right.Y(),
				Z() + right.Z(),
				1
			};
		}

		std::ostream& operator<<(std::ostream& out, Vector4 const& vec)
		{
			out << "{ " << vec.X() << ", " << vec.Y() << ", " << vec.Z() << ", " << vec.W() << " }";
			return out;
		}
	}
}
