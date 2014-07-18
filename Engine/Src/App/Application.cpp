/**
* Copyright (c) 2014 Sonora Games
*
*/

//
// External Includes
//
#include <windows.h>

//
// Module Includes
//
#include "App/Application.h"

// TEMP
float rand_FloatRange(float a, float b)
{
	return ((b - a)*((float)rand() / RAND_MAX)) + a;
}

bool randomBool() {
	return rand() % 2 == 1;
}
// TEMP

namespace monsoon {

	Application::Application(renderer::Renderer* renderer)
		: mRenderer(renderer)
	{

	}


	Application::~Application() {
		srand(NULL);
	}

	bool Application::Initialize() {
		// Initialize Sub-Systems
		AllocConsole();
		freopen("CONOUT$", "w", stdout);

		mRenderer->Initialize();
		return true;
	}

	void Application::Shutdown() {
		// Shutdown Sub-Systems
		// This MUST be done in the reverse order of initialization.
		mRenderer->Shutdown();
	}

	void Application::Run() {
		if (Initialize()) {	
			bool quit = false;
			while (!quit) {
				// Update Sub-Systems.
				quit = !mRenderer->Update();

			}
		}

		Shutdown();
	}

} // namespace monsoon