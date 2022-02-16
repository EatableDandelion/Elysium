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

		protected:
			std::shared_ptr<Input> m_input;
			ResourceManager<Shader, ShaderLoader> shaders;
			ResourceManager<Mesh, MeshLoader> meshes;
			ResourceManager<Texture, TextureLoader> textures;
			ResourceManager<Model, ModelLoader> models;
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
