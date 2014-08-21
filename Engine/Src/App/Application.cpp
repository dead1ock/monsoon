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

namespace Monsoon {

	Application::Application(Renderer::Renderer* renderer)
		: mRenderer(renderer)
		, mEventManager()
		, mEntityManager(&mEventManager)
	{

	}


	Application::~Application() {
		delete mRenderer;
	}

	bool Application::Initialize() {
		// Initialize Sub-Systems
		mRenderer->Initialize();
		return true;
	}

	void Application::Shutdown() {
		// Shutdown Sub-Systems
		// This MUST be done in the reverse order of initialization.
		mRenderer->Shutdown();
	}

	void Application::Run() {
		//
		// Initialization
		//
		Initialize();
		OnInitialize();

		//
		// Main Loop
		//
		bool quit = false;
		while (!quit) {
			// Update Sub-Systems.
			quit = !mRenderer->Update();
			OnUpdate();
		}
		
		//
		// Shutdown
		//
		OnShutdown();
		Shutdown();
	}

} // namespace monsoon