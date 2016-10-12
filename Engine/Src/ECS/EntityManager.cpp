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

void EntityManager::AddTag(Entity entity, std::string tag)
{
	if (!Exists(entity))
		return;

	const std::string* strPtr = nullptr;
	auto tags = mTags.find(tag);
	if (tags != mTags.end()) {
		tags->second.push_back(entity);
		strPtr = &tags->first;
	}
	else {
		auto pair = std::pair<std::string, std::list<Entity>>(tag, std::list<Entity>());
		pair.second.push_back(entity);
		mTags.insert(pair);
		strPtr = &pair.first;
	}
}

void EntityManager::RemoveTag(Entity entity, std::string tag)
{
	if (!Exists(entity))
		return;

	auto tags = mTags.find(tag);
	if (tags != mTags.end())
		tags->second.remove(entity);

}

void EntityManager::AddTag(std::string name, std::string tag)
{
	Entity ent = Find(name);
	if (ent != MONSOON_INVALID_ENTITY)
		AddTag(ent, tag);
}

void EntityManager::RemoveTag(std::string name, std::string tag)
{
	Entity ent = Find(name);
	if (ent != MONSOON_INVALID_ENTITY)
		RemoveTag(ent, tag);
}

std::list<Entity> EntityManager::FindByTag(std::string tag)
{
	auto tags = mTags.find(tag);
	if (tags != mTags.end())
		return tags->second;
	else
		return std::list<Entity>();
}

bool EntityManager::Exists(std::string name)
{
	auto pair = mLookupTable.find(name);
	if (pair != mLookupTable.end())
		return true;
	else
		return false;
}

bool EntityManager::Exists(Entity id)
{
	auto pair = mReverseLookupTable.find(id);
	if (pair != mReverseLookupTable.end())
		return true;
	else
		return false;
}

bool EntityManager::hasTag(Entity id, std::string tag)
{
	bool found = false;

	auto iter = mTags.find(tag);
	if (iter != mTags.end())
	{
		auto entity = std::find(iter->second.begin(), iter->second.end(), id);
		if (entity != iter->second.end())
			found = true;
	}

	return found;
}

bool EntityManager::hasTag(std::string name, std::string tag)
{
	bool found = false;

	Entity ent = Find(name);
	if (ent != MONSOON_INVALID_ENTITY)
		found = hasTag(ent, tag);

	return found;
}