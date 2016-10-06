/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

// External Includes
#include <windows.h>


// Module Includes
#include "App/Application.h"

namespace Monsoon {

	Application::Application(Renderer::Renderer* renderer)
		: mRenderer(renderer)
		, mEventManager()
		, mEntityManager(&mEventManager)
		, mTransformSystem(&mEventManager)
		, mAnimationSystem(&mGameClock, mRenderer)
		, mQuit(false)
	{
	}


	Application::~Application() {
		delete mRenderer;
	}

	bool Application::Initialize() {
		bool initSuccess = true;

		// Initialize simulation subsystems
		initSuccess = mRenderer->Initialize();

		return initSuccess;
	}

	void Application::Shutdown() {
		// Shutdown Sub-Systems
		// This MUST be done in the reverse order of initialization.
		mRenderer->Shutdown();

	}

	void Application::Run() {
		// Initialization
		Initialize();
		OnInitialize();

		// Main Loop
		while (!mQuit) {

			// Update simulation sub-systems.
			mGameClock.Update();
			mRenderer->Update();
			mAnimationSystem.Update();

			// Update the application.
			OnUpdate();
		}
		
		// Shutdown
		OnShutdown();
		Shutdown();
	}

	void Application::Quit() {
		mQuit = true;
	}

} // namespace monsoon