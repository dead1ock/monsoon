/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_APPLICATION_H_
#define _MONSOON_APPLICATION_H_

#include <boost/noncopyable.hpp>

#include "Platform\DynLib.h"
#include "Renderer\Renderer.h"
#include <Renderer/AnimationSystem.h>
#include "ECS\EntityManager.h"
#include "Event\EventManager.h"
#include "Platform\Clock.h"
#include "Resource\ResourceManager.h"
#include <Platform/Log.h>
#include <Scene/SpatialSystem.h>

namespace Monsoon {
	/*
	* Manages the initialization and shutdown of sub-systems (rendering, sound, physics, networking, simulation ect...) and
	* handles the application loop.
	*/
	class DYNLIB Application : boost::noncopyable {
	public:
		Application(Renderer::Renderer* renderer);
		~Application();

		void Run();

	protected:
		/**
		* Called after engine sub-systems are initialized.
		*/
		virtual void OnInitialize()=0;

		/**
		* Called before engine sub-systems are shutdown.
		*/
		virtual void OnShutdown()=0;

		virtual void OnUpdate() = 0;


		Renderer::Renderer* mRenderer;
		ECS::EntityManager mEntityManager;
		Event::EventManager mEventManager;
		Platform::Clock mGameClock;
		Scene::SpatialSystem mSpatialSystem;
		Resource::ResourceManager mResourceManager;
		Platform::Log mLog;
		Renderer::AnimationSystem mAnimationSystem;

	private:
		/**
		* Initializes each simulation sub-system and checks for initialization failures.
		*/
		bool Initialize();

		/**
		* Shuts down each simulation sub-system.
		*/
		void Shutdown();

	};
}

#endif // _MONSOON_APPLICATION_H_