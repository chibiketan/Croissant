#ifndef CROISSANT_ENGINE_MATH_VECTOR4_HPP_INC
#define CROISSANT_ENGINE_MATH_VECTOR4_HPP_INC

#include "Engine.hpp"
#include <array>
#include "Quaternion.hpp"

namespace Croissant
{
	namespace Math
	{
		class Matrix4;
		class Point4;

		class ENGINE_API Vector4
		{
		public:
			using type = float;
			using reference = type&;
			using const_reference = type const&;
			Vector4(type x, type y, type z, type w = 0.0f);// composant W d'un vecteur vaut 0
			const_reference	X() const;
			const_reference	Y() const;
			const_reference	Z() const;
			const_reference	W() const;
			type			Length() const;
			void			MakeUnit();
			type			DotProduct(Vector4 const& v) const;
			type			DotProduct(Point4 const& p) const;
			Vector4		operator*(Matrix4 const& right) const;
			Vector4		operator-() const;
			Vector4		operator*(type val) const;
			static Vector4 const	UnitX;
			static Vector4 const	UnitY;
			static Vector4 const	UnitZ;
			static Vector4 const	Zero;

		private:
			std::array<type, 4>	m_components;
		};
	}
}

#endif //CROISSANT_ENGINE_MATH_VECTOR4_HPP_INC
