/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_APPLICATION_H_
#define _MONSOON_APPLICATION_H_

#include <boost/noncopyable.hpp>

#include "Platform\DynLib.h"
#include "Renderer\Renderer.h"

namespace monsoon {
	/*
	* Manages the initialization and shutdown of sub-systems (rendering, sound, physics, networking, simulation ect...) and
	* handles the application loop.
	*/
	class DYNLIB Application : boost::noncopyable {
	public:
		Application(renderer::Renderer* renderer);
		~Application();

		void Run();

	private:
		/**
		* Initializes each simulation sub-system and checks for initialization failures.
		*/
		bool Initialize();

		/**
		* Shuts down each simulation sub-system.
		*/
		void Shutdown();

		renderer::Renderer* mRenderer;
	};
}

#endif // _MONSOON_APPLICATION_H_