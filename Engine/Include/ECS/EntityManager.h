/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <string>
#include <unordered_map>
#include <list>

#include <Platform/DynLib.h>
#include <Platform/Types.h>
#include <Event/EventManager.h>

namespace Monsoon {
	namespace ECS {
		class DYNLIB EntityManager
		{
		public:
			EntityManager(Event::EventManager* eventManager);
			~EntityManager();

			Entity CreateEntity(std::string identifier);
			Entity CreateEntity();
			Entity FindEntity(std::string identifier);
			void DestroyEntity(std::string identifier);
			void DestroyEntity(Entity entity);

		private:
			Event::EventManager* mEventManager;
			Monsoon::Entity mNextEntityHandle;
			std::unordered_map<std::string, Monsoon::Entity> mEntities;
			std::list<Monsoon::Entity> mEntityFreeList;
		};
	}
}