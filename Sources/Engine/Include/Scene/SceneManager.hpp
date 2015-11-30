#pragma once
#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include "Engine.hpp"

namespace Croissant
{
	namespace Scene
	{
		class ENGINE_API SceneManager
		{
		public:
			SceneManager();
			~SceneManager();

		private:
			class Pimpl;

			Pimpl* m_pimpl;
		};
	}
}

#endif
