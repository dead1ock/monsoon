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
			Entity parent;
		};

		/**
		 * Manages entity spatial information, such as position, orientation, and scale.
		 * Provides functions to translate, rotate, and change spaces.
		 */
		class DYNLIB TransformSystem : public ECS::System<TransformComponent>
		{
		public:

			/**
			 * Default constructor.
			 * @param eventManager The event manager to subscribe to for global events.
			 */
			TransformSystem(Event::EventManager* eventManager);

			/**
			 * Default destructor.
			 */
			~TransformSystem();

			/**
			 * Translates the target entity by the amounts in the delta vector.
			 * @param entity The entity to translate.
			 * @param delta The amount to translate by.
			 */
			void Translate(Entity entity, Math::Vector3 delta);

			/**
			 * Rotates the target entity by the amounts (axis angles, in degrees) in the eulers vector.
			 * @param entity The entity to translate.
			 * @param eulers The degrees to rotate on each axis.
			 */
			void Rotate(Entity entity, Math::Vector3 eulers);
			void Scale(Entity entity, Math::Vector3 scalars);

			/*
			 * Position in world space.
			 */
			const Math::Vector3& GetPosition(Entity entity);

			void SetPosition(Entity entity, Math::Vector3 position);

			/**
			 * Scale
			 */
			const Math::Vector3& GetScale(Entity entity);

			void SetScale(Entity entity, Math::Vector3 scalars);

			/**
			 * Rotation in world space.
			 */
			const Math::Vector3& GetRotation(Entity entity);

			void SetRotation(Entity entity, Math::Vector3 eulers);

			/**
			 * Sets the parent of the target entity.
			 */
			//void SetParent(Entity child, Entity parent);

			/**
			 * Returns the parent fo the target entity.
			 */
			//Entity GetParent(Entity entity);

			/**
			 * Sets the parent of the target entity. 
			 */
			//void AddChild(Entity parent, Entity child);

			/**
			 * Returns the front vector of the entity in world space.
			 */
			Math::Vector3 Front(Entity entity);

			/**
			 * Returns the up vector of the entity in world space.
			 */
			Math::Vector3 Up(Entity entity);

			/**
			 * Returns the right vector the entity in world space.
			 */
			Math::Vector3 Right(Entity entity);
		};
	}
}

#endif // _MONSOON_SPATIAL_SYSTEM_H_