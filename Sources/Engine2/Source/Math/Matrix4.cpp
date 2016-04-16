#include "Math/Matrix4.hpp"
#include "Math/Point4.hpp"
#include <assert.h>
#include <iostream>

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

		Matrix4::Matrix4(Matrix4::ArrayType const& source)
			: m_data(source)
		{
		}

		Matrix4::ArrayType const& Matrix4::Data() const
		{
			return m_data;
		}

		Matrix4::ValueType Matrix4::operator()(size_t row, size_t column) const
		{
			assert(row < DIMENSION);
			assert(column < DIMENSION);
			return m_data[row * DIMENSION + column];
		}

		Matrix4::ValueType& Matrix4::operator()(size_t row, size_t column)
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
			// TODO: ne pas utiliser l'op�rateur () mais plut�t les indexes de m_data
			auto result = Matrix4({
				// first line
				rthis(0, 0) * right(0, 0) + rthis(0, 1) * right(1, 0) + rthis(0, 2) * right(2, 0) + rthis(0, 3) * right(3, 0),
				rthis(0, 0) * right(0, 1) + rthis(0, 1) * right(1, 1) + rthis(0, 2) * right(2, 1) + rthis(0, 3) * right(3, 1),
				rthis(0, 0) * right(0, 2) + rthis(0, 1) * right(1, 2) + rthis(0, 2) * right(2, 2) + rthis(0, 3) * right(3, 2),
				rthis(0, 0) * right(0, 3) + rthis(0, 1) * right(1, 3) + rthis(0, 2) * right(2, 3) + rthis(0, 3) * right(3, 3),
				// second line
				rthis(1, 0) * right(0, 0) + rthis(1, 1) * right(1, 0) + rthis(1, 2) * right(2, 0) + rthis(1, 3) * right(3, 0),
				rthis(1, 0) * right(0, 1) + rthis(1, 1) * right(1, 1) + rthis(1, 2) * right(2, 1) + rthis(1, 3) * right(3, 1),
				rthis(1, 0) * right(0, 2) + rthis(1, 1) * right(1, 2) + rthis(1, 2) * right(2, 2) + rthis(1, 3) * right(3, 2),
				rthis(1, 0) * right(0, 3) + rthis(1, 1) * right(1, 3) + rthis(1, 2) * right(2, 3) + rthis(1, 3) * right(3, 3),
				// third line
				rthis(2, 0) * right(0, 0) + rthis(2, 1) * right(1, 0) + rthis(2, 2) * right(2, 0) + rthis(2, 3) * right(3, 0),
				rthis(2, 0) * right(0, 1) + rthis(2, 1) * right(1, 1) + rthis(2, 2) * right(2, 1) + rthis(2, 3) * right(3, 1),
				rthis(2, 0) * right(0, 2) + rthis(2, 1) * right(1, 2) + rthis(2, 2) * right(2, 2) + rthis(2, 3) * right(3, 2),
				rthis(2, 0) * right(0, 3) + rthis(2, 1) * right(1, 3) + rthis(2, 2) * right(2, 3) + rthis(2, 3) * right(3, 3),
				// fourth line
				rthis(3, 0) * right(0, 0) + rthis(3, 1) * right(1, 0) + rthis(3, 2) * right(2, 0) + rthis(3, 3) * right(3, 0),
				rthis(3, 0) * right(0, 1) + rthis(3, 1) * right(1, 1) + rthis(3, 2) * right(2, 1) + rthis(3, 3) * right(3, 1),
				rthis(3, 0) * right(0, 2) + rthis(3, 1) * right(1, 2) + rthis(3, 2) * right(2, 2) + rthis(3, 3) * right(3, 2),
				rthis(3, 0) * right(0, 3) + rthis(3, 1) * right(1, 3) + rthis(3, 2) * right(2, 3) + rthis(3, 3) * right(3, 3),
			});

			return result;
		}

		bool Matrix4::operator==(Matrix4 const& right) const
		{
			return m_data == right.m_data;
		}

		std::ostream& operator<<(std::ostream& out, Matrix4 const& obj)
		{
			auto precision = out.precision();
			auto oldSetf = out.setf(std::ios_base::fixed, std::ios_base::floatfield);

			out.precision(2);
			out << "[\n"
				<< "  [" << obj(0, 0) << ", " << obj(0, 1) << ", " << obj(0, 2) << ", " << obj(0, 3) << "]\n"
				<< "  [" << obj(1, 0) << ", " << obj(1, 1) << ", " << obj(1, 2) << ", " << obj(1, 3) << "]\n"
				<< "  [" << obj(2, 0) << ", " << obj(2, 1) << ", " << obj(2, 2) << ", " << obj(2, 3) << "]\n"
				<< "  [" << obj(3, 0) << ", " << obj(3, 1) << ", " << obj(3, 2) << ", " << obj(3, 3) << "]\n"
				<< "]";
			out.setf(oldSetf);
			out.precision(precision);
			return out;
		}

		Matrix4::ValueType Matrix4::Determinant() const {
			return (
				(
						(*this)(0, 0) * (*this)(1, 1) * (*this)(2, 2) * (*this)(3, 3) +
						(*this)(0, 1) * (*this)(1, 2) * (*this)(2, 3) * (*this)(3, 0) +
						(*this)(0, 2) * (*this)(1, 3) * (*this)(2, 0) * (*this)(3, 1) +
						(*this)(0, 3) * (*this)(1, 0) * (*this)(2, 1) * (*this)(3, 2)
				) -
				(
						(*this)(3, 0) * (*this)(2, 1) * (*this)(1, 2) * (*this)(0, 3) +
						(*this)(2, 0) * (*this)(1, 1) * (*this)(0, 2) * (*this)(3, 3) +
						(*this)(1, 0) * (*this)(0, 1) * (*this)(3, 2) * (*this)(2, 3) +
						(*this)(0, 0) * (*this)(3, 1) * (*this)(2, 2) * (*this)(1, 3)
				)
			);
		}


	}
}