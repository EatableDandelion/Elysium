#pragma once
#include <chrono>
#include <ratio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include "Rendering/RenderingEngine.h"
#include "Game/Input.h"
#include "Game/ECS.h"
#include "Game/Debug.h"
#include "Game/World.h"

using Elysium::EntityID;
using Elysium::ComponentID;
using Elysium::Entity;
using Elysium::GameInterface;

namespace Elysium
{
	class Context : public GameInterface, 
					public std::enable_shared_from_this<Context>
	{
		public:
			Context(std::shared_ptr<Input> input);

			virtual Entity getEntity(const EntityID id) const;

			Entity newEntity();

			void removeEntity(const EntityID entity);

			virtual std::map<EntityID, Entity> getEntities() const;

			virtual void onComponentAdded(const EntityID entityID,
								 	  	  const ComponentID cID);

			virtual void onComponentRemoved(const unsigned int entityID,
									 		const ComponentID cID);

			Shader newShader(const std::string& name);

			Mesh newMesh(const std::string& name);

			Texture newTexture(const std::string& name);

			Model newModel(const std::string& name);

			std::shared_ptr<Input> getInput();

			void setShadersDirectory(const std::string& name);

			void setModelsDirectory(const std::string& name);

			void setTexturesDirectory(const std::string& name);

			std::shared_ptr<Renderer> getRenderer() const;

			void setRenderer(const std::shared_ptr<Renderer> renderer);

			void addSystem(const std::shared_ptr<System> system);

			std::vector<std::shared_ptr<System>> getSystems() const;

		private:
			std::map<EntityID, Entity> m_entities;
			std::vector<std::shared_ptr<System>> m_systems;
			std::shared_ptr<Input> m_input;
			std::shared_ptr<Renderer> m_renderer;
			ResourceManager<Shader, ShaderLoader> m_shaders;
			ResourceManager<Mesh, GeometryLoader> m_geometries;
			ResourceManager<Texture, TextureLoader> m_textures;
			ResourceManager<Model, ModelLoader> m_models;			
	};

	class Game
	{
		public:
			Game();

			void init();

			virtual void init(std::shared_ptr<Context> context) = 0;

			/** User-hook for regular update */ 
			virtual void update(const Real dt,
								std::shared_ptr<Context> context){};

			/** Calls the update on every system + user-hook update */ 
			void updateAll(const Real dt);

			void draw();

			bool isTerminated();

		private:
			std::shared_ptr<Context> m_context;
	};
}
