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

namespace Elysium
{
	class Game;

	class Context
	{
		public:
			Context();

			Shader newShader(const std::string& name);

			static Mesh NewMesh(const std::string& name);

			Texture newTexture(const std::string& name);

			Model newModel(const std::string& name);

			std::shared_ptr<Input> getInput();

			void setShadersDirectory(const std::string& name);
			void setModelsDirectory(const std::string& name);
			void setTexturesDirectory(const std::string& name);

		private:
			ResourceManager<Shader, ShaderLoader> m_shaders;
			static ResourceManager<Mesh, GeometryLoader> m_geometries;
			ResourceManager<Texture, TextureLoader> m_textures;
			ResourceManager<Model, ModelLoader> m_models;			
			std::shared_ptr<Input> m_input;
	};

	class Game
	{
		public:
			virtual void init(Context& context) = 0;

			/** User-hook for regular update */ 
			virtual void update(const Real dt,
								std::vector<Entity>& entities,
								Context& context) = 0;

			/** Calls the update on every system + user-hook update */ 
			void update(const Real dt, Context& context);

			void addSystem(const std::shared_ptr<System> system);

			Entity newEntity();

			Entity newSprite(const std::string& textureName);

			static void SetRenderer
						(const std::shared_ptr<RenderingEngine> renderer);

			static std::shared_ptr<RenderingEngine> Renderer();

		private:
			static std::shared_ptr<RenderingEngine> m_rendering;
			std::vector<Entity> m_entities;
			std::vector<std::shared_ptr<System>> m_systems;
	};

	class GameLoop
	{
		public:
			GameLoop(const std::shared_ptr<Game> game);

			void start();

		private:
			Context m_context;
			std::shared_ptr<Game> m_game;
			bool m_running;
			void run();
	};
}
