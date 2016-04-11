#ifndef CROISSANT_ENGINE_CORE_NODE_HPP_INC
#  define CROISSANT_ENGINE_CORE_NODE_HPP_INC
#  pragma once

#  include "Engine.hpp"
#  include "Math/Quaternion.hpp"
#  include "Math/Vector4.hpp"
#  include <vector>
#  include <memory>
#  include <functional>
#  include <list>

namespace Croissant
{

	namespace Core
	{
		class ENGINE_API Node
		{
		public:
			Node();


		private:
			Node*									m_parent;
			std::vector<std::unique_ptr<Node>>		m_children;
			Math::Quaternion						m_rotation;
			Math::Vector4							m_translation;
			std::list<std::function<void (Node&)>>	m_onUpdateListeners;
		};
	}
}


#endif // !CROISSANT_ENGINE_CORE_NODE_HPP_INC