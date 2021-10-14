#pragma once
#ifdef _WIN32
#include "wtypes.h"
#endif

#ifdef __linux__

#endif

#ifdef __APPLE__

#endif

namespace engine
{

	void getDesktopResolution(int& horizontal, int& vertical)
	{
#ifdef _WIN32
		RECT desktop;
		 
		const HWND hDesktop = GetDesktopWindow();

		GetWindowRect(hDesktop, &desktop);

		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		horizontal = desktop.right;
		vertical = desktop.bottom;
#endif

#ifdef __linux__

#endif

#ifdef __APPLE__

#endif
	}

}