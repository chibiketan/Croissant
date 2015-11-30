/*
 * Point.hpp
 *
 *  Created on: 10 sept. 2014
 *      Author: Gregory_Compte
 */

#ifndef POINT_HPP_INC
#define POINT_HPP_INC

#include "Engine.hpp"

namespace Croissant
{
	namespace Math
	{
		template <typename T>
		class ENGINE_API Point
		{
		public:
			Point(T x, T y, T z);
			T X() const;
			T Y() const;
			T Z() const;
			void X(T x);
			void Y(T y);
			void Z(T z);

		private:
			T m_pos[3];
		};

		// ---------------------------------------------- implémentation
		template <typename T>
		Point<T>::Point(T x, T y, T z)
			: m_pos {x, y, z}
		{
		}

		template <typename T>
		T Point<T>::X() const
		{
			return m_pos[0];
		}

		template <typename T>
		T Point<T>::Y() const
		{
			return m_pos[1];
		}

		template <typename T>
		T Point<T>::Z() const
		{
			return m_pos[2];
		}

		template <typename T>
		void Point<T>::X(T x)
		{
			m_pos[0] = x;
		}

		template <typename T>
		void Point<T>::Y(T y)
		{
			m_pos[1] = y;
		}

		template <typename T>
		void Point<T>::Z(T z)
		{
			m_pos[2] = z;
		}

		// ---------------------------------------------- alias
		using Pointf = Point<float>;
		using Pointd = Point<double>;
		using Pointi = Point<int>;
	}
}

#endif /* POINT_HPP_INC */
