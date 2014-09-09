/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <Scene/SpatialSystem.h>

using namespace Monsoon;
using namespace Monsoon::Scene;

SpatialSystem::SpatialSystem()
{

}

SpatialSystem::~SpatialSystem()
{

}

void SpatialSystem::AttachSpatialComponent(Entity entity, SpatialComponent component)
{
	if (!mComponents.Exists(entity))
		mComponents.Add(entity, component);
	else
		mComponents[entity] = component;
}

void SpatialSystem::DetachSpatialComponent(Entity entity)
{
	mComponents.Remove(entity);
}

void SpatialSystem::SetPosition(Entity entity, float x, float y, float z)
{
	if (!mComponents.Exists(entity))
		return;

	auto component = mComponents[entity];
	component.x = x;
	component.y = y;
	component.z = z;
}

void SpatialSystem::SetOrientation(Entity entity, float yaw, float pitch, float roll)
{
	if (!mComponents.Exists(entity))
		return;

	auto component = mComponents[entity];
	component.yaw = yaw;
	component.pitch = pitch;
	component.roll = roll;
}

void SpatialSystem::SetScale(Entity entity, float scaleX, float scaleY, float scaleZ)
{
	if (!mComponents.Exists(entity))
		return;

	auto component = mComponents[entity];
	component.scaleX = scaleX;
	component.scaleY = scaleY;
	component.scaleZ = scaleZ;
}

SpatialComponent& SpatialSystem::GetSpatialComponent(Entity entity)
{
	return mComponents[entity];
}