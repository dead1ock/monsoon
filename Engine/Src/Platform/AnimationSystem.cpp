/**
* Copyright (c) 2014 Sonora Games
*
*/

#include <Renderer/AnimationSystem.h>

using namespace Monsoon;
using namespace Monsoon::Renderer;

AnimationSystem::AnimationSystem(Platform::Clock* gameClock, Monsoon::Renderer::Renderer* renderer)
	: mGameClock(gameClock) ,
	mRenderer(renderer)
{

}

AnimationSystem::~AnimationSystem()
{

}

SpriteAnimationHandle AnimationSystem::CreateAnimation2d(SpriteAnimation animation)
{
	SpriteAnimationHandle handle = mSpriteAnimations.size();
	if (mSpriteAnimationsFreelist.size())
	{
		handle = mSpriteAnimationsFreelist.back();
		mSpriteAnimations[handle] = animation;
		mSpriteAnimations.pop_back();
	}
	else
	{
		mSpriteAnimations.push_back(animation);
	}

	return handle;
}

void AnimationSystem::ReleaseAnimation2d(SpriteAnimationHandle animationHandle)
{
	if (mSpriteAnimations.size() >= animationHandle)
	{
		mSpriteAnimations.erase(mSpriteAnimations.begin() + animationHandle);
		mSpriteAnimationsFreelist.push_back(animationHandle);
	}
}

void AnimationSystem::Play2d(Entity entity, SpriteAnimationHandle animationHandle)
{
	if (mSpriteAnimationComponents.Exists(entity))
	{
		if (mSpriteAnimationComponents[entity].currentAnimation != animationHandle)
			mSpriteAnimationComponents[entity] = SpriteAnimationComponent(animationHandle);
	}
	else
	{
		mSpriteAnimationComponents.Add(entity, SpriteAnimationComponent(animationHandle));
	}
}

void AnimationSystem::Stop2d(Entity entity)
{
	if (mSpriteAnimationComponents.Exists(entity))
	{
		mSpriteAnimationComponents.Remove(entity);
	}
}

void AnimationSystem::Update()
{
	for (int x = 0; x < mSpriteAnimationComponents.Size(); x++) {
		auto& component = mSpriteAnimationComponents.At(x);
		auto& spriteComponent = mRenderer->GetSpriteComponent(mSpriteAnimationComponents.IndexToId(x));
		auto& spriteAnimation = mSpriteAnimations[component.currentAnimation];

		// Update Frame
		if ((mGameClock->getTime() - component.lastFrameTimestamp) >= 1.0f/spriteAnimation.Framerate) {
			component.currentFrame++;

			// Wrap frames if we were at the end of the animation.
			if (component.currentFrame > (spriteAnimation.Frames.size() - 1))
				component.currentFrame = 0;

			component.lastFrameTimestamp = mGameClock->getTime();

			spriteComponent.Texture = spriteAnimation.Frames[component.currentFrame].Texture;
			spriteComponent.AtlasSheet = spriteAnimation.Frames[component.currentFrame].Atlas;
			spriteComponent.AtlasIndex = spriteAnimation.Frames[component.currentFrame].AtlasIndex;
		}
	}
}