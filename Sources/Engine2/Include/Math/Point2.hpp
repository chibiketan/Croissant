#ifndef CROISSANT_ENGINE_MATH_POINT2_HPP_INC
#  define CROISSANT_ENGINE_MATH_POINT2_HPP_INC

#  include "Engine.hpp"
#  include "Math/Tuple.hpp"

namespace Croissant
{
	namespace Math
	{
		class ENGINE_API Point2 : Tuple<float, 2>
		{
		public:
			Point2(type x, type y);
			using Tuple<float, 2>::X;
			using Tuple<float, 2>::Y;
		};
	}
}


#endif // !CROISSANT_ENGINE_MATH_POINT2_HPP_INC