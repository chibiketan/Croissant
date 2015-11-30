#ifndef STRINGCONVERSION_HPP_
#define STRINGCONVERSION_HPP_
#pragma once

#include <sstream>
#include <iostream>

namespace Croissant
{
	namespace Core
	{
		template<typename TReturn>
		TReturn fromString(const std::string& str)
		{
			std::istringstream stream(str);
			TReturn tmp;

			stream >> tmp;
			return tmp;
		}

		template<typename TRef>
		std::string toString(const TRef& t, int precision = 15)
		{
			std::ostringstream stream;

			stream.precision(precision);
			stream << t;
			return stream.str();
		}
	}
}

#endif
