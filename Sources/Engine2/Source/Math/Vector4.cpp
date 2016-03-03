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

		Vector4::const_reference Vector4::GetX() const
		{
			return m_components.at(0);
		}

		Vector4::const_reference Vector4::GetY() const
		{
			return m_components.at(1);
		}

		Vector4::const_reference Vector4::GetZ() const
		{
			return m_components.at(2);
		}

		Vector4::const_reference Vector4::GetW() const
		{
			return m_components.at(3);
		}

	}
}
