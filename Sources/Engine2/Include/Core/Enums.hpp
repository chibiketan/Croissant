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

		enum class BufferTypeEnum
		{
			Index,
			Vertex
		};
	
		enum class VertexComponentEnum
		{
			Position = 0,
			Color,
			MAX_ELEMENT = Color
		};

		enum class VertexComponentTypeEnum
		{
			Undefined,
			Float1,
			Float2,
			Float3,
			Float4,
			Color,
			MAX_ELEMENT = Color
		};
	}
}

#endif // !CROISSANT_ENGINE_CORE_ENUMS_HPP_INC