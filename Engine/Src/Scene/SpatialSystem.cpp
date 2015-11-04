/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <Scene/SpatialSystem.h>

using namespace Monsoon;
using namespace Monsoon::Scene;

SpatialSystem::SpatialSystem(Event::EventManager* eventManager)
: System(eventManager)
{

}

SpatialSystem::~SpatialSystem()
{

}

void SpatialSystem::SetPosition(Entity entity, float x, float y, float z)
{
	if (!mComponents.Exists(entity))
		return;

	auto& component = mComponents[entity];
	component.position.mX = x;
	component.position.mY = y;
	component.position.mZ = z; 
}

void SpatialSystem::SetOrientation(Entity entity, float yaw, float pitch, float roll)
{
	if (!mComponents.Exists(entity))
		return;

	auto& component = mComponents[entity];
	component.yaw = yaw;
	component.pitch = pitch;
	component.roll = roll;
}

void SpatialSystem::SetScale(Entity entity, float scaleX, float scaleY, float scaleZ)
{
	if (!mComponents.Exists(entity))
		return;

	auto& component = mComponents[entity];
	component.scaleX = scaleX;
	component.scaleY = scaleY;
	component.scaleZ = scaleZ;
}

void SpatialSystem::Translate(Entity entity, Math::Vector3 delta)
{
	if (!mComponents.Exists(entity))
		return;

	auto& component = mComponents[entity];
	component.position += delta;
}