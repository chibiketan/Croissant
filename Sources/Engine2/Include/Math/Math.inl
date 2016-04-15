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
		inline float Sin(float radian)
		{
			return std::sin(radian);
		}

		inline float Cos(float radian)
		{
			return std::cos(radian);
		}

		inline float ToRadian(float degree)
		{
			return degree * PI_FOR_RAD;
		}

		inline Quaternion ToQuaternion(EulerAngle const& angle)
		{
			// W = cos(R / 2) * cos(P / 2) * cos(Y / 2) + sin(R /2) * sin(P / 2) * sin(Y / 2)
			// X = sin(R / 2) * cos(P / 2) * cos(Y / 2) - cos(R /2) * sin(P / 2) * sin(Y / 2)
			// Y = cos(R / 2) * sin(P / 2) * cos(Y / 2) + sin(R /2) * cos(P / 2) * sin(Y / 2)
			// Z = cos(R / 2) * cos(P / 2) * sin(Y / 2) - sin(R /2) * sin(P / 2) * cos(Y / 2)
			auto cr = Cos(ToRadian(angle.Roll()) / 2.0f);
			auto cp = Cos(ToRadian(angle.Pitch()) / 2.0f);
			auto cy = Cos(ToRadian(angle.Yaw()) / 2.0f);
			auto sr = Sin(ToRadian(angle.Roll()) / 2.0f);
			auto sp = Sin(ToRadian(angle.Pitch()) / 2.0f);
			auto sy = Sin(ToRadian(angle.Yaw()) / 2.0f);

			//return Quaternion{
			//	Vector4{
			//		sy * sr * cp + cy * cr * sp,
			//		sy * cr * cp + cy * sr * sp,
			//		cy * sr * cp - sy * cr * sp
			//	},
			//	cy * cr * cp - sy * sr * sp
			//};
			return Quaternion{
				Vector4{
					cr * sp * cy + sr * cp * sy,
					cr * cp * sy - sr * sp * cy,
					sr * cp * cy - cr * sp * sy
			},
				cr * cp * cy + sr * sp * sy
			};
		}

		inline Math::Matrix4 ToMatrix(Math::Vector4 const& vector)
		{
			Math::Matrix4 result;

			result.LoadIdentity();
			result(0, 3) = vector.X();
			result(1, 3) = vector.Y();
			result(2, 3) = vector.Z();
			//result(3, 0) = vector.X();
			//result(3, 1) = vector.Y();
			//result(3, 2) = vector.Z();
			return result;
		}


	}
}

#undef PI
#undef PI_FOR_RAD
