/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_EVENT_MANAGER_H_
#define _MONSOON_EVENT_MANAGER_H_

#include <vector>
#include <unordered_map>
#include <functional>

#include <Platform/Types.h>
#include <Platform/Export.h>

namespace Monsoon {
	namespace Event {

		typedef std::string						EventType;
		typedef std::function<int(void*)>		EventCallback;
		typedef U32								ListenerHandle;

		/**
		* Manages a collection of event listeners and allows users to invoke events.
		*/
		class DYNLIB EventManager
		{
		public:
			EventManager();
			~EventManager();

			/**
			 * Subscribes a callback function to the target event type, then returns
			 * a listener handle which should later be used when unsubscribing this specific
			 * listener.
			 *
			 * @param handle An identifier for the type of event to subscribe to.
			 * @param callback A function pointer to the method which will be called when
			 * the specified event is invoked.
			 * 
			 * @return A unique listener handle which should be used to unsubscribe from the event.
			 */
			ListenerHandle Subscribe(EventType handle, EventCallback callback);

			/**
			 * Unsubscribe a specific listener from the target event type.
			 *
			 * @param handle The event type.
			 * @param listenerId The unique listener which is unsubscribing.
			 */
			void Unsubscribe(EventType handle, ListenerHandle listenerId);

			/**
			 * Unsubscribes all listeners from the target event type.
			 *
			 * @param handle The target event type.
			 */
			void Unsubscribe(EventType handle);

			/**
			 * Invokes all listeners of the target event type, and passes
			 * the specificed argument to their callback functions.
			 *
			 * @param handle The target event type.
			 */
			void Invoke(EventType handle, void* arg);

		private:
			struct Listener
			{
				ListenerHandle Handle;
				EventCallback Func;
			};

			std::unordered_map<EventType, std::vector<Listener>> mListeners;
			ListenerHandle mNextListenerHandle;
		};
	}
}

#endif // _MONSOON_EVENT_MANAGER_H_