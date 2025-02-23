/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_CLOCK_H_
#define _MONSOON_CLOCK_H_

#include <chrono>

#include <Platform/Export.h>

namespace Monsoon {
	namespace Platform {
		
		/**
		 * High-Resolution clock to track current time and calculate change in time over frames (delta time).
		 */
		class DYNLIB Clock
		{
		public:
			Clock();
			~Clock();

			void Update();

			/**
			 * Calculates the time between the last frame and now in seconds.
			 *
			 * @return How many seconds have passed since the last frame.
			 */
			const double getDeltaTime() const;

			/**
			 * Returns the current system time.
			 */
			const double getTime() const;

		private:
			std::chrono::high_resolution_clock::time_point mLastFrameTime;
		};
	}
}

#endif // _MONSOON_CLOCK_H_