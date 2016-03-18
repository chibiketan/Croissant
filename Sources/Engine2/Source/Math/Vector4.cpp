#include "Math/Vector4.hpp"

namespace Croissant
{
	namespace Math
	{
		Vector4::Vector4(type x, type y, type z, type w)
		{
			this->m_components[0] = x;
			this->m_components[1] = y;
			this->m_components[2] = z;
			this->m_components[3] = w;
		}

		Vector4::const_reference Vector4::X() const
		{
			return m_components.at(0);
		}

		Vector4::const_reference Vector4::Y() const
		{
			return m_components.at(1);
		}

		Vector4::const_reference Vector4::Z() const
		{
			return m_components.at(2);
		}

		Vector4::const_reference Vector4::W() const
		{
			return m_components.at(3);
		}

		Vector4::type Vector4::Length() const
		{
			return std::sqrt(this->m_components[0] * this->m_components[0]
				+ this->m_components[1] * this->m_components[1]
				+ this->m_components[2] * this->m_components[2]
				+ this->m_components[3] * this->m_components[3]);
		}

		void Vector4::MakeUnit()
		{
			auto len = this->Length();

			if (len != 1.0f)
			{
				auto invLen = 1.0f / len;
				m_components[0] *= invLen;
				m_components[1] *= invLen;
				m_components[2] *= invLen;
				m_components[3] *= invLen;
			}
		}
	}
}
