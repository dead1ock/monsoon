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

}

TEST(EntityManager, RemoveTag) {

}

TEST(EntityManager, InvalidFindByTag) {

}

TEST(EntityManager, FindByTag) {

}