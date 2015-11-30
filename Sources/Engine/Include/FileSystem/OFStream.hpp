/*
 * OFstream.hpp
 *
 *  Created on: 17 juil. 2013
 *      Author: gcompte
 */

#ifndef OFSTREAM_HPP_
#define OFSTREAM_HPP_

#include <fstream>
#include "Core/String.hpp"

using Croissant::Core::String;
using Croissant::Core::EncodingType;

namespace Croissant
{
	namespace FileSystem
	{

		class OFStream : public std::ofstream
		{
		public:
			OFStream(const String& filename, std::ios::openmode mode, EncodingType encoding = EncodingType::UTF8);
			virtual ~OFStream();
		};

	} /* namespace FileSystem */
} /* namespace Croissant */
#endif /* OFSTREAM_HPP_ */
