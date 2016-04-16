#include "Math/Quaternion.hpp"
#include "Math/Vector4.hpp"
#include "Math/Matrix4.hpp"
#include "Math/Math.hpp"

namespace Croissant
{
	namespace Math
	{
		Quaternion::Quaternion(Vector4 const& vector, float angle)
			: m_elements { vector.X() * Sin(angle / 2.0f), vector.Y() * Sin(angle / 2.0f),
			vector.Z() * Sin(angle / 2.0f), Cos(angle / 2.0f) }
		{
		}

		float Quaternion::Length() const
		{
			return std::sqrt(this->m_elements[0] * this->m_elements[0]
				+ this->m_elements[1] * this->m_elements[1]
				+ this->m_elements[2] * this->m_elements[2]
				+ this->m_elements[3] * this->m_elements[3]);
		}

		Quaternion& Quaternion::operator*=(Quaternion const& right)
		{
			// (x0*i + y0*j + z0*k + w0)*(x1*i + y1*j + z1*k + w1)
			// =
			// i*(+x0*w1 + y0*z1 - z0*y1 + w0*x1) +
			// j*(-x0*z1 + y0*w1 + z0*x1 + w0*y1) +
			// k*(+x0*y1 - y0*x1 + z0*w1 + w0*z1) +
			// 1*(-x0*x1 - y0*y1 - z0*z1 + w0*w1)
			auto x = m_elements[0];
			auto y = m_elements[1];
			auto z = m_elements[2];
			auto w = m_elements[3];

			m_elements[0] = x * right.m_elements[3] + y * right.m_elements[2] - z * right.m_elements[1] + w * right.m_elements[0];
			m_elements[1] = -x * right.m_elements[2] + y * right.m_elements[3] + z * right.m_elements[0] + w * right.m_elements[1];
			m_elements[2] = x * right.m_elements[1] - y * right.m_elements[0] + z * right.m_elements[3] + w * right.m_elements[2];
			m_elements[3] = -x * right.m_elements[0] - y * right.m_elements[1] - z * right.m_elements[2] + w * right.m_elements[3];

			return *this;
		}

		Quaternion Quaternion::operator*(Quaternion const& right) const
		{
			auto result{ *this };

			result *= right;
			return result;
		}

		std::ostream& operator<<(std::ostream& out, Quaternion const& quat)
		{
			auto precision = out.precision();
			auto oldSetf = out.setf(std::ios_base::fixed, std::ios_base::floatfield);

			out.precision(6);
			out << "(" << quat.m_elements[0] << ", " << quat.m_elements[1] << ", " << quat.m_elements[2] << ", " << quat.m_elements[3] << ")";
			out.setf(oldSetf);
			out.precision(precision);
			return out;
		}
	}
}