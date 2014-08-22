/**
* Copyright (c) 2014 Sonora Games
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
		* Manages a collection of event listeners which are subscribe to a specific event type and how these
		* listeners are invoked.
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
			 */
			ListenerHandle Subscribe(EventType handle, EventCallback callback);

			/**
			 * Unsubscribe a specific listener from the target event type.
			 */
			void Unsubscribe(EventType handle, ListenerHandle listenerId);

			/**
			 * Unsubscribe all listeners from the target event type.
			 */
			void Unsubscribe(EventType handle);

			/**
			 * Invokes all listeners of the target event type and passes them
			 * the specificed argument.
			 */
			void Invoke(EventType, void*);

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