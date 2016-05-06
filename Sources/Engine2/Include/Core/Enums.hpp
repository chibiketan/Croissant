#ifndef CROISSANT_ENGINE_CORE_ENUMS_HPP_INC
#  define CROISSANT_ENGINE_CORE_ENUMS_HPP_INC

namespace Croissant
{
	namespace Core
	{
		enum class BufferUsageEnum
		{
			Static = 0,

			MAX_ELEMENTS = Static
		};

		enum class BufferAccessEnum
		{
			Read = 0,
			Write,
			ReadWrite,

			MAX_ELEMENT = ReadWrite
		};
	}
}

#endif // !CROISSANT_ENGINE_CORE_ENUMS_HPP_INC