#include "stdafx.hpp"
#include "Scene/SceneManager.hpp"
#include "Scene/SceneElement.hpp"

namespace Croissant
{
	namespace Scene
	{
		class SceneManager::Pimpl
		{
		public:

		private:
			SceneElement* m_root;
		};

		// ------------------------------------ SceneManager::Pimpl implémentation

		// ------------------------------------ SceneManager implémentation
		SceneManager::SceneManager()
			: m_pimpl(CROISSANT_NEW SceneManager::Pimpl())
		{
		}

		SceneManager::~SceneManager()
		{
			CROISSANT_DELETE(m_pimpl);
		}

	}
}
