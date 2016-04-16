#ifndef CROISSANT_ENGINE_MATH_QUATERNION_HPP_INC
#  define CROISSANT_ENGINE_MATH_QUATERNION_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include <array>

namespace Croissant
{
	namespace Math
	{
		class Vector4;

		class ENGINE_API Quaternion
		{
		public:
			Quaternion(Vector4 const& vector, float angle);
			float	Length() const;
			inline float 	X() const;
			inline float	Y() const;
			inline float	Z() const;
			inline float	W() const;
			Quaternion&	operator*=(Quaternion const& right);
			Quaternion	operator*(Quaternion const& right) const;
			ENGINE_API friend std::ostream&	operator<<(std::ostream& out, Quaternion const& quat);
			
		private:
			std::array<float, 4>	m_elements;
		};
	}
}

namespace Croissant
{
	namespace Math
	{
		inline float Quaternion::X() const
		{
			return m_elements[0];
		}

		inline float Quaternion::Y() const
		{
			return m_elements[1];
		}

		inline float Quaternion::Z() const
		{
			return m_elements[2];
		}

		inline float Quaternion::W() const
		{
			return m_elements[3];
		}

	}
}

#endif //!CROISSANT_ENGINE_MATH_QUATERNION_HPP_INC