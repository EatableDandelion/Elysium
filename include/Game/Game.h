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
	class Game
	{
		public:
			Game(const std::string& name, 
				 const int width, 
				 const int height);

			/** User-hook for initialization */ 
			virtual void init(RenderingEngine& renderingEngine) = 0;

			void init();
			
			/** User-hook for regular update */ 
			virtual void update(const Real dt,
								std::vector<Entity>& entities) = 0;

			/** Calls the update on every system + user-hook update */ 
			void update(const Real dt);

			void draw();

			std::shared_ptr<Input> getInput();

			void pollInput();

			bool isTerminated() const;

			Shader newShader(const std::string& name);
			Mesh newMesh(const std::string& name);
			Mesh newGeometry(const std::string& name);
			Texture newTexture(const std::string& name);
			Model newModel(const std::string& name);
			Entity newEntity();
			Entity newSprite(const std::string& textureName);

			void addSystem(const std::shared_ptr<System> system);

		protected:
			std::shared_ptr<Input> m_input;
			ResourceManager<Shader, ShaderLoader> m_shaders;
			ResourceManager<Mesh, MeshLoader> m_meshes;
			ResourceManager<Mesh, GeometryLoader> m_geometries;
			ResourceManager<Texture, TextureLoader> m_textures;
			ResourceManager<Model, ModelLoader> m_models;

		private:
			std::vector<Entity> entities;
			std::vector<std::shared_ptr<System>> systems;
			RenderingEngine m_renderingEngine;
	};

	class GameLoop
	{
		public:
			GameLoop(const std::shared_ptr<Game> game);

			void start();

		private:
			std::shared_ptr<Game> m_game;
			bool m_running;
			void run();
	};
}
