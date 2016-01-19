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
		template<class Component>
		class System
		{
		public:
			System(Event::EventManager* eventManager)
				: mEventManager(eventManager) {

				mEventListener = mEventManager->Subscribe("Entity::Destroyed", [this](void* arg) {
					this->DetachComponent((Monsoon::Entity)arg);
					return 0;
				});
			}
			~System() {
				mEventManager->Unsubscribe("Entity::Destroyed", mEventListener);
			}

			void AttachComponent(Monsoon::Entity entity, Component component) {
				if (mComponents.Exists(entity))
					mComponents[entity] = component;
				else
					mComponents.Add(entity, component);

				OnComponentAttached(entity, component);
			};

			boost::optional<Component&> GetComponent(Monsoon::Entity entity) {
				if (mComponents.Exists(entity))
					return boost::optional<Component&>(mComponents[entity]);
				else
					return boost::optional<Component&>();
			}
			
			void DetachComponent(Monsoon::Entity entity) {
				if (!mComponents.Exists(entity))
					return;

				OnComponentDetached(entity, mComponents[entity]);
				mComponents.Remove(entity);
			};

		protected:
			virtual void OnComponentAttached(Monsoon::Entity& entity, Component& component) { };
			virtual void OnComponentDetached(Monsoon::Entity& entity, Component& component) { };

			Util::PackedPool<Monsoon::Entity, Component> mComponents;
		private:
			Event::EventManager* mEventManager;
			Event::ListenerHandle mEventListener;
		};
	}
}

#endif // MONSOON_SYSTEM_H_