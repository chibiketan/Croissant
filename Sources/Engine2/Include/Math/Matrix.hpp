#ifndef CROISSANT_ENGINE_MATH_MATRIX_HPP_INC
#  define CROISSANT_ENGINE_MATH_MATRIX_HPP_INC
#  include <array>

namespace Croissant
{
	namespace Math
	{
		namespace _Internal
		{
			template<typename Number, size_t size, size_t index>
			struct MatrixHelper
			{
				inline static void InitializeToZero(std::array<Number, size * size> &data);
				inline static void InitializeToIdentity(std::array<Number, size * size> &data);
				constexpr static Number GetIndexValueForIdentity();
			};

			template<typename Number, size_t size>
			struct MatrixHelper<Number, size, 0>
			{
				inline static void InitializeToZero(std::array<Number, size * size> &data);
				inline static void Init(std::array<Number, size * size> &data);
				inline static void InitializeToIdentity(std::array<Number, size * size> &data);
				constexpr static Number GetIndexValueForIdentity();
			};
		}

		template<typename Number, size_t size>
		class Matrix final
		{
		public:
			Matrix();
			explicit Matrix(std::array<Number, size * size> const& source);
			Matrix(Matrix const&);
			Matrix(Matrix &&);
			Matrix& operator=(Matrix const&);
			Matrix& operator=(Matrix &&);
			std::array<Number, size * size> const&	Data() const;
			static Matrix Identity();

		private:
			std::array<Number, size * size>	m_data;
		};

		// --------------------------------------------------------------------------- _Internal::MatrixHelper
		template<typename Number, size_t size, size_t index>
		void _Internal::MatrixHelper<Number, size, index>::InitializeToZero(std::array<Number, size * size> &data)
		{
			MatrixHelper<Number, size, index - 1>::InitializeToZero(data);
			data[index] = 0;
		}

		template<typename Number, size_t size>
		void _Internal::MatrixHelper<Number, size, 0>::InitializeToZero(std::array<Number, size * size> &data)
		{
			data[0] = 0;
		}

		template <typename Number, size_t size>
		void _Internal::MatrixHelper<Number, size, 0>::Init(std::array<Number, size * size> &data)
		{
			data[0] = 0;
		}

		template <typename Number, size_t size, size_t index>
		constexpr Number _Internal::MatrixHelper<Number, size, index>::GetIndexValueForIdentity()
		{
			return (index % size == static_cast<int>(index / size) ? 1 : 0);;
		}

		template <typename Number, size_t size, size_t index>
		void _Internal::MatrixHelper<Number, size, index>::InitializeToIdentity(std::array<Number, size * size> &data)
		{
			MatrixHelper<Number, size, index - 1>::InitializeToIdentity(data);
			data[index] = GetIndexValueForIdentity();
		}

		template <typename Number, size_t size>
		void _Internal::MatrixHelper<Number, size, 0>::InitializeToIdentity(std::array<Number, size * size> &data)
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
		Matrix<Number, size>::Matrix(std::array<Number, size * size> const& source)
		{
			m_data = source;
		}

		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix(Matrix const& ref)
		{
			m_data = ref.m_data;
		}

		template <typename Number, size_t size>
		Matrix<Number, size>::Matrix(Matrix&& ref)
		{
			m_data = std::move(ref.m_data);
			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToZero(ref.m_data);
		}

		template <typename Number, size_t size>
		Matrix<Number, size>& Matrix<Number, size>::operator=(Matrix const& ref)
		{
			m_data = ref.m_data;
			return *this;
		}

		template <typename Number, size_t size>
		Matrix<Number, size>& Matrix<Number, size>::operator=(Matrix&& ref)
		{
			m_data = std::move(ref.m_data);
			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToZero(ref.m_data);
			return *this;
		}

		template <typename Number, size_t size>
		std::array<Number, size * size> const& Matrix<Number, size>::Data() const
		{
			return m_data;
		}

		template <typename Number, size_t size>
		Matrix<Number, size> Matrix<Number, size>::Identity()
		{
			std::array<Number, size * size> data;

			_Internal::MatrixHelper<Number, size, size * size - 1>::InitializeToIdentity(data);
			return Matrix(data);
		}

		// --------------------------------------------------------------------------- Using
		using Matrix4f = Matrix<float, 4>;
	}
}

#endif /* CROISSANT_ENGINE_MATH_MATRIX_HPP_INC */