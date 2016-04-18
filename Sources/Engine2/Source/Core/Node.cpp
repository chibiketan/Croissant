#include "Core/Node.hpp"
#include "Math/Math.hpp"
#include <string>
#include <algorithm>

namespace Croissant
{
	namespace Core
	{
		Node::Node()
			: m_name { Node::GenerateName() }, m_parent { nullptr }, m_children(5),
			m_rotation{ Math::Vector4::Zero, 1}, m_translation { Math::Vector4::Zero },
			m_onUpdateListeners{},
			m_needUpdate{ false }
		{
		}

		void Node::AddOnUpdate(OnUpdateCallback& callback) const
		{
			m_onUpdateListeners.push_back(&callback);
		}

		void Node::RemoveOnUpdate(OnUpdateCallback& callback) const
		{
			m_onUpdateListeners.remove(&callback);
		}

		void Node::Move(Math::Vector4 const& move)
		{
			// TODO : marquer la matrice comme modifiée et non la calculer tout de suite
			m_translation = m_translation + move;
			m_needUpdate = true;
		}

		void Node::Rotate(Math::Quaternion const& rotation)
		{
			m_rotation *= rotation;
			m_needUpdate = true;
		}

		Math::Matrix4f const& Node::GetModelToWorldMatrix() const
		{
			Update();
			return m_modelToWorldMatrix;
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

			if (nullptr != m_parent)
			{
				result = m_parent->GetModelToWorldMatrix();
			}
			else
			{
				result.MakeIdentity();
			}

			result = result * (ToMatrix(m_translation) * ToMatrix(m_rotation));
			m_modelToWorldMatrix = std::move(result);
		}

		void Node::Update(bool goUp) const
		{
			m_needUpdate = false;
			PreUpdate();
			// TODO itérer sur les enfants pour les mettre à jour.
			PostUpdate();
			if (goUp && nullptr != m_parent)
			{
				auto parent = m_parent;

				do
				{
					parent->PostUpdate();
					parent = parent->m_parent;
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
			std::for_each(m_onUpdateListeners.begin(), m_onUpdateListeners.end(), [this](OnUpdateCallback* callback) {
				(*callback)(*this, m_modelToWorldMatrix);
			});
		}

		void Node::Update() const
		{
			if (!m_needUpdate)
			{
				return;
			}

			Update(false);
		}
	}
}
