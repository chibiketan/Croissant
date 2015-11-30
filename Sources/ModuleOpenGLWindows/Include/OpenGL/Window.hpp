#pragma once

#ifndef WINDOW_H_
#define WINDOW_H_

#include <memory>
#include <Windows.h>

namespace Croissant
{
	namespace Module
	{
		namespace OpenGL
		{
			class Window
			{
			public:
				Window();
				~Window();
				bool Init();
				void Cleanup();
				const HWND& GetHandle() const;

			private:
				class Pimpl;

				std::unique_ptr<Pimpl> m_pimpl;
			};
		}
	}
}

#endif
