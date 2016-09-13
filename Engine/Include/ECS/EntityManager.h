/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_ENTITYMANAGER_H_
#define _MONSOON_ENTITYMANAGER_H_

#include <string>
#include <unordered_map>
#include <list>

#include <Platform/DynLib.h>
#include <Platform/Types.h>
#include <Event/EventManager.h>

namespace Monsoon {
	namespace ECS {

		/**
		 * Manages the creation, destruction, and lookup of simulation entities.
		 */
		class DYNLIB EntityManager
		{
		public:
			EntityManager(Event::EventManager* eventManager);
			~EntityManager();

			/**
			 * Creates a new entity and associates it with the sepcified string identifier.
			 *
			 * @return Entity A new unique entity handle.
			 */
			Entity CreateEntity(std::string identifier);

			/**
			 * Creates a new entity which has no string identifer (anonymous).
			 *
			 * @return Entity A new unique entity handle.
			 */
			Entity CreateEntity();

			/**
			 * Finds an entity which has the target associated string identifier.
			 *
			 * @return Entity If the entity was found, the handle of that Entity, otherwise -1.
			 */
			Entity FindEntity(std::string identifier);

			/**
			 * Destroys an entity based on its associated string identifier.
			 */
			void DestroyEntity(std::string identifier);

			/**
			 * Destroys an entity based on its handle.
			 */
			void DestroyEntity(Entity entity);

		private:
			Event::EventManager* mEventManager;
			Monsoon::Entity mNextEntityHandle;
			std::unordered_map<std::string, Monsoon::Entity> mEntities;
			std::vector<Monsoon::Entity> mEntityFreeList;
		};
	}
}

#endif