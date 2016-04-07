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
			return std::sin(radian);
		}

		float Cos(float radian)
		{
			return std::cos(radian);
		}

		float ToRadian(float degree)
		{
			return degree * PI_FOR_RAD;
		}


		//template<typename T>
		//Quaternion<T> EulerAngles<T>::ToQuaternion() const
		//{
		//	T cy = std::cos(ToRadians(yaw) / F(2.0));
		//	T cr = std::cos(ToRadians(roll) / F(2.0));
		//	T cp = std::cos(ToRadians(pitch) / F(2.0));

		//	T sy = std::sin(ToRadians(yaw) / F(2.0));
		//	T sr = std::sin(ToRadians(roll) / F(2.0));
		//	T sp = std::sin(ToRadians(pitch) / F(2.0));

		//	return Quaternion<T>(,
		//		,
		//		,
		//		);
		//}

		Quaternion ToQuaternion(EulerAngle const& angle)
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

	}
}

#undef PI
#undef PI_FOR_RAD
