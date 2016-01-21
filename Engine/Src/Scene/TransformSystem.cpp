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

void TransformSystem::Rotate(Entity entity, Math::Vector3 eulers)
{
	if (!mComponents.Exists(entity))
		return;

	mComponents[entity].pitch += eulers.mX;
	mComponents[entity].yaw += eulers.mY;
	mComponents[entity].roll += eulers.mZ;
}

void TransformSystem::Scale(Entity entity, Math::Vector3 scale)
{
	if (!mComponents.Exists(entity))
		return;

	mComponents[entity].scale.mX *= scale.mX;
	mComponents[entity].scale.mY *= scale.mY;
	mComponents[entity].scale.mZ *= scale.mZ;
}

const Math::Vector3& TransformSystem::GetPosition(Entity entity)
{
	return mComponents[entity].position;
}

void TransformSystem::SetPosition(Entity entity, Math::Vector3 position)
{
	mComponents[entity].position = position;
}

const Math::Vector3& TransformSystem::GetScale(Entity entity)
{
	return mComponents[entity].scale;
}

void TransformSystem::SetScale(Entity entity, Math::Vector3 scalars)
{
	mComponents[entity].scale = scalars;
}

const Math::Vector3& TransformSystem::GetRotation(Entity entity)
{
	return Math::Vector3(mComponents[entity].pitch, mComponents[entity].yaw, mComponents[entity].roll);
}

void TransformSystem::SetRotation(Entity entity, Math::Vector3 eulers)
{
	mComponents[entity].pitch = eulers.mX;
	mComponents[entity].yaw = eulers.mY;
	mComponents[entity].roll = eulers.mZ;
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

Math::Vector3 TransformSystem::Front(Entity entity)
{
	if (!mComponents.Exists(entity))
		return Math::Vector3::Front();

	Math::Matrix4x4 globalTransform = Math::Matrix4x4::TransformMatrix(mComponents[entity].position,
		Math::Vector3(mComponents[entity].pitch, mComponents[entity].yaw, mComponents[entity].roll),
		mComponents[entity].scale);
	Math::Matrix4x4 transform = globalTransform * Math::Vector4(Math::Vector3::Front(), 0.0f);
	return transform.GetRow(0);
}

Math::Vector3 TransformSystem::Right(Entity entity)
{
	if (!mComponents.Exists(entity))
		return Math::Vector3::Right();

	Math::Matrix4x4 globalTransform = Math::Matrix4x4::TransformMatrix(mComponents[entity].position,
		Math::Vector3(mComponents[entity].pitch, mComponents[entity].yaw, mComponents[entity].roll),
		mComponents[entity].scale);
	Math::Matrix4x4 transform = globalTransform * Math::Vector4(Math::Vector3::Right(), 0.0f);
	return transform.GetRow(0);
}