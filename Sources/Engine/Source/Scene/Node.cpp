#include "stdafx.hpp"
#include "Scene/Node.hpp"
#include <algorithm>

namespace Croissant
{
	namespace Scene
	{
		class Node::Pimpl
		{
		public:
			Pimpl(Node* node);
			~Pimpl();
			Node* Parent() const;
			bool SetParent(Node* parent);
			const Node::NodeList& Children() const;
			bool AddChild(Node* child);
			bool RemoveChild(Node* child);

		private:
			Node::NodeList m_children;
			Node* m_parent;
			Node* m_this;
		};

		// ----------------------------------------- Node::Pimpl implémentation
		Node::Pimpl::Pimpl(Node* node)
			: m_children(),
			m_parent(nullptr),
			m_this(node)
		{
		}

		Node::Pimpl::~Pimpl()
		{
			auto begin = m_children.cbegin();
			auto end = m_children.cend();

			for (; begin != end; ++begin)
			{
				CROISSANT_DELETE(*begin);
			}

			m_children.clear();
			if (nullptr != m_parent)
			{
				m_parent->RemoveChild(m_this);
				m_parent = nullptr;
			}
		}

		Node* Node::Pimpl::Parent() const
		{
			return m_parent;
		}

		bool Node::Pimpl::SetParent(Node* parent)
		{
			if (nullptr != m_parent && nullptr != parent)
			{
				return false;
			}

			m_parent = parent;
			return true;
		}

		const Node::NodeList& Node::Pimpl::Children() const
		{
			return m_children;
		}

		bool Node::Pimpl::AddChild(Node* child)
		{
			if (nullptr == child || child == m_this)
			{
				return false;
			}

			if (nullptr != child->Parent())
			{
				return false;
			}

			if (!child->SetParent(m_this))
			{
				return false;
			}

			m_children.push_back(child);
			return true;
		}

		bool Node::Pimpl::RemoveChild(Node* child)
		{
			if (nullptr == child || child->Parent() != m_this)
			{
				return false;
			}

			m_children.remove(child);
			return true;
		}

		// ----------------------------------------- Node implémentation
		using Type = Croissant::Core::Type;

		const Type Node::s_type = Type::Make<Croissant::Core::Object>("Croissant::Scene::Node");

		Node::Node()
			: m_pimpl(nullptr)
		{
			m_pimpl = CROISSANT_NEW Node::Pimpl(this);
		}

		Node::~Node()
		{
			CROISSANT_DELETE(m_pimpl);
		}

		Node* Node::Parent() const
		{
			return m_pimpl->Parent();
		}

		bool Node::SetParent(Node* parent)
		{
			return m_pimpl->SetParent(parent);
		}

		const Node::NodeList& Node::Children() const
		{
			return m_pimpl->Children();
		}

		bool Node::AddChild(Node* child)
		{
			return m_pimpl->AddChild(child);
		}

		bool Node::RemoveChild(Node* child)
		{
			return m_pimpl->RemoveChild(child);
		}

	}
}
