#include "Core/Node.hpp"
#include <string>
#include <Math/Math.inl>

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

		Math::Matrix4 const& Node::GetModelToWorldMatrix() const
		{
			Update();
			return m_modelToWorldMatrix;
		}

		std::string Node::GenerateName()
		{
			static uint64_t nextNum;

			return std::string("Unnamed Node ") + std::to_string(nextNum++);
		}

		void Node::Update() const
		{
			if (!m_needUpdate)
			{
				return;
			}

			m_needUpdate = false;

			Math::Matrix4 result;

			if (nullptr != m_parent)
			{
				result = m_parent->GetModelToWorldMatrix();
			}
			else
			{
				result.LoadIdentity();
			}

			result = result * m_rotation.ToMatrix() * Math::ToMatrix(m_translation);
			m_modelToWorldMatrix = std::move(result);
			std::for_each(m_onUpdateListeners.begin(), m_onUpdateListeners.end(), [this] (OnUpdateCallback* callback) {
				(*callback)(*this, m_modelToWorldMatrix);
			});
		}
	}
}
