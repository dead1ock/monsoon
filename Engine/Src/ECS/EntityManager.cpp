/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include <Platform/Predef.h>

#ifdef MONSOON_OS_WINDOWS
#define NOMINMAX
#endif

#include "ECS/EntityManager.h"

using namespace Monsoon;
using namespace Monsoon::ECS;
using namespace Monsoon::Event;

EntityManager::EntityManager(EventManager* eventManager)
: mNextEntityHandle(0)
, mEntityFreeList()
, mEventManager(eventManager)
, mLookupTable()
, mReverseLookupTable()
, mDefaultIdentifier("null")
{
	
}

EntityManager::~EntityManager()
{
	mEntityFreeList.clear();
	mLookupTable.clear();
}

Monsoon::Entity EntityManager::Create()
{
	Entity entity = GetNextFreeId();
	mReverseLookupTable.insert(std::pair<Monsoon::Entity, std::string*>(entity, &mDefaultIdentifier));
	return entity;
}

Monsoon::Entity EntityManager::Create(std::string identifier)
{
	if (mLookupTable.find(identifier) != mLookupTable.end())
		return MONSOON_INVALID_ENTITY;

	Entity entity = GetNextFreeId();

	std::pair<std::string, Monsoon::Entity> pair = std::make_pair(identifier, entity);
	mLookupTable.insert(pair);
	mReverseLookupTable.insert(std::pair<Monsoon::Entity, std::string*>(entity, &pair.first));
	
	return entity;
}

Monsoon::Entity EntityManager::Find(std::string identifier)
{
	auto entity = mLookupTable.find(identifier);
	if (entity == mLookupTable.end())
		return MONSOON_INVALID_ENTITY;
	else
		return entity->second;
}

void EntityManager::Destroy(Monsoon::Entity entity)
{
	// Remove string identifier if one exists for this entity.
	auto identifier = mReverseLookupTable.find(entity);
	if (identifier != mReverseLookupTable.end()) {
		mReverseLookupTable.erase(identifier);
		mEntityFreeList.push_back(entity);
		mEventManager->Invoke("Entity::Destroyed", (void*)entity);
	}
}

void EntityManager::Destroy(std::string identifier) {
	auto entity = mLookupTable.find(identifier);
	if (entity == mLookupTable.end())
		return;
	else {
		mEntityFreeList.push_back(entity->second);
		mEventManager->Invoke("Entity::Destroyed", (void*)entity->second);
		mLookupTable.erase(entity);
	}
}

U32 EntityManager::Count() {
	return U32(mNextEntityHandle - mEntityFreeList.size());
}

Entity EntityManager::GetNextFreeId() {
	Monsoon::Entity entity = MONSOON_INVALID_ENTITY;
	if (mEntityFreeList.size())
	{
		entity = mEntityFreeList.front();
		mEntityFreeList.erase(mEntityFreeList.begin());
	}
	else {
		entity = mNextEntityHandle++;
	}

	assert((entity < MONSOON_MAX_ENTITIES) && "Maximum entity count reached.");
	return entity;
}