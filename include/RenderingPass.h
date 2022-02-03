#pragma once

#include <GL/glew.h>
#include <vector>
#include <Circe/Circe.h>
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Display.h"
#include "Assets.h"

namespace Elysium
{
	class Renderer
	{
		public:
			Renderer(const int width, const int height, const int nbBuffers);

			Camera getCamera() const;

			Camera& getCamera();

			void readFrameBuffer();	

			void writeFrameBuffer();	

			void writeDepthBuffer(const bool value);

			void readDepthBuffer(const bool value);

			void clear(const float color);

		private:
			Camera m_camera;
			FrameBuffer m_frame;

			/** Prevent object copy */ 
			Renderer(const Renderer& other);

			Renderer& operator=(const Renderer& other);
	};

	class RenderingPass
	{
		public:
			/** Override this to initialize constant shader uniforms */
			virtual void init(Renderer& renderer, Shader& shader) = 0;

			/** Set up the pass, draw all the models, finish pass */
			virtual void draw(Renderer& renderer, Shader& shader, 
							  const std::vector<Model>& models) = 0;
	};

	class GeometryPass : public RenderingPass
	{
		public:
			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(Renderer& renderer, Shader& shader, 
						  	  const std::vector<Model>& models);
	};

	class FinalPass : public RenderingPass
	{
		public:
			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(Renderer& renderer, Shader& shader, 
						  	  const std::vector<Model>& models);

		private:
			Mesh screen;
	};
}
