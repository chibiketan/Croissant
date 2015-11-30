/*
 * OFstream.cpp
 *
 *  Created on: 17 juil. 2013
 *      Author: gcompte
 */

#include "FileSystem/OFStream.hpp"
#include "Exception/NotImplementedException.hpp"

namespace Croissant
{
	namespace FileSystem
	{

		OFStream::OFStream(const String& filename, std::ios::openmode mode, EncodingType encoding)
			: std::ofstream(filename.Str(), mode)
		{
			//throw Croissant::Exception::NotImplementedException(String("OFStream"));
			// TODO Auto-generated constructor stub

		}

		OFStream::~OFStream()
		{
			// TODO Auto-generated destructor stub
		}

	} /* namespace FileSystem */
} /* namespace Croissant */
