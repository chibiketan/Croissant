#include "Math/Quaternion.hpp"
#include "Math/Vector4.hpp"
#include "Math/Matrix4.hpp"

namespace Croissant
{
	namespace Math
	{
		Quaternion::Quaternion(Vector4 const& vector, float angle)
			: m_elements { vector.X() * std::sin(angle / 2.0f), vector.Y() * std::sin(angle / 2.0f),
			vector.Z() * std::sin(angle / 2.0f), std::cos(angle / 2.0f) }
		{
		}

		Matrix4 Quaternion::ToMatrix() const
		{
			Matrix4 res;

			res.LoadIdentity();
			auto length = this->Length();
			float x = this->m_elements[0] / length;
			float y = this->m_elements[1] / length;
			float z = this->m_elements[2] / length;
			float w = this->m_elements[3] / length;

			float twoX = static_cast<float>(2)*x;
			float twoY = static_cast<float>(2)*y;
			float twoZ = static_cast<float>(2)*z;
			float twoXX = twoX*x;
			float twoXY = twoX*y;
			float twoXZ = twoX*z;
			float twoXW = twoX*w;
			float twoYY = twoY*y;
			float twoYZ = twoY*z;
			float twoYW = twoY*w;
			float twoZZ = twoZ*z;
			float twoZW = twoZ*w;

			//res(0, 0) = static_cast<float>(1) - twoYY - twoZZ;
			//res(0, 1) = twoXY - twoZW;
			//res(0, 2) = twoXZ + twoYW;
			//res(1, 0) = twoXY + twoZW;
			//res(1, 1) = static_cast<float>(1) - twoXX - twoZZ;
			//res(1, 2) = twoYZ - twoXW;
			//res(2, 0) = twoXZ - twoYW;
			//res(2, 1) = twoYZ + twoXW;
			//res(2, 2) = static_cast<float>(1) - twoXX - twoYY;
			res(0, 0) = static_cast<float>(1) - twoYY - twoZZ;
			res(0, 1) = twoXY - twoZW;
			res(0, 2) = twoXZ + twoYW;
			res(1, 0) = twoXY + twoZW;
			res(1, 1) = static_cast<float>(1) - twoXX - twoZZ;
			res(1, 2) = twoYZ + twoXW;
			res(2, 0) = twoXZ - twoYW;
			res(2, 1) = twoYZ - twoXW;
			res(2, 2) = static_cast<float>(1) - twoXX - twoYY;
			return res;
		}

		float Quaternion::Length() const
		{
			return std::sqrt(this->m_elements[0] * this->m_elements[0]
				+ this->m_elements[1] * this->m_elements[1]
				+ this->m_elements[2] * this->m_elements[2]
				+ this->m_elements[3] * this->m_elements[3]);
		}
	}
}