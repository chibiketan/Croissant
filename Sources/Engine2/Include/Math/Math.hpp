#ifndef CROISSANT_ENGINE_MATH_MATH_HPP_INC
#  define CROISSANT_ENGINE_MATH_MATH_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include "Math/Matrix.hpp"

namespace Croissant
{
	namespace Math
	{
		class Quaternion;
		class EulerAngle;
		class Vector4;
		class Point4;

		inline float			Sin(float radian);
		inline float			Cos(float radian);
		inline float			ToRadian(float degree);
		inline Quaternion		ToQuaternion(EulerAngle const& angle);
		inline Matrix4f	ToMatrix(Vector4 const& vector);
		inline Matrix4f	ToMatrix(Quaternion const& quaternion);
		template<typename Real> Real	Min(Real const& left, Real const& right);
		template<typename Real> Real	Max(Real const& left, Real const& right);


		inline Vector4		operator*(Vector4 const& vector, Matrix4f const& matrix);
		///<summary>Multiplication d'un point par une matrice qui donne un point</summary>
		inline Point4		operator*(Point4 const& point, Matrix4f const& matrix);
	}
}

#  include "Math/Math.inl"

#endif // !CROISSANT_ENGINE_MATH_MATH_HPP_INC