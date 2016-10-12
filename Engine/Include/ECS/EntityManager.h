/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_ENTITYMANAGER_H_
#define _MONSOON_ENTITYMANAGER_H_

#include <string>
#include <unordered_map>
#include <map>
#include <list>

#include <Platform/DynLib.h>
#include <Platform/Types.h>
#include <Event/EventManager.h>

namespace Monsoon {
	namespace ECS {

		/**
		 * Manages the creation, destruction, and lookup of entities.
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
			Entity Create(std::string identifier);

			/**
			 * Creates a new entity which has no string identifer (anonymous).
			 *
			 * @return Entity A new unique entity handle.
			 */
			Entity Create();

			/**
			 * Looks up an entity by a string identifier.
			 *
			 * @return Entity If the entity was found, the handle of that Entity, otherwise it returns MONSOON_INVALID_ENTITY.
			 */
			Entity Find(std::string identifier);

			/**
			 * Finds all entities with the specified tag.
			 *
			 * @param tag The entity tag.
			 * @return The entities associated with the specified tag.
			 */
			std::list<Entity> FindByTag(std::string tag);

			/**
			 * Adds the specified tag to the target entity.
			 *
			 * @param entity The entity to tag.
			 * @param tag The specified tag.
			 */
			void AddTag(Entity entity, std::string tag);

			/**
			* Adds the specified tag to the target entity by name.
			*
			* @param name The name of the entity.
			* @param tag The specified tag.
			*/
			void AddTag(std::string name, std::string tag);

			/**
			* Removes the specified tag from the target entity by name.
			*
			* @param name The name of the entity.
			* @param tag The specified tag.
			*/
			void RemoveTag(std::string name, std::string tag);

			/**
			* Removes the specified tag from the target entity.
			*
			* @param entity The entity to tag.
			* @param tag The specified tag.
			*/
			void RemoveTag(Entity entity, std::string tag);

			/**
			 * Removes all tags from the target entity.
			 *
			 * @param entity The target entity.
			 */
			void RemoveAllTags(Entity entity);

			/**
			* Removes all tags from the target entity by name.
			*
			* @param name The name of the entity.
			*/
			void RemoveAllTags(std::string name);

			/**
			 * Determines if the target entity has the specified tag.
			 *
			 * @param id Id of the entity.
			 * @param tag The tag to check for.
			 * @return Returns true if a match is found, otherwise false.
			 */
			bool hasTag(Entity id, std::string tag);

			/**
			 * Determines if the target entity has the specified tag, by name.
			 *
			 * @param name Name of the entity.
			 * @param tag The tag to check for.
			 * @return REturns true if a match is found, otherwise false.
			 */
			bool hasTag(std::string name, std::string tag);

			/**
			 * Determines if the target entity exists by name.
			 * 
			 * @param name Name of the entity to find.
			 * @return True if the entity exists, otherwise false.
			 */
			bool Exists(std::string name);

			/**
			* Determines if the target entity exists by id.
			*
			* @param id Id of the entity to find.
			* @return True if the entity exists, otherwise false.
			*/
			bool Exists(Entity id);

			/**
			 * Destroys an entity based on its associated string identifier.
			 */
			void Destroy(std::string identifier);

			/**
			 * Destroys an entity based on its handle.
			 */
			void Destroy(Entity entity);

			/**
			 * Returns the number of entities which exists in the manager.
			 *
			 * @return The number of entities alive.
			 */
			U32 Count();

		private:
			Entity GetNextFreeId();

			Event::EventManager* mEventManager;
			Monsoon::Entity mNextEntityHandle;

			std::map<std::string, Monsoon::Entity> mLookupTable;
			std::unordered_map<Monsoon::Entity, std::string*> mReverseLookupTable;

			std::string mDefaultIdentifier;

			std::map<std::string, std::list<Entity>> mTags;
			std::map<Entity, std::list<const std::string*>> mEntityTags;

			std::vector<Monsoon::Entity> mEntityFreeList;
		};
	}
}

#endif