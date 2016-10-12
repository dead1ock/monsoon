/**
* Copyright (c) 2016 Dallin Wellington
*
*/

#include <gtest/gtest.h>

#include <ECS/EntityManager.h>

using namespace Monsoon;
using namespace Monsoon::ECS;
using namespace Monsoon::Event;

TEST(EntityManager, EmptyDestroy) {
	EventManager eventManager;
	EntityManager manager(&eventManager);
	manager.Destroy(0);
	EXPECT_EQ(0, manager.Count());
}

TEST(EntityManager, CreateOne) {
	EventManager eventManager;
	EntityManager manager(&eventManager);
	EXPECT_EQ(manager.Create(), 0);
	EXPECT_EQ(manager.Count(), 1);
}

TEST(EntityManager, EmptyExists) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	EXPECT_EQ(false, manager.Exists(0));
	EXPECT_EQ(false, manager.Exists("null"));
}

TEST(EntityManager, EmptyHasTag) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	EXPECT_EQ(0, manager.hasTag("player", "player"));
	EXPECT_EQ(false, manager.hasTag(0, "player"));
}

TEST(EntityManager, EmptyRemoveTag) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	manager.RemoveTag(0, "player");
}

TEST(EntityManager, EmptyFindByTag) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	EXPECT_EQ(0, manager.FindByTag("player").size());
}

TEST(EntityManager, DeleteOne) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	Entity ent = manager.Create();
	EXPECT_EQ(manager.Count(), 1);

	manager.Destroy(ent);
	EXPECT_EQ(manager.Count(), 0);
}

TEST(EntityManager, ReuseId) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	Entity ent = manager.Create();
	EXPECT_EQ(manager.Count(), 1);

	manager.Destroy(ent);
	EXPECT_EQ(manager.Count(), 0);

	ent = manager.Create();
	EXPECT_EQ(manager.Count(), 1);
}

TEST(EntityManager, InvalidRemove) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	Entity ent = manager.Create();
	EXPECT_EQ(manager.Count(), 1);

	manager.Destroy(1);
	EXPECT_EQ(manager.Count(), 1);
}

TEST(EntityManager, InvalidFind) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	EXPECT_EQ(manager.Find("a"), MONSOON_INVALID_ENTITY);
}

TEST(EntityManager, CreateOneByName) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	manager.Create("myObject");
	EXPECT_EQ(1, manager.Count());
}

TEST(EntityManager, DestroyOneByName) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	manager.Create("myObject");
	EXPECT_EQ(1, manager.Count());

	manager.Destroy("myObject");
	EXPECT_EQ(0, manager.Count());
}

TEST(EntityManager, FindOne) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	manager.Create("myObject");
	EXPECT_NE(manager.Find("myObject"), MONSOON_INVALID_ENTITY);
}

TEST(EntityManager, AddTag) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	Entity myObject = manager.Create("myObject");
	manager.AddTag(myObject, "player");
	manager.AddTag("myObject", "debuff");
	EXPECT_EQ(true, manager.hasTag(myObject, "player"));
	EXPECT_EQ(true, manager.hasTag("myObject", "player"));
	EXPECT_EQ(true, manager.hasTag(myObject, "debuff"));
	EXPECT_EQ(true, manager.hasTag("myObject", "debuff"));
	EXPECT_EQ(false, manager.hasTag(myObject, "enemy"));
	EXPECT_EQ(false, manager.hasTag("myObject", "enemy"));
}

TEST(EntityManager, RemoveTag) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	Entity myObject = manager.Create("myObject");
	manager.AddTag(myObject, "player");
	manager.RemoveTag(myObject, "player");
	EXPECT_EQ(false, manager.hasTag(myObject, "player"));
	EXPECT_EQ(false, manager.hasTag("myObject", "player"));
}

TEST(EntityManager, InvalidAddTag) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	// Try adding a tag to an entity which doesn't exist.
	manager.AddTag(0, "player");
	Entity myObject = manager.Create("player");

	EXPECT_EQ(false, manager.hasTag("player", "player"));
	EXPECT_EQ(false, manager.hasTag(myObject, "player"));
}

TEST(EntityManager, FindByTag) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	Entity myObject = manager.Create("myObject");
	Entity mySecondObject = manager.Create("mySecondObject");

	manager.AddTag(myObject, "player");
	manager.AddTag(myObject, "debuff");
	manager.AddTag(mySecondObject, "enemey");
	manager.AddTag(mySecondObject, "debuff");

	auto list = manager.FindByTag("debuff");
	EXPECT_EQ(myObject, list.front());
	EXPECT_EQ(mySecondObject, list.back());
	EXPECT_EQ(2, list.size());
	EXPECT_EQ(0, manager.FindByTag("someproperty").size());
}

TEST(EntityManager, Exists) {
	EventManager eventManager;
	EntityManager manager(&eventManager);

	Entity myObject = manager.Create("myObject");
	
	EXPECT_EQ(true, manager.Exists(myObject));
	EXPECT_EQ(true, manager.Exists("myObject"));
	EXPECT_EQ(false, manager.Exists(myObject + 1));
	EXPECT_EQ(false, manager.Exists("mySecondObject"));
}