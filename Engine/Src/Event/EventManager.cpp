/**
* Copyright (c) 2014 Sonora Games
*
*/

#include "Event/EventManager.h"

#include <algorithm>

using namespace Monsoon;
using namespace Monsoon::Event;

EventManager::EventManager()
: mNextListenerHandle(0)
{

}

EventManager::~EventManager()
{

}

ListenerHandle EventManager::Subscribe(EventType type, EventCallback callback)
{
	// Find the listeners via the event type, register the new type if it doesn't exist.
	auto listeners = mListeners.find(type);
	if (listeners == mListeners.end())
		listeners = mListeners.insert(std::pair<EventType, std::vector<Listener>>(type, std::vector<Listener>())).first;

	Listener listener;
	listener.Handle = mNextListenerHandle++;
	listener.Func = callback;

	listeners->second.push_back(listener);
	return listener.Handle;
}

void EventManager::Unsubscribe(EventType type, U32 listenerId)
{
	auto listeners = mListeners.find(type);
	if (listeners != mListeners.end())
		listeners->second.erase(

		std::find_if(listeners->second.begin(), listeners->second.end(), 
		[listenerId](const Listener& listener) 
			{ 
				return (listener.Handle == listenerId); 
			}
		));
}

void EventManager::Invoke(EventType type, void* arg)
{
	auto listeners = mListeners.find(type);
	if (listeners == mListeners.end())
		return;

	for (auto iter = listeners->second.begin(); iter != listeners->second.end(); iter++)
		iter->Func(arg);
}