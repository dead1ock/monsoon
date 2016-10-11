/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include <Platform/Predef.h>

#ifdef MONSOON_OS_WINDOWS
#define NOMINMAX
#endif

#include "ECS/EntityManager.h"

using namespace Monsoon::ECS;
using namespace Monsoon::Event;

EntityManager::EntityManager(EventManager* eventManager)
: mNextEntityHandle(0)
, mEntityFreeList()
, mEventManager(eventManager)
, mEntities()
{
	
}

EntityManager::~EntityManager()
{
	mEntityFreeList.clear();
	mEntities.clear();
}

Monsoon::Entity EntityManager::CreateEntity()
{
	Monsoon::Entity entity = MONSOON_MAX_ENTITIES;
	if (mEntityFreeList.size())
	{
		entity = mEntityFreeList.front();
		mEntityFreeList.erase(mEntityFreeList.begin());
	}
	else {
		entity = mNextEntityHandle++;
	}

	assert((entity < MONSOON_MAX_ENTITIES) && "Maximum entity count reached.");

	mEntities.insert(std::pair<std::string, Entity>(std::to_string(entity), entity));

	return entity;
}

Monsoon::Entity EntityManager::CreateEntity(std::string identifier)
{
	if (mEntities.find(identifier) != mEntities.end())
		return std::numeric_limits<Monsoon::Entity>::max();

	Monsoon::Entity entity = MONSOON_MAX_ENTITIES;
	if (mEntityFreeList.size())
	{
		entity = mEntityFreeList.front();
		mEntityFreeList.erase(mEntityFreeList.begin());
	} else {
		entity = mNextEntityHandle++;
	}

	assert((entity < MONSOON_MAX_ENTITIES) && "Maximum entity count reached.");

	mEntities.insert(std::pair<std::string, Monsoon::Entity>(identifier, entity));
	
	return entity;
}

Monsoon::Entity EntityManager::FindEntity(std::string identifier)
{
	auto entity = mEntities.find(identifier);
	if (entity == mEntities.end())
		return std::numeric_limits<Monsoon::Entity>::max();
	else
		return entity->second;
}

void EntityManager::DestroyEntity(Monsoon::Entity entity)
{
	for (auto iter = mEntities.begin(); iter != mEntities.end(); iter++)
	{
		if (iter->second == entity)
		{
			mEntityFreeList.push_back(entity);
			mEntities.erase(iter);
			break;
		}
	}
	mEventManager->Invoke("Entity::Destroyed", (void*)entity);
}

void EntityManager::DestroyEntity(std::string identifier)
{
	auto entity = mEntities.find(identifier);
	if (entity == mEntities.end())
		return;
	else {
		mEntityFreeList.push_back(entity->second);
		mEventManager->Invoke("Entity::Destroyed", (void*)entity->second);
		mEntities.erase(entity);
	}
}