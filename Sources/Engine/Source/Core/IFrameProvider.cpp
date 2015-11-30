#include "Core/IFrameProvider.hpp"

namespace Croissant
{
	namespace Core
	{
		namespace
		{
			static const String TypeName("IFrameProvider", EncodingType::ASCII);
		}

		const String& IFrameProvider::GetTypeName()
		{
				return TypeName;
		}


	}  // namespace Core
}  // namespace Croissant
