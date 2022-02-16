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
#include "Input.h"

namespace Elysium
{
	typedef std::uint8_t RenderBit;

	class Renderer
	{
		public:
			Renderer(const int width, 
					 const int height, 
					 const int nbBuffers);

			Renderer(const Renderer&) = delete;
			Renderer& operator=(const Renderer&) = delete;

			Camera& getCamera();

	//		Input& getInput();

			void enable(const RenderBit kw);

			void disable(const RenderBit kw);

			void clear(const float color, const bool diffuse = true, 
					   const bool depth = true, const bool stencil = true);

			void applyDepthBuffer();

			Circe::Vec2 getScreenSize() const;

			static constexpr RenderBit BLEND		{1 << 0};
			static constexpr RenderBit DEPTH_READ	{1 << 1};
			static constexpr RenderBit DEPTH_WRITE	{1 << 2};
			static constexpr RenderBit STENCIL_READ	{1 << 3};
			static constexpr RenderBit STENCIL_WRITE{1 << 4};
			static constexpr RenderBit FRAMEBUFFER	{1 << 5};
			static constexpr RenderBit FACE_CULLING	{1 << 6};

		private:
			Camera m_camera;
			FrameBuffer m_frame;
			Circe::Vec2 m_screenSize;
	};

	class RenderingPass
	{
		public:
			/** Override this to initialize constant shader uniforms */
			virtual void init(Renderer& renderer, Shader& shader) = 0;

			/** Set up the pass, draw all the models, finish pass */
			virtual void draw(Renderer& renderer, Shader& shader) = 0;
	};

	class GeometryPass : public RenderingPass
	{
		public:
			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(Renderer& renderer, Shader& shader);
			
			void addModel(const Model& model);

		private:
			std::vector<Model> m_models;
	};
}
