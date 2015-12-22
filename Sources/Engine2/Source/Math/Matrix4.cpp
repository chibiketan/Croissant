#include "Math/Matrix4.hpp"
#include <assert.h>

#define DIMENSION 4


namespace Croissant
{
	namespace Math
	{
		namespace
		{
		}

		Matrix4::Matrix4()
			: m_data({	1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f})
		{
		}

		Matrix4::Matrix4(std::array<float, 16> const& source)
			: m_data(source)
		{
		}

		std::array<float, 16> const& Matrix4::Data() const
		{
			return m_data;
		}

		float Matrix4::operator()(size_t row, size_t column) const
		{
			assert(row < DIMENSION);
			assert(column < DIMENSION);
			return m_data[row * DIMENSION + column];
		}

		float& Matrix4::operator()(size_t row, size_t column)
		{
			assert(row < DIMENSION);
			assert(column < DIMENSION);
			return m_data[row * DIMENSION + column];
		}

		void Matrix4::LoadIdentity()
		{
			m_data = {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}

		void Matrix4::LoadZero()
		{
			m_data = {
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f
			};
		}

		Matrix4 Matrix4::operator*(Matrix4 const& right) const
		{
			auto& rthis = *this;
			// TODO: ne pas utiliser l'opérateur () mais plutôt les indexes de m_data
			auto result = Matrix4({
				// first line
				rthis(0, 0) * right(0, 0) + rthis(1, 0) * right(0, 1) + rthis(2, 0) * right(0, 2) + rthis(3, 0) * right(0, 3),
				rthis(0, 1) * right(0, 0) + rthis(1, 1) * right(0, 1) + rthis(2, 1) * right(0, 2) + rthis(3, 1) * right(0, 3),
				rthis(0, 2) * right(0, 0) + rthis(1, 2) * right(0, 1) + rthis(2, 2) * right(0, 2) + rthis(3, 2) * right(0, 3),
				rthis(0, 3) * right(0, 0) + rthis(1, 3) * right(0, 1) + rthis(2, 3) * right(0, 2) + rthis(3, 3) * right(0, 3),
				// second line
				rthis(0, 0) * right(1, 0) + rthis(1, 0) * right(1, 1) + rthis(2, 0) * right(1, 2) + rthis(3, 0) * right(1, 3),
				rthis(0, 1) * right(1, 0) + rthis(1, 1) * right(1, 1) + rthis(2, 1) * right(1, 2) + rthis(3, 1) * right(1, 3),
				rthis(0, 2) * right(1, 0) + rthis(1, 2) * right(1, 1) + rthis(2, 2) * right(1, 2) + rthis(3, 2) * right(1, 3),
				rthis(0, 3) * right(1, 0) + rthis(1, 3) * right(1, 1) + rthis(2, 3) * right(1, 2) + rthis(3, 3) * right(1, 3),
				// third line
				rthis(0, 0) * right(2, 0) + rthis(1, 0) * right(2, 1) + rthis(2, 0) * right(2, 2) + rthis(3, 0) * right(2, 3),
				rthis(0, 1) * right(2, 0) + rthis(1, 1) * right(2, 1) + rthis(2, 1) * right(2, 2) + rthis(3, 1) * right(2, 3),
				rthis(0, 2) * right(2, 0) + rthis(1, 2) * right(2, 1) + rthis(2, 2) * right(2, 2) + rthis(3, 2) * right(2, 3),
				rthis(0, 3) * right(2, 0) + rthis(1, 3) * right(2, 1) + rthis(2, 3) * right(2, 2) + rthis(3, 3) * right(2, 3),
				// fourth line
				rthis(0, 0) * right(3, 0) + rthis(1, 0) * right(3, 1) + rthis(2, 0) * right(3, 2) + rthis(3, 0) * right(3, 3),
				rthis(0, 1) * right(3, 0) + rthis(1, 1) * right(3, 1) + rthis(2, 1) * right(3, 2) + rthis(3, 1) * right(3, 3),
				rthis(0, 2) * right(3, 0) + rthis(1, 2) * right(3, 1) + rthis(2, 2) * right(3, 2) + rthis(3, 2) * right(3, 3),
				rthis(0, 3) * right(3, 0) + rthis(1, 3) * right(3, 1) + rthis(2, 3) * right(3, 2) + rthis(3, 3) * right(3, 3),
			});

			return result;
		}

		bool Matrix4::operator==(Matrix4 const& right) const
		{
			return m_data == right.m_data;
		}
	}
}