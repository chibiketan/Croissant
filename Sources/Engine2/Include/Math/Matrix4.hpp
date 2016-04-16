//#ifndef CROISSANT_ENGINE_MATH_MATRIX_4_HPP_INC
//#  define CROISSANT_ENGINE_MATH_MATRIX_4_HPP_INC
//#  include "Engine.hpp"
//#  include <array>
//
//#define DIMENSION 4
//#define ARRAY_SIZE 16
//
//namespace Croissant
//{
//	namespace Math
//	{
//		class Point4;
//
//		class ENGINE_API Matrix4 final
//		{
//		public:
//			using ValueType = float;
//			using ArrayType = std::array<ValueType, ARRAY_SIZE>;
//
////			Matrix4();
////			explicit Matrix4(ArrayType const& source);
//			ArrayType const&		Data() const;
//			ValueType				operator()(size_t row, size_t column) const;
//			ValueType&				operator()(size_t row, size_t column);
//			void					LoadIdentity();
//			void					LoadZero();
//			Matrix4					operator*(Matrix4 const& right) const;
//			bool					operator==(Matrix4 const& right) const;
//			ENGINE_API friend std::ostream&	operator<<(std::ostream& out, Matrix4 const& obj);
//			///<summary>Calcul du déterminant de la matrice carrée</summary>
//			ValueType				Determinant() const;
//
//		private:
//			ArrayType	m_data;
//		};
//
//	}
//}
//
//#undef  DIMENSION
//#undef ARRAY_SIZE
//
//#endif //CROISSANT_ENGINE_MATH_MATRIX_4_HPP_INC
