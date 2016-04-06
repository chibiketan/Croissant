#include "Math/EulerAngle.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector4.hpp"
#include <cmath>

#define PI 3.14159f
#define PI_FOR_RAD 0.0174532778f

namespace Croissant
{
	namespace Math
	{
		float Sin(float radian)
		{
			return std::sinf(radian);
		}

		float Cos(float radian)
		{
			return std::cosf(radian);
		}

		float ToRadian(float degree)
		{
			return degree * PI_FOR_RAD;
		}


		//template<typename T>
		//Quaternion<T> EulerAngles<T>::ToQuaternion() const
		//{
		//	T c1 = std::cos(ToRadians(yaw) / F(2.0));
		//	T c2 = std::cos(ToRadians(roll) / F(2.0));
		//	T c3 = std::cos(ToRadians(pitch) / F(2.0));

		//	T s1 = std::sin(ToRadians(yaw) / F(2.0));
		//	T s2 = std::sin(ToRadians(roll) / F(2.0));
		//	T s3 = std::sin(ToRadians(pitch) / F(2.0));

		//	return Quaternion<T>(c1 * c2 * c3 - s1 * s2 * s3,
		//		s1 * s2 * c3 + c1 * c2 * s3,
		//		s1 * c2 * c3 + c1 * s2 * s3,
		//		c1 * s2 * c3 - s1 * c2 * s3);
		//}

		Quaternion ToQuaternion(EulerAngle const& angle)
		{
			// W = cos(R / 2) * cos(P / 2) * cos(Y / 2) + sin(R /2) * sin(P / 2) * sin(Y / 2)
			// X = sin(R / 2) * cos(P / 2) * cos(Y / 2) - cos(R /2) * sin(P / 2) * sin(Y / 2)
			// Y = cos(R / 2) * sin(P / 2) * cos(Y / 2) + sin(R /2) * cos(P / 2) * sin(Y / 2)
			// Z = cos(R / 2) * cos(P / 2) * sin(Y / 2) - sin(R /2) * sin(P / 2) * cos(Y / 2)
			auto cr = Cos(ToRadian(angle.Roll()));
			auto cp = Cos(ToRadian(angle.Pitch()));
			auto cy = Cos(ToRadian(angle.Yaw()));
			auto sr = Sin(ToRadian(angle.Roll()));
			auto sp = Sin(ToRadian(angle.Pitch()));
			auto sy = Sin(ToRadian(angle.Yaw()));

			return Quaternion{
				Vector4{
					cr * sp * cy + sr * cp * sy,
					cr * cp * sy - sr * sp * cy,
					sr * cp * cy - cr * sp * sy
			},
				cr * cp * cy + sr * sp * sy
			};
		}

	}
}

#undef PI
#undef PI_FOR_RAD
