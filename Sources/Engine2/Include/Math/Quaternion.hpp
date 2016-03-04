#ifndef CROISSANT_ENGINE_MATH_QUATERNION_HPP_INC
#  define CROISSANT_ENGINE_MATH_QUATERNION_HPP_INC

#  include "Engine.hpp"
#include <array>

namespace Croissant
{
	namespace Math
	{
		class Vector4;
		class Matrix4;

		class ENGINE_API Quaternion
		{
		public:
			Quaternion(Vector4 const& vector, float angle);
			Matrix4	ToMatrix() const;
			float	Length() const;
			
		private:
			std::array<float, 4>	m_elements;
		};
	}
}

#endif //!CROISSANT_ENGINE_MATH_QUATERNION_HPP_INC