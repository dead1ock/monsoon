/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_ANIMATION_SYSTEM_H_
#define _MONSOON_ANIMATION_SYSTEM_H_

#include <Renderer/Renderer.h>
#include <Platform/Clock.h>
#include <Util/PackedPool.h>

#include <map>

namespace Monsoon
{
	namespace Renderer
	{
		typedef U16 SpriteAnimationHandle;

		struct SpriteAnimationFrame
		{
			SpriteAnimationFrame(TextureHandle texture, AtlasSheetHandle atlas, int atlasIndex)
			{
				Texture = texture;
				Atlas = atlas;
				AtlasIndex = atlasIndex;
			}

			TextureHandle Texture;
			AtlasSheetHandle Atlas;
			int AtlasIndex;
		};

		struct SpriteAnimation
		{
			SpriteAnimation(std::string name, double framerate, std::vector<SpriteAnimationFrame> frames)
			{
				Name = name;
				Framerate = framerate;
				Frames = frames;
			}

			std::string Name;
			double Framerate;
			std::vector<SpriteAnimationFrame> Frames;
		};

		struct SpriteAnimationComponent
		{
			SpriteAnimationComponent(SpriteAnimationHandle animation)
			{
				lastFrameTimestamp = 0.0f;
				currentFrame = 0;
				currentAnimation = animation;
			}

			double lastFrameTimestamp;
			int currentFrame;
			SpriteAnimationHandle currentAnimation;
		};

		/**
		 * Handles creation and playback of sprite/mesh animations.
		 */
		class DYNLIB AnimationSystem
		{
		public:
			AnimationSystem(Platform::Clock* gameClock, Monsoon::Renderer::Renderer* renderer);
			virtual ~AnimationSystem();

			SpriteAnimationHandle CreateAnimation2d(SpriteAnimation animation);
			void ReleaseAnimation2d(SpriteAnimationHandle animationHandle);

			void Play2d(Entity entity, SpriteAnimationHandle animationHandle);
			void Stop2d(Entity entity);

			void Update();

		private:
			std::vector<SpriteAnimation> mSpriteAnimations;
			std::vector<SpriteAnimationHandle> mSpriteAnimationsFreelist;

			Util::PackedPool<Entity, SpriteAnimationComponent> mSpriteAnimationComponents;
			
			Platform::Clock* mGameClock;
			Renderer* mRenderer;
		};
	}
}

#endif // _MONSOON_ANIMATION_SYSTE_H_