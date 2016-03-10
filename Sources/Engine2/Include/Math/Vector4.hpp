#ifndef CROISSANT_ENGINE_MATH_VECTOR4_HPP_INC
#define CROISSANT_ENGINE_MATH_VECTOR4_HPP_INC

#include "Engine.hpp"
#include <array>

namespace Croissant
{
	namespace Math
	{
		class ENGINE_API Vector4
		{
		public:
			using type = float;
			using reference = type&;
			using const_reference = type const&;
			Vector4(type x, type y, type z, type w = 0.0f);// composant W d'un vecteur vaut 0
			const_reference	GetX() const;
			const_reference	GetY() const;
			const_reference	GetZ() const;
			const_reference	GetW() const;
			type			Length() const;
			void			MakeUnit();

		private:
			std::array<type, 4>	m_components;
		};
	}
}

#endif //CROISSANT_ENGINE_MATH_VECTOR4_HPP_INC
