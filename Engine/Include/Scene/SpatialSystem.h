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
		class DYNLIB SpatialSystem
		{
		public:
			SpatialSystem();
			~SpatialSystem();

			void AttachSpatialComponent(Entity entity, SpatialComponent component);
			void DetachSpatialComponent(Entity entity);

			void SetPosition(Entity entity, float x, float y, float z);
			void SetOrientation(Entity entity, float yaw, float pitch, float roll);
			void SetScale(Entity entity, float scaleX, float scaleY, float scaleZ);

			SpatialComponent& GetSpatialComponent(Entity entity);

		private:
			Util::PackedPool<Entity, SpatialComponent> mComponents;
		};
	}
}

#endif // _MONSOON_SPATIAL_SYSTEM_H_