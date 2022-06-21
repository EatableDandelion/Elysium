#pragma once

#include <unordered_map>
#include <typeinfo>
#include <typeindex>
#include <iostream>
#include <set>
#include <Circe/Circe.h>
#include "Game/GameInterface.h"
#include "Rendering/RenderingEngine.h"

namespace Elysium
{

	typedef unsigned int EntityID;
	typedef std::type_index ComponentID;
	class EntityData;
	using Entity = std::shared_ptr<EntityData>;

//	using ComponentID = std::uint8_t;
//	const ComponentID MAX_COMPONENTS = 256;
//	using Signature = std::bitset<ComponentID>;


	template<class T>
	ComponentID getComponentID()
	{
		return std::type_index(typeid(T));
	}

	template<class T>
	bool isComponentID(const ComponentID id)
	{
		return id == getComponentID<T>();
	}

	class Component
	{
		public:
			virtual void update(Entity& entity, 
								std::shared_ptr<GameInterface> game);

//			virtual void draw(Entity& entity,
//							  std::shared_ptr<Renderer> renderer);

			void setEntityID(const EntityID id);

			EntityID getEntityID() const;

		private:
			EntityID m_id;
	};

	class EntityData
	{
		public:
			EntityData(const std::shared_ptr<GameInterface> comm);
		
			~EntityData();

			EntityData(const EntityData&) = delete;

			void update(Entity& entity, 
						std::shared_ptr<GameInterface> game);

//			void update(Entity& entity, std::shared_ptr<Renderer> renderer,
//						const Real dt);

			template<class T, typename... Args>
			void addComponent(Args&&... args)
			{
				m_components[getComponentID<T>()] = 
					std::make_shared<T>(std::forward<Args>(args)...);

				std::shared_ptr<GameInterface> game =m_gameInterface.lock();
				if(game)
				{
					game->onComponentAdded(id, getComponentID<T>());
				}
			}
			
			template<class T>
			void removeComponent()
			{
				removeComponent(getComponentID<T>());
			}

			void removeComponent(const std::type_index componentID)
			{
				std::shared_ptr<GameInterface> game =m_gameInterface.lock();
				if(game && m_components.count(componentID))
				{
					game->onComponentRemoved(id, componentID);
				}

				m_components.erase(componentID);
			}

			template<class T>
			std::shared_ptr<T> getComponent()
			{
				std::shared_ptr<Component> res = 
							m_components[getComponentID<T>()];
				return std::dynamic_pointer_cast<T>(res);
			}

			template<typename T>
			bool hasComponent()
			{
				return m_components.find(getComponentID<T>()) != 
								m_components.end();
			}

			bool hasComponent(const ComponentID componentID) const;
		
			Transform getTransform();

			Vec getPosition() const;

			EntityID getID() const;

			template<typename T>
			void setVariable(const std::string& name, const T& var)
			{
				m_register.setVariable(name, var);
			}

			std::vector<Real> getVariable(const std::string& name);

			bool hasVariable(const std::string& name) const;

		private:
			Circe::Register m_register;
			static EntityID allid;			
			const EntityID id;
			map<ComponentID, std::shared_ptr<Component>> m_components;	
			Transform m_transform;
			std::weak_ptr<GameInterface> m_gameInterface;
	};


	class System
	{
		public:
			virtual void update(std::shared_ptr<GameInterface> game) = 0;

			virtual void onComponentAdded(Entity entity, 
										  const ComponentID id);

			virtual void onComponentRemoved(Entity entity, 
										  	const ComponentID id);

			template<typename T>
			void addComponentType()
			{
				m_key.insert(getComponentID<T>());
			}

			bool isCompatible(const Entity& entity) const;

			bool hasEntity(const EntityID id) const;

		protected:
			std::set<EntityID> m_entities;

		private:
			std::set<ComponentID> m_key;
	};
}
