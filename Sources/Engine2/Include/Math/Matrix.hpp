#ifndef CROISSANT_ENGINE_MATH_HPP_INC
#  define CROISSANT_ENGINE_MATH_HPP_INC

namespace Croissant
{
	namespace Math
	{
		namespace _Internal
		{
			template<typename Number, size_t size, size_t index>
			struct MatrixHelper
			{
				inline static void InitializeToZero(Number data[size * size]);
				inline static void InitializeToIdentity(Number data[size * size]);
				constexpr static Number GetIndexValueForIdentity();
			};

			template<typename Number, size_t size>
			struct MatrixHelper<Number, size, 0>
			{
				inline static void Init(Number data[size * size]);
				inline static void InitializeToIdentity(Number data[size * size]);
				constexpr static Number GetIndexValueForIdentity();
			};
		}

		template<typename Number, size_t size>
		class Matrix sealed
		{
		public:
			Matrix();
			explicit Matrix(Number const source[size * size]);
			Matrix(Matrix const&);
			Matrix(Matrix &&);
			Matrix& operator=(Matrix const&);
			Matrix& operator=(Matrix &&);
			static Matrix Identity();

		private:
			Number	m_data[size * size];
		};

		// --------------------------------------------------------------------------- _Internal::MatrixHelper
		template<typename Number, size_t size, size_t index>
		void _Internal::MatrixHelper<Number, size, index>::InitializeToZero(Number data[size * size])
		{
			MatrixHelper<Number, size, index - 1>::InitializeToZero(data);
			data[index] = 0;
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

		// --------------------------------------------------------------------------- Matrix
		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix()
		{
			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToZero(m_data);
		}

		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix(const Number source[size * size])
		{
			::memcpy(m_data, source, size * size * sizeof(Number));
		}

		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix(Matrix const&)
		{
			::memcpy(m_data, source, size * size * sizeof(Number));
		}

		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix(Matrix&& ref)
		{
			::memcpy(m_data, ref.m_data, size * size * sizeof(Number));
			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToZero(ref.m_data);
		}

		template <typename Number, size_t size>
		Matrix<Number, size>& Matrix<Number, size>::operator=(Matrix const&)
		{
			::memcpy(m_data, ref.m_data, size * size * sizeof(Number));
			return *this;
		}

		template <typename Number, size_t size>
		Matrix<Number, size>& Matrix<Number, size>::operator=(Matrix&& ref)
		{
			::memcpy(m_data, ref.m_data, size * size * sizeof(Number));
			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToZero(ref.m_data);
			return *this;
		}

		template <typename Number, size_t size>
		Matrix<Number, size> Matrix<Number, size>::Identity()
		{
			Number data[size * size];

			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToIdentity(data);
			return Matrix(data);
		}

		// --------------------------------------------------------------------------- Using
		using Matrix4f = Matrix<float, 4>;
	}
}

#endif /* CROISSANT_ENGINE_MATH_HPP_INC */