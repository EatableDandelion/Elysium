#pragma once
#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <Circe/Circe.h>
#include "Rendering/RenderingEngine.h"
#include "Game/Register.h"

namespace Elysium
{
	typedef unsigned int EntityID;

	class Context;
	class EntityData;
	using Entity = std::shared_ptr<EntityData>;

	class Component
	{
		public:
			virtual void update(Entity& entity, const Real dt);

			virtual void draw(Entity& entity,
							  std::shared_ptr<Renderer> renderer);

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

			void update(Entity& entity, std::shared_ptr<Renderer> renderer,
						const Real dt);

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
		
			Transform getTransform();

			EntityID getID() const;

			template<typename T>
			void setVariable(const std::string& name, const T& var)
			{
				m_register.setVariable(name, var);
			}

			std::vector<Real> getVariable(const std::string& name);

			bool hasVariable(const std::string& name) const;

		private:
			Register m_register;
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

	class World;

	class System
	{
		public:
			virtual void update(const Real dt, 
								World& world,
								Context& context) = 0;
	};
}
