#ifndef CROISSANT_ENGINE_MATH_MATRIX_HPP_INC
#  define CROISSANT_ENGINE_MATH_MATRIX_HPP_INC
#  pragma once
#  include "Exception/CroissantException.hpp"
#  include <array>
#  include <iostream>

namespace Croissant {
	namespace Math {
		template<typename Number, size_t size>
		class Matrix final {
		public:
			using ArrayType = std::array<Number, size * size>;
			Matrix();

			explicit Matrix(std::array<Number, size * size> const &source);
			explicit Matrix(Number const source[size * size]);

			Matrix(Matrix const &);
			Matrix(Matrix &&);
			Number const* Data() const;
			static Matrix Identity();
			void 	MakeIdentity();
			Number Determinant() const;
			Matrix CofactorMatrix() const;
			Number	Cofactor(size_t row, size_t col) const;
			Matrix	GetTranspose() const;
			Matrix	GetAdjoint() const;
			Matrix	GetInverse() const;
			Matrix &operator=(Matrix const &);
			Matrix &operator=(Matrix &&);
			bool	operator==(Matrix const&) const;
			Matrix	operator*(Matrix const& right);
			Number const& operator()(size_t row, size_t col) const;
			Number& operator()(size_t row, size_t col);

		private:
			//std::array<Number, size * size> m_data;
			Number	m_data[size * size];
		};

		namespace _Internal {
			template<typename Number, size_t size, size_t index>
			struct MatrixHelper {
				inline static void InitializeToZero(Number data[size * size]);

				inline static void InitializeToIdentity(Number data[size * size]);

				constexpr static Number GetIndexValueForIdentity();
			};

			template<typename Number, size_t size>
			struct MatrixHelper<Number, size, 0> {
				inline static void InitializeToZero(Number data[size * size]);

				inline static void Init(Number data[size * size]);

				inline static void InitializeToIdentity(Number data[size * size]);

				constexpr static Number GetIndexValueForIdentity();
			};

			template<typename Number, size_t size>
			struct MatrixDeterminant
			{
				static Number Calculate(Matrix<Number, size> const &matrix);
			};

			template<typename Number>
			struct MatrixDeterminant<Number, 2>
			{
				static Number Calculate(Matrix<Number, 2> const& matrix);
			};
		}


		template<typename Number, size_t size>
		std::ostream &operator<<(std::ostream &out, Matrix<Number, size> const &matrix);
		template<typename Number, size_t size>
		Matrix<Number, size> operator*(Number const& scalar, Matrix<Number, size> const& matrix);
		template<typename Number, size_t size>
		Matrix<Number, size> operator*(Matrix<Number, size> const& matrix, Number const& scalar);

		// --------------------------------------------------------------------------- Using
		template class Matrix<float, 4>;
		using Matrix4f = Matrix<float, 4>;
	}
}

#  include "Math/Math.hpp"

namespace Croissant
{
	namespace Math
	{
		// --------------------------------------------------------------------------- _Internal::MatrixHelper
		template<typename Number, size_t size, size_t index>
		void _Internal::MatrixHelper<Number, size, index>::InitializeToZero(Number data[size * size])
		{
			MatrixHelper<Number, size, index - 1>::InitializeToZero(data);
			data[index] = 0;
		}

		template<typename Number, size_t size>
		void _Internal::MatrixHelper<Number, size, 0>::InitializeToZero(Number data[size * size])
		{
			data[0] = 0;
		}

		template <typename Number, size_t size>
		void _Internal::MatrixHelper<Number, size, 0>::Init(Number data[size * size])
		{
			data[0] = 0;
		}

		template <typename Number, size_t size, size_t index>
		constexpr Number _Internal::MatrixHelper<Number, size, index>::GetIndexValueForIdentity()
		{
			return (index % size == static_cast<int>(index / size) ? 1 : 0);;
		}

		template <typename Number, size_t size, size_t index>
		void _Internal::MatrixHelper<Number, size, index>::InitializeToIdentity(Number data[size * size])
		{
			MatrixHelper<Number, size, index - 1>::InitializeToIdentity(data);
			data[index] = GetIndexValueForIdentity();
		}

		template <typename Number, size_t size>
		void _Internal::MatrixHelper<Number, size, 0>::InitializeToIdentity(Number data[size * size])
		{
			data[0] = GetIndexValueForIdentity();
		}

		template <typename Number, size_t size>
		constexpr Number _Internal::MatrixHelper<Number, size, 0>::GetIndexValueForIdentity()
		{
			return 1;
		}

		template<typename Number, size_t size>
		Number _Internal::MatrixDeterminant<Number, size>::Calculate(Matrix<Number, size> const& matrix)
		{
			Number result = 0;
			Number tmp = Number { 0 };

			// première partie du calcul
			for (size_t col = 0; col < size; ++col)
			{
				auto tmpLoc = Number { 1 };
				size_t curCol = col;
				size_t curRow = 0;

				do {
					tmpLoc *= matrix(curRow, curCol);
					++curCol;
					++curRow;
					if (curCol >= size)
					{
						curCol = 0;
					}

				} while (curCol != col);
				tmp += tmpLoc;
			}

			result = tmp;

			// deuxième partie du calcul
			tmp = Number { 0 };
			for (auto col = static_cast<int>(size) - 1; col >= 0; --col)
			{
				auto tmpLoc = Number { 1 };
				auto curCol = col;
				auto curRow = 0;

				do {
					tmpLoc *= matrix(curRow, curCol);
					--curCol;
					++curRow;
					if (curCol < 0)
					{
						curCol = size - 1;
					}

				} while (curCol != col);
				tmp += tmpLoc;
			}

			result = result - tmp;
			return result;
		};

		template<typename Number>
		Number _Internal::MatrixDeterminant<Number, 2>::Calculate(Matrix<Number, 2> const& matrix)
		{
			return (matrix(0, 0) * matrix(1, 1)) - (matrix(0, 1) * matrix(1, 0));
		};

		// --------------------------------------------------------------------------- Matrix
		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix()
		{
			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToIdentity(m_data);
		}

		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix(std::array<Number, size * size> const& source)
		{
			std::memcpy(m_data, source.data(), source.size());
		}

		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix(const Number source[size * size])
		{
			std::memcpy(m_data, source, size * size);
		}

		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix(Matrix const& ref)
		{
			std::memcpy(m_data, ref.m_data, size * size);
		}

		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix(Matrix&& ref)
		{
			std::memcpy(m_data, ref.m_data, size * size);
			//m_data = std::move(ref.m_data);
			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToZero(ref.m_data);
		}

		template <typename Number, size_t size>
		Matrix<Number, size>& Matrix<Number, size>::operator=(Matrix const& ref)
		{
			std::memcpy(m_data, ref.m_data, size * size);
			return *this;
		}

		template <typename Number, size_t size>
		Matrix<Number, size>& Matrix<Number, size>::operator=(Matrix&& ref)
		{
			std::memcpy(m_data, ref.m_data, size * size);
			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToZero(ref.m_data);
			return *this;
		}

		template <typename Number, size_t size>
		bool Matrix<Number, size>::operator==(Matrix const& right) const
		{
			return m_data == right.m_data;
		}

		template <typename Number, size_t size>
		Number const* Matrix<Number, size>::Data() const
		{
			return m_data;
		}

		template <typename Number, size_t size>
		Matrix<Number, size> Matrix<Number, size>::Identity()
		{
			Number data[size * size];

			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToIdentity(data);
			return Matrix(data);
		}

		template <typename Number, size_t size>
		Number Matrix<Number, size>::Determinant() const {
			return _Internal::MatrixDeterminant<Number, size>::Calculate(*this);
		}

		template <typename Number, size_t size>
		Number const& Matrix<Number, size>::operator()(size_t row, size_t col) const
		{
			return m_data[row * size + col];
		}

		template <typename Number, size_t size>
		Number& Matrix<Number, size>::operator()(size_t row, size_t col)
		{
			return m_data[row * size + col];
		}

		// TODO -------------------------------- begin to refactor

		template<typename Number, size_t size>
		Matrix<Number, size - 1>	GetCofactorOfElement(Matrix<Number, size> const& matrix, size_t const row, size_t const col)
		{
			Matrix<Number, size - 1> result { Matrix<Number, size - 1>::Identity() };
			auto newRow = 0;

			for (size_t curRow = 0; curRow < size; ++curRow)
			{
				if (row == curRow)
				{
					// même ligne, on ne prend pas
					continue;
				}

				auto newCol = 0;

				for (size_t curCol = 0; curCol < size; ++curCol)
				{
					if (col == curCol)
					{
						// même colonne, on ne prend pas
						continue;
					}

					result(newRow, newCol) = matrix(curRow, curCol);
					++newCol;
				}

				++newRow;
			}

			return result;
		};

		// TODO -------------------------------- end to refactor

		template <typename Number, size_t size>
		Matrix<Number, size> Matrix<Number, size>::CofactorMatrix() const
		{
			Matrix<Number, size> result;

			for (size_t row = 0; row < size; ++row)
			{
				for (size_t col = 0; col < size; ++col)
				{
					result(row, col) = Cofactor(row, col);
				}
			}

			return result;
		}

		template <typename Number, size_t size>
		Number Matrix<Number, size>::Cofactor(size_t row, size_t col) const
		{
			auto result = GetCofactorOfElement(*this, row, col).Determinant();

			if (Number { 0 } == result)
			{
				return result;
			}

			return result * (((row + col) % 2) == 0 ? 1 : -1);
		}

		template <typename Number, size_t size>
		void Matrix<Number, size>::MakeIdentity()
		{
			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToIdentity(m_data);
		}

		template <typename Number, size_t size>
		Matrix<Number, size> Matrix<Number, size>::GetTranspose() const
		{
			Matrix<Number, size> result;

			for (size_t i = 0; i < size; ++i)
			{
				result(i, i) = (*this)(i, i);
				for (size_t col = i -1; col < size; --col)
				{
					result(i, col) = (*this)(col, i);
					result(col, i) = (*this)(i, col);
				}
			}

			return result;
		}

		template <typename Number, size_t size>
		Matrix<Number, size> Matrix<Number, size>::GetAdjoint() const {
			return CofactorMatrix().GetTranspose();
		}

		template <typename Number, size_t size>
		Matrix<Number, size> Matrix<Number, size>::GetInverse() const {
			auto determinant = Determinant();

			if (determinant == Number { 0 })
			{
				throw Croissant::Exception::CroissantException { "Le determinant de la matrice vaut 0, impossible de calculer la matrice inverse" };
			}

			auto invDeterminant = Number { 1 } / determinant;
			auto adjointMatrix = GetAdjoint();
			return invDeterminant * adjointMatrix;
		}

		template<typename Number, size_t size>
		Matrix<Number, size> Matrix<Number, size>::operator*(const Matrix& right)
		{
			// TODO optimisation du calcul par les templates et le preprocesseur ?
			auto& rthis = *this;
			Matrix4f result;

			for (size_t row = 0; row < size; ++row)
			{
				for (size_t col = 0; col < size; ++col)
				{
					Number val { 0 };

					for (size_t i = 0; i < size; ++i)
					{
						val = val + (rthis(row, i) * right(i, col));
					}

					result(row, col) = val;
				}
			}

//			auto result = Matrix4f(std::array<Number, size * size> {
//				// first line
//				rthis(0, 0) * right(0, 0) + rthis(0, 1) * right(1, 0) + rthis(0, 2) * right(2, 0) + rthis(0, 3) * right(3, 0),
//				rthis(0, 0) * right(0, 1) + rthis(0, 1) * right(1, 1) + rthis(0, 2) * right(2, 1) + rthis(0, 3) * right(3, 1),
//				rthis(0, 0) * right(0, 2) + rthis(0, 1) * right(1, 2) + rthis(0, 2) * right(2, 2) + rthis(0, 3) * right(3, 2),
//				rthis(0, 0) * right(0, 3) + rthis(0, 1) * right(1, 3) + rthis(0, 2) * right(2, 3) + rthis(0, 3) * right(3, 3),
//				// second line
//				rthis(1, 0) * right(0, 0) + rthis(1, 1) * right(1, 0) + rthis(1, 2) * right(2, 0) + rthis(1, 3) * right(3, 0),
//				rthis(1, 0) * right(0, 1) + rthis(1, 1) * right(1, 1) + rthis(1, 2) * right(2, 1) + rthis(1, 3) * right(3, 1),
//				rthis(1, 0) * right(0, 2) + rthis(1, 1) * right(1, 2) + rthis(1, 2) * right(2, 2) + rthis(1, 3) * right(3, 2),
//				rthis(1, 0) * right(0, 3) + rthis(1, 1) * right(1, 3) + rthis(1, 2) * right(2, 3) + rthis(1, 3) * right(3, 3),
//				// third line
//				rthis(2, 0) * right(0, 0) + rthis(2, 1) * right(1, 0) + rthis(2, 2) * right(2, 0) + rthis(2, 3) * right(3, 0),
//				rthis(2, 0) * right(0, 1) + rthis(2, 1) * right(1, 1) + rthis(2, 2) * right(2, 1) + rthis(2, 3) * right(3, 1),
//				rthis(2, 0) * right(0, 2) + rthis(2, 1) * right(1, 2) + rthis(2, 2) * right(2, 2) + rthis(2, 3) * right(3, 2),
//				rthis(2, 0) * right(0, 3) + rthis(2, 1) * right(1, 3) + rthis(2, 2) * right(2, 3) + rthis(2, 3) * right(3, 3),
//				// fourth line
//				rthis(3, 0) * right(0, 0) + rthis(3, 1) * right(1, 0) + rthis(3, 2) * right(2, 0) + rthis(3, 3) * right(3, 0),
//				rthis(3, 0) * right(0, 1) + rthis(3, 1) * right(1, 1) + rthis(3, 2) * right(2, 1) + rthis(3, 3) * right(3, 1),
//				rthis(3, 0) * right(0, 2) + rthis(3, 1) * right(1, 2) + rthis(3, 2) * right(2, 2) + rthis(3, 3) * right(3, 2),
//				rthis(3, 0) * right(0, 3) + rthis(3, 1) * right(1, 3) + rthis(3, 2) * right(2, 3) + rthis(3, 3) * right(3, 3),
//			});

			return result;
		}


		template<typename Number, size_t size>
		std::ostream &operator<<(std::ostream &out, Matrix<Number, size> const& matrix) {
			out << "{\r\n";
			for (size_t row = 0; row < size; ++row)
			{
				out << "  {";
				for (size_t col = 0; col < size; ++col)
				{
					if (col != 0)
					{
						out << ", ";
					}

					out << matrix(row, col);
				}

				out << "}\r\n";
			}

			out << "}";
			return out;
		}

		template<typename Number, size_t size>
		Matrix<Number, size> operator*(Number const& scalar, Matrix<Number, size> const& matrix)
		{
			Matrix<Number, size> result;

			for (size_t row = 0; row < size; ++row)
			{
				for (size_t col = 0; col < size; ++col)
				{
					result(row, col) = matrix(row, col) * scalar;
				}
			}

			return result;
		};

		template<typename Number, size_t size>
		Matrix<Number, size> operator*(Matrix<Number, size> const& matrix, Number const& scalar)
		{
			return scalar * matrix;
		};

	}
}

#endif /* CROISSANT_ENGINE_MATH_MATRIX_HPP_INC */