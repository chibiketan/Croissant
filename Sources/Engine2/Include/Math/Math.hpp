#ifndef CROISSANT_ENGINE_MATH_MATH_HPP_INC
#  define CROISSANT_ENGINE_MATH_MATH_HPP_INC
#  pragma once

#  include "Engine.hpp"

namespace Croissant
{
	namespace Math
	{
		class Quaternion;
		class EulerAngle;

		inline float		Sin(float radian);
		inline float		Cos(float radian);
		inline float		ToRadian(float degree);
		inline Quaternion	ToQuaternion(EulerAngle const& angle);

	}
}

#  include "Math/Math.inl"

#endif // !CROISSANT_ENGINE_MATH_MATH_HPP_INC