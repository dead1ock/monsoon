/**
* Copyright (c) 2014 Sonora Games
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
, mEventManager(eventManager)
{
	
}

EntityManager::~EntityManager()
{

}

Monsoon::Entity EntityManager::CreateEntity()
{
	Monsoon::Entity entity;
	if (mEntityFreeList.size())
	{
		entity = mEntityFreeList.front();
		mEntityFreeList.pop_front();
	}
	else
		entity = mNextEntityHandle++;

	return entity;
}

Monsoon::Entity EntityManager::CreateEntity(std::string identifier)
{
	if (mEntities.find(identifier) != mEntities.end())
		return std::numeric_limits<Monsoon::Entity>::max();

	Monsoon::Entity entity = CreateEntity();
	mEntities.insert(std::pair<std::string, Monsoon::Entity>(identifier, entity));
	return entity;
}

Monsoon::Entity EntityManager::FindEntity(std::string identifier)
{
	auto entity = mEntities.find(identifier);
	if (entity != mEntities.end())
		return std::numeric_limits<Monsoon::Entity>::max();
	else
		return entity->second;
}

void EntityManager::DestroyEntity(Monsoon::Entity entity)
{
	mEntityFreeList.push_back(entity);
	mEventManager->Invoke("Entity::Destroyed", (void*)entity);
}

void EntityManager::DestroyEntity(std::string identifier)
{
	auto entity = mEntities.find(identifier);
	if (entity != mEntities.end())
		return;
	else {
		mEntityFreeList.push_back(entity->second);
		mEntities.erase(entity);
		mEventManager->Invoke("Entity::Destroyed", (void*)entity->second);
	}
}