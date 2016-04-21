#include "Math/EulerAngle.hpp"
#include "Math/Quaternion.hpp"
#include "Math/Vector4.hpp"
#include "Math/Point4.hpp"
#include "Math/Matrix.hpp"
#include <cmath>

#define PI CROISSANT_PI
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

		inline float Tan(float radian)
		{
			return std::tan(radian);
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
			//auto cr = Cos(ToRadian(angle.Roll()) / 2.0f);
			//auto cp = Cos(ToRadian(angle.Pitch()) / 2.0f);
			//auto cy = Cos(ToRadian(angle.Yaw()) / 2.0f);
			//auto sr = Sin(ToRadian(angle.Roll()) / 2.0f);
			//auto sp = Sin(ToRadian(angle.Pitch()) / 2.0f);
			//auto sy = Sin(ToRadian(angle.Yaw()) / 2.0f);

			//auto vec = Vector4{
			//	sy * sr * cp + cy * cr * sp,
			//	sy * cr * cp + cy * sr * sp,
			//	cy * sr * cp - sy * cr * sp
			//};

			//vec.MakeUnit();

			//return Quaternion{
			//	vec,
			//	cy * cr * cp - sy * sr * sp
			//};

			Quaternion rotX{ Vector4::UnitX , ToRadian(angle.Pitch()) };
			Quaternion rotY{ Vector4::UnitY , ToRadian(angle.Yaw()) };
			Quaternion rotZ{ Vector4::UnitZ , ToRadian(angle.Roll()) };

			return rotX * rotY * rotZ;

			//return Quaternion{
			//	cr * cp * cy + sr * sp * sy,
			//	cr * sp * cy + sr * cp * sy,
			//	cr * cp * sy - sr * sp * cy,
			//	sr * cp * cy - cr * sp * sy
			//};
		}

		inline Math::Matrix4f ToMatrix(Math::Vector4 const& vector)
		{
			Math::Matrix4f result;

			result.MakeIdentity();
			result(0, 3) = vector.X();
			result(1, 3) = vector.Y();
			result(2, 3) = vector.Z();
			//result(3, 0) = vector.X();
			//result(3, 1) = vector.Y();
			//result(3, 2) = vector.Z();
			return result;
		}

		Matrix4f ToMatrix(Quaternion const& quaternion)
		{
			Matrix4f res;

			res.MakeIdentity();
			auto length = quaternion.Length();
			float x = quaternion.X() / length;
			float y = quaternion.Y() / length;
			float z = quaternion.Z() / length;
			float w = quaternion.W() / length;

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

		template<typename Real> Real	Min(Real const& left, Real const& right)
		{
			return std::min(left, right);
		}

		template<typename Real> Real	Max(Real const& left, Real const& right)
		{
			return std::max(left, right);
		}

		inline Vector4	operator*(Vector4 const& vector, Matrix4f const& matrix)
		{
			auto transpose = matrix.GetTranspose();
			return Vector4{
					vector.X()*transpose(0, 0) + vector.Y()*transpose(1, 0) + vector.Z()*transpose(2, 0) + vector.W()*transpose(3, 0),
					vector.X()*transpose(0, 1) + vector.Y()*transpose(1, 1) + vector.Z()*transpose(2, 1) + vector.W()*transpose(3, 1),
					vector.X()*transpose(0, 2) + vector.Y()*transpose(1, 2) + vector.Z()*transpose(2, 2) + vector.W()*transpose(3, 2),
					vector.X()*transpose(0, 3) + vector.Y()*transpose(1, 3) + vector.Z()*transpose(2, 3) + vector.W()*transpose(3, 3)
			};
		}

		inline Point4		operator*(Point4 const& point, Matrix4f const& matrix)
		{
			// dans la multiplication P*M, la matrice doit être transposée
			// Le nombre de colonne de la matrice de gauche est �gal au nombre de ligne de celle de droite
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
			auto transpose = matrix.GetTranspose();
			return Point4{
					point.X()*transpose(0, 0) + point.Y()*transpose(1, 0) + point.Z()*transpose(2, 0) + point.W()*transpose(3, 0),
					point.X()*transpose(0, 1) + point.Y()*transpose(1, 1) + point.Z()*transpose(2, 1) + point.W()*transpose(3, 1),
					point.X()*transpose(0, 2) + point.Y()*transpose(1, 2) + point.Z()*transpose(2, 2) + point.W()*transpose(3, 2),
					point.X()*transpose(0, 3) + point.Y()*transpose(1, 3) + point.Z()*transpose(2, 3) + point.W()*transpose(3, 3) // should always be 0
			};
		}
	}
}

#undef PI
#undef PI_FOR_RAD
