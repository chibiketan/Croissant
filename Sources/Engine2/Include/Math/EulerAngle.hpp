#ifndef CROISSANT_ENGINE_MATH_EULERANGLE_HPP_INC
#  define CROISSANT_ENGINE_MATH_EULERANGLE_HPP_INC

#  include "Engine.hpp"
#  include <ostream>

namespace Croissant
{
	namespace Math
	{
		/// <summary>
		/// Cette classe représente un ensemble de rotationappelé yaw pitch roll.
		/// <para>Les angles manipulés sont exprimés en degrés.</para>
		/// <para>Yaw : rotation autour de l'axe y (rotation d'une voiture)</para>
		/// <para>pitch : rotation autour de l'axe x (inclinaison du nez d'un avion)</para>
		/// <para>roll : rotation autour de l'axe z (axe de tonneau pour un avion)</para>
		/// <para>L'ordre des rotations est la suivantes : yaw * pitch * roll</para>
		/// </summary>
		class ENGINE_API EulerAngle
		{
		public:
			using type = float;
			EulerAngle(type yaw, type pitch, type roll);
			inline float Roll() const;
			inline float Pitch() const;
			inline float Yaw() const;

			friend std::ostream& operator<<(std::ostream& out, EulerAngle const& angle);
		private:
			type	m_yaw;
			type	m_pitch;
			type	m_roll;
		};

		inline float EulerAngle::Roll() const
		{
			return m_roll;
		}

		inline float EulerAngle::Pitch() const
		{
			return m_pitch;
		}

		inline float EulerAngle::Yaw() const
		{
			return m_yaw;
		}
	}
}




#endif // !CROISSANT_ENGINE_MATH_EULERANGLE_HPP_INC
