#pragma once
#ifndef NODE_H_
#define NODE_H_

#include "Engine.hpp"
#include "Core/Object.hpp"
#include <list>

namespace Croissant
{
	namespace Scene
	{
		class ENGINE_API Node : public Croissant::Core::Object
		{
			CROISSANT_TYPE_DECLARATION;

		public:
			typedef std::list<Node*> NodeList;

			Node();
			virtual ~Node();
			Node* Parent() const;
			bool SetParent(Node* parent);
			const NodeList& Children() const;
			bool AddChild(Node* child);
			bool RemoveChild(Node* child);

		private:
			class Pimpl;

			Pimpl* m_pimpl;
		};
	}
}

#endif
