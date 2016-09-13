/**
* Copyright (c) 2014-2016 Dallin Wellington
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
#include <Scene/TransformSystem.h>

namespace Monsoon {
	/*
	* A barebones application implimentation which manages the initialization, shutdown, and updating of simulation sub-systems.
	*/
	class DYNLIB Application : boost::noncopyable {
	public:

		/**
		* Default Constructor
		*/
		Application(Renderer::Renderer* renderer);
		
		/**
		 * Default Destructor
		 */
		~Application();

		/**
		 * Boots the application and enters the application loop until
		 * Application::Quit() is called.
		 */
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

		/**
		 * Called on every iteration of the application loop after simulation
		 * subsystems are updated.
		 */
		virtual void OnUpdate() = 0;

		/**
		 * Flags the application for shutdown.
		 */
		void Quit();


		Renderer::Renderer* mRenderer;
		ECS::EntityManager mEntityManager;
		Event::EventManager mEventManager;
		Platform::Clock mGameClock;
		Scene::TransformSystem mTransformSystem;
		Resource::ResourceManager mResourceManager;
		Platform::Log mLog;
		Renderer::AnimationSystem mAnimationSystem;
		bool mQuit;

	private:
		/**
		* Initializes each simulation sub-system and checks for initialization failures.
		*
		* @return bool If initialization is successful, returns true, otherwise this function returns false.
		*/
		bool Initialize();

		/**
		* Shuts down each simulation sub-system.
		*/
		void Shutdown();

	};
}

#endif // _MONSOON_APPLICATION_H_