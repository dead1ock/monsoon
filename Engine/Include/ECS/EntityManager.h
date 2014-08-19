/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <string>
#include <map>
#include <list>

#include <Platform/DynLib.h>
#include <Platform/Types.h>

namespace Monsoon {
	namespace ECS {
		class DYNLIB EntityManager
		{
		public:
			EntityManager();
			~EntityManager();

			Entity CreateEntity(std::string identifier);
			Entity CreateEntity();
			Entity FindEntity(std::string identifier);
			void DestroyEntity(std::string identifier);
			void DestroyEntity(Entity entity);

		private:
			Monsoon::Entity mNextEntityHandle;
			std::map<std::string, Monsoon::Entity> mEntities;
			std::list<Monsoon::Entity> mEntityFreeList;
		};
	}
}