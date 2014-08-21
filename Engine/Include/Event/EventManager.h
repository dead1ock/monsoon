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
		* Manages a collection of event listeners which can be invoked by an event.
		*/
		class DYNLIB EventManager
		{
		public:
			EventManager();
			~EventManager();

			ListenerHandle Subscribe(EventType handle, EventCallback callback);
			void Unsubscribe(EventType handle, ListenerHandle listenerId);
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