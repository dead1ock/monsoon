/**
* Copyright (c) 2014 Sonora Games
*
*/

#ifndef _MONSOON_SPATIAL_SYSTEM_H_
#define _MONSOON_SPATIAL_SYSTEM_H_

#include <vector>

#include <Platform/Types.h>
#include <Util/PackedPool.h>
#include <Platform/Export.h>
#include <ECS/System.h>

namespace Monsoon
{
	namespace Scene
	{
		struct SpatialComponent
		{
			SpatialComponent()
			{
				x = 0.0f;
				y = 0.0f;
				z = 0.0f;
				yaw = 0.0f;
				pitch = 0.0f;
				roll = 0.0f;
				scaleX = 1.0f;
				scaleY = 1.0f;
				scaleZ = 1.0f;
			}

			float x, y, z;
			float yaw, pitch, roll;
			float scaleX, scaleY, scaleZ;
		};

		/**
		 * Manages entity spatial information, such as position, orientation, and scale.
		 */
		class DYNLIB SpatialSystem : public ECS::System<SpatialComponent>
		{
		public:
			SpatialSystem(Event::EventManager* eventManager);
			~SpatialSystem();

			void SetPosition(Entity entity, float x, float y, float z);
			void SetOrientation(Entity entity, float yaw, float pitch, float roll);
			void SetScale(Entity entity, float scaleX, float scaleY, float scaleZ);
		};
	}
}

#endif // _MONSOON_SPATIAL_SYSTEM_H_