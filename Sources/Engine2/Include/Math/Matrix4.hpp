#ifndef CROISSANT_ENGINE_MATH_MATRIX_4_HPP_INC
#  define CROISSANT_ENGINE_MATH_MATRIX_4_HPP_INC
#  include "Engine.hpp"
#  include <array>

#define DIMENSION 4
#define ARRAY_SIZE 16

namespace Croissant
{
	namespace Math
	{
		class ENGINE_API Matrix4 final
		{
		public:

			Matrix4();
			explicit Matrix4(std::array<float, ARRAY_SIZE> const& source);
			std::array<float, ARRAY_SIZE> const&	Data() const;
			float									operator()(size_t row, size_t column) const;
			float&									operator()(size_t row, size_t column);
			void									LoadIdentity();
			void									LoadZero();
			Matrix4									operator*(Matrix4 const& right);

		private:
			std::array<float, ARRAY_SIZE>	m_data;
		};

	}
}

#undef  DIMENSION
#undef ARRAY_SIZE

#endif //CROISSANT_ENGINE_MATH_MATRIX_4_HPP_INC
