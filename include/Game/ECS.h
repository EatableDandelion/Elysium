#pragma once
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <Circe/Circe.h>

namespace Elysium
{
	typedef int EntityID;

	class EntityData;
	using Entity = std::shared_ptr<EntityData>;

	class Component
	{
		public:
			virtual void update(Entity& entity, const Real dt);

			void setEntityID(const EntityID id);

			EntityID getEntityID() const;

		private:
			EntityID m_id;
	};

	class EntityData
	{
		public:
			EntityData();
		
			~EntityData();

			EntityData(const EntityData&) = delete;

			template<class T, typename... Args>
			void addComponent(Args&&... args)
			{
				m_components[getComponentId<T>()] = 
					std::make_shared<T>(std::forward<Args>(args)...);
			}
			
			template<class T>
			std::shared_ptr<T> getComponent()
			{
				std::shared_ptr<Component> res = 
							m_components[getComponentId<T>()];
				return std::dynamic_pointer_cast<T>(res);
			}

			template<typename T>
			bool hasComponent()
			{
				return m_components.find(getComponentId<T>()) != 
								m_components.end();
			}
			
/*			template<typename T1, typename... Ts>
			bool hasComponents()
			{
				return hasComponents<T1>() && hasComponents<Ts...>();
			}
*/
			Transform getTransform();

			EntityID getID() const;

		private:
			static EntityID allid;			
			const EntityID id;
			map<std::type_index, std::shared_ptr<Component>> m_components;	
			Transform m_transform;
	
			template<class T>
			std::type_index getComponentId()
			{
				return std::type_index(typeid(T));
			}
	};

	class System
	{
		public:
			virtual void update(const Real dt, 
								std::vector<Entity>& entities) = 0;
	};
}
