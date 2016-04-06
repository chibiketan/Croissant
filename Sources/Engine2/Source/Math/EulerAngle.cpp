#include "Math/EulerAngle.hpp"
#include <ostream>

namespace Croissant
{
	namespace Math
	{
		EulerAngle::EulerAngle(type yaw, type pitch, type roll)
			: m_yaw{ yaw }, m_pitch{ pitch }, m_roll{ roll }
		{
		}

		std::ostream& operator<<(std::ostream& out, EulerAngle const& angle)
		{
			out << "EulerAngle[Y=" << angle.Yaw() << ", P=" << angle.Pitch() << ", R=" << angle.Roll() << "]";
			return out;
		}


	}
}
