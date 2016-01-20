/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include <Scene/TransformSystem.h>

#include <Math/Matrix4x4.h>

using namespace Monsoon;
using namespace Monsoon::Scene;

TransformSystem::TransformSystem(Event::EventManager* eventManager)
: System(eventManager)
{

}

TransformSystem::~TransformSystem()
{

}

void TransformSystem::Translate(Entity entity, Math::Vector3 delta)
{
	if (!mComponents.Exists(entity))
		return;

	auto& component = mComponents[entity];
	component.position += delta;
}

Math::Vector3 TransformSystem::Up(Entity entity)
{
	if (!mComponents.Exists(entity))
		return Math::Vector3::Up();

	Math::Matrix4x4 globalTransform = Math::Matrix4x4::TransformMatrix(mComponents[entity].position, 
		Math::Vector3(mComponents[entity].pitch, mComponents[entity].yaw, mComponents[entity].roll),
		mComponents[entity].scale);
	Math::Matrix4x4 transform = globalTransform * Math::Vector4(Math::Vector3::Up(), 0.0f);
	return transform.GetRow(0);
}