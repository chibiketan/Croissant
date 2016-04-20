#include "Math/Quaternion.hpp"
#include "Math/Vector4.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Math.hpp"

// http://www.cprogramming.com/tutorial/3d/quaternions.html
// layout :
// w = m_elements[0]
// x = m_elements[1]
// y = m_elements[2]
// z = m_elements[3]

namespace Croissant
{
	namespace Math
	{
		Quaternion::Quaternion(Vector4 const& vector, float angle)
			: m_elements { Cos(angle / 2.0f), vector.X() * Sin(angle / 2.0f), vector.Y() * Sin(angle / 2.0f),
			vector.Z() * Sin(angle / 2.0f) }
		{
		}

		Quaternion::Quaternion(float w, float x, float y, float z)
			: m_elements { w, x, y, z }
		{
		}

		float Quaternion::Length() const
		{
			return std::sqrt(W() * W()
				+ X() * X()
				+ Y() * Y()
				+ Z() * Z());
		}

		Quaternion& Quaternion::operator*=(Quaternion const& right)
		{
			// (x0*i + y0*j + z0*k + w0)*(x1*i + y1*j + z1*k + w1)
			// =
			// i*(+x0*w1 + y0*z1 - z0*y1 + w0*x1) +
			// j*(-x0*z1 + y0*w1 + z0*x1 + w0*y1) +
			// k*(+x0*y1 - y0*x1 + z0*w1 + w0*z1) +
			// 1*(-x0*x1 - y0*y1 - z0*z1 + w0*w1)
			auto x = X();
			auto y = Y();
			auto z = Z();
			auto w = W();

			m_elements[0] = -x * right.X() - y * right.Y() - z * right.Z() + w * right.W();
			m_elements[1] = x * right.W() + y * right.Z() - z * right.Y() + w * right.X();
			m_elements[2] = -x * right.Z() + y * right.W() + z * right.X() + w * right.Y();
			m_elements[3] = x * right.Y() - y * right.X() + z * right.W() + w * right.Z();

			return *this;
		}

		Quaternion Quaternion::operator*(Quaternion const& right) const
		{
			Quaternion result{ *this };

			result *= right;
			return result;
		}

		std::ostream& operator<<(std::ostream& out, Quaternion const& quat)
		{
			auto precision = out.precision();
			auto oldSetf = out.setf(std::ios_base::fixed, std::ios_base::floatfield);

			out.precision(6);
			out << "(W: " << quat.m_elements[0] << ", X: " << quat.m_elements[1] << ", Y: " << quat.m_elements[2] << ", Z: " << quat.m_elements[3] << ")";
			out.setf(oldSetf);
			out.precision(precision);
			return out;
		}
	}
}