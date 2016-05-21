#include "Core/Node.hpp"
#include "Math/Math.hpp"
#include "Core/Mesh.hpp"
#include <string>
#include <algorithm>
#include "Debug/MemoryManager.hpp"

namespace Croissant
{
	namespace Core
	{
		class Node::Pimpl
		{
		public:
			Pimpl() 
				: m_name{ Node::GenerateName() }, m_parent{ nullptr }, m_children(5),
				m_rotation{ Math::Vector4::Zero, 1 }, m_translation{ Math::Vector4::Zero },
				m_meshes{}, 
				m_needUpdate{ false }
				, m_onUpdateListeners {}
				, m_modelToWorldMatrix{}
			{}

			std::string								m_name;
			Node*									m_parent;
			std::vector<std::unique_ptr<Node>>		m_children;
			Math::Quaternion						m_rotation;
			Math::Vector4							m_translation;
			std::vector<std::shared_ptr<Mesh>>		m_meshes;
			mutable bool							m_needUpdate;
			mutable std::list<OnUpdateCallback*>	m_onUpdateListeners;
			mutable Math::Matrix4f					m_modelToWorldMatrix;
		};

		Node::Node()
			: m_pimpl{ CROISSANT_NEW Pimpl{ } }
		{
		}

		Node::~Node()
		{
			CROISSANT_DELETE(m_pimpl);
			m_pimpl = nullptr;
		}

		void Node::AddOnUpdate(OnUpdateCallback& callback) const
		{
			m_pimpl->m_onUpdateListeners.push_back(&callback);
		}

		void Node::RemoveOnUpdate(OnUpdateCallback& callback) const
		{
			m_pimpl->m_onUpdateListeners.remove(&callback);
		}

		void Node::Move(Math::Vector4 const& move)
		{
			// TODO : marquer la matrice comme modifiée et non la calculer tout de suite
			m_pimpl->m_translation = m_pimpl->m_translation + move;
			m_pimpl->m_needUpdate = true;
		}

		void Node::Rotate(Math::Quaternion const& rotation)
		{
			m_pimpl->m_rotation = rotation * m_pimpl->m_rotation;
			m_pimpl->m_needUpdate = true;
		}

		Math::Matrix4f const& Node::GetModelToWorldMatrix() const
		{
			Update();
			return m_pimpl->m_modelToWorldMatrix;
		}

		std::string Node::GenerateName()
		{
			static uint64_t nextNum;

			return std::string("Unnamed Node ") + std::to_string(nextNum++);
		}

		void Node::PreUpdate() const
		{
			// TODO mettre ici les mises à jour à faire avant de demander aux fils de se mettre à jour
			Math::Matrix4f result;

			if (nullptr != m_pimpl->m_parent)
			{
				result = m_pimpl->m_parent->GetModelToWorldMatrix();
			}
			else
			{
				result.MakeIdentity();
			}

			result = result * (ToMatrix(m_pimpl->m_translation) * ToMatrix(m_pimpl->m_rotation));
			m_pimpl->m_modelToWorldMatrix = std::move(result);
		}

		void Node::Update(bool goUp) const
		{
			m_pimpl->m_needUpdate = false;
			PreUpdate();
			// TODO itérer sur les enfants pour les mettre à jour.
			PostUpdate();
			if (goUp && nullptr != m_pimpl->m_parent)
			{
				auto parent = m_pimpl->m_parent;

				do
				{
					parent->PostUpdate();
					parent = parent->m_pimpl->m_parent;
				} while (nullptr != parent);
			}
		}

		void Node::PostUpdate() const
		{
			// TODO mettre ici les mises à jour à faire après avoir demandé aux fils mais avant de propager au père, par exemple mettre à jour les bounding boxes
			NotifyUpdate();
		}

		void Node::NotifyUpdate() const
		{
			std::for_each(m_pimpl->m_onUpdateListeners.begin(), m_pimpl->m_onUpdateListeners.end(), [this](OnUpdateCallback* callback) {
				(*callback)(*this, m_pimpl->m_modelToWorldMatrix);
			});
		}

		void Node::Update() const
		{
			if (!m_pimpl->m_needUpdate)
			{
				return;
			}

			Update(false);
		}

		void Node::AddMesh(std::shared_ptr<Mesh> mesh)
		{
			m_pimpl->m_meshes.push_back(mesh);
		}

		std::vector<std::shared_ptr<Mesh>> const& Node::GetMeshes() const
		{
			return m_pimpl->m_meshes;
		}

		Math::Vector4 const&	Node::GetTranslation() const
		{
			return m_pimpl->m_translation;
		}

		Math::Quaternion const&	Node::GetRotation() const
		{
			return m_pimpl->m_rotation;
		}
	}
}
