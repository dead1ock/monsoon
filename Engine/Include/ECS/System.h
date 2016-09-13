/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef MONSOON_SYSTEM_H_
#define MONSOON_SYSTEM_H_

#include <Platform/Types.h>
#include <Util/PackedPool.h>
#include <Event/EventManager.h>
#include <boost/optional.hpp>

namespace Monsoon {
	namespace ECS {

		/**
		 * System base class for managing a certain component type. This base class
		 * owns the Component objects of the system (in a PackedPool container), 
		 * manages automatic destruction of components when an associated entity is destroyed,
		 * and allows the user to attach, detach, and lookup components.
		 */
		template<class Component>
		class System
		{
		public:
			/**
			 * Default constructor.
			 *
			 * @param eventManager 
			 */
			System(Event::EventManager* eventManager)
				: mEventManager(eventManager) {

				mEntityDestroyedEventListener = mEventManager->Subscribe("Entity::Destroyed", [this](void* arg) {
					this->DetachComponent((Monsoon::Entity)arg);
					return 0;
				});
			}

			/**
			 * Default destructor.
			 */
			~System() {
				mEventManager->Unsubscribe("Entity::Destroyed", mEntityDestroyedEventListener);
			}

			/**
			 * Associates a component object with the target entity handle, and then
			 * notifies the inhierted class that a new component has been attached via
			 * OnComponentAttached(Entity, Component).
			 *
			 * @param entity The entity handle to associate the component with.
			 * @param component The component object to attach.
			 */
			void AttachComponent(Monsoon::Entity entity, Component component) {
				if (mComponents.Exists(entity))
					mComponents[entity] = component;
				else
					mComponents.Add(entity, component);

				OnComponentAttached(entity, component);
			};
			
			/**
			 * If the target entity has an associated component in this system, remove
			 * the component and notify the inhierted class via OnComponentDetached.
			 *
			 * @param entity The target entity 
			 */
			void DetachComponent(Monsoon::Entity entity) {
				if (!mComponents.Exists(entity))
					return;

				OnComponentDetached(entity, mComponents[entity]);
				mComponents.Remove(entity);
			};

			/**
			 * Checks if the target entity has an associated system component.
			 *
			 * @param entity The target entity.
			 * @return bool True if the component exists for the target entity, otherwise false.
			 */
			bool IsValid(Monsoon::Entity entity) {
				return mComponents.Exists(entity);
			}

		protected:
			/**
			 * Invoked when a new component is attached to an entity.
			 *
			 * @param entity The target entity handle.
			 * @param component The component object which was attached.
			 */
			virtual void OnComponentAttached(Monsoon::Entity& entity, Component& component) { };
			
			/**
			 * Invoked when a component is being detached from an entity.
			 *
			 * @param entity The target entity handle.
			 * @param component The component object which was detached.
			 */
			virtual void OnComponentDetached(Monsoon::Entity& entity, Component& component) { };

			Util::PackedPool<Monsoon::Entity, Component> mComponents;
			Event::EventManager* mEventManager;

		private:
			Event::ListenerHandle mEntityDestroyedEventListener;
		};
	}
}

#endif // MONSOON_SYSTEM_H_