/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_SPATIAL_SYSTEM_H_
#define _MONSOON_SPATIAL_SYSTEM_H_

#include <vector>

#include <Platform/Types.h>
#include <Util/PackedPool.h>
#include <Platform/Export.h>
#include <ECS/System.h>
#include <Math/Vector.h>

namespace Monsoon
{
	namespace Scene
	{
		struct TransformComponent
		{
			TransformComponent()
				: position(0.0f, 0.0f, 0.0f)
				, scale(1.0f, 1.0f, 1.0f)
			{
				yaw = 0.0f;
				pitch = 0.0f;
				roll = 0.0f;
			}

			Math::Vector3 position;
			float yaw, pitch, roll;
			Math::Vector3 scale;
		};

		/**
		 * Manages entity spatial information, such as position, orientation, and scale.
		 * Provides functions to translate, rotate, and change spaces.
		 */
		class DYNLIB TransformSystem : public ECS::System<TransformComponent>
		{
		public:
			TransformSystem(Event::EventManager* eventManager);
			~TransformSystem();

			void Translate(Entity entity, Math::Vector3 delta);

			//
			// Transforms the target's local basis axis into world
			// space coordinates.
			//
			Math::Vector3 Front(Entity entity);
			Math::Vector3 Up(Entity entity);
			Math::Vector3 Right(Entity entity);
		};
	}
}

#endif // _MONSOON_SPATIAL_SYSTEM_H_