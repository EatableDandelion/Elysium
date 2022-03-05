#pragma once
#include <chrono>
#include <ratio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include "RenderingEngine.h"
#include "Input.h"

namespace Elysium
{
	class Game
	{
		public:
			virtual void init(RenderingEngine& renderingEngine) = 0;

			virtual void update() = 0;

			std::shared_ptr<Input> getInput();

			void pollInput();

			bool isTerminated() const;

			Shader newShader(const std::string& name);
			Mesh newMesh(const std::string& name);
			Texture newTexture(const std::string& name);
			Model newModel(const std::string& name);
			Model newSprite(const std::string& textureName);

		protected:
			std::shared_ptr<Input> m_input;
			ResourceManager<Shader, ShaderLoader> m_shaders;
			ResourceManager<Mesh, MeshLoader> m_meshes;
			ResourceManager<Texture, TextureLoader> m_textures;
			ResourceManager<Model, ModelLoader> m_models;
	};

	class GameLoop
	{
		public:
			GameLoop(const std::string& name, const int width,
				 	 const int height, const std::shared_ptr<Game> game);

			void start();

		private:
			RenderingEngine m_renderingEngine;
			std::shared_ptr<Game> m_game;
			bool m_running;

			void run();
	};
}
