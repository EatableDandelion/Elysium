#pragma once

#include <GL/glew.h>
#include <vector>
#include <stack>
#include <Circe/Circe.h>
#include "Rendering/Model.h"
#include "Rendering/Shader.h"
#include "Game/Camera.h"
#include "Rendering/Texture.h"
#include "Rendering/Display.h"
#include "Rendering/Assets.h"
#include "Game/Input.h"

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

			void enable(const RenderBit kw);

			void disable(const RenderBit kw);

			void clear(const Real color, const bool diffuse = true, 
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
			Mesh sprite;
	};

	class RenderingPass
	{
		public:
			/** Override this to initialize constant shader uniforms */
			virtual void init(Renderer& renderer, Shader& shader) = 0;

			/** Set up the pass, draw all the models, finish pass */
			virtual void draw(std::vector<Entity>& entities,
							  Renderer& renderer,
							  Shader& shader) = 0;
	};

	class GeometryPass : public RenderingPass
	{
		public:
			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(std::vector<Entity>& entities,
							  Renderer& renderer,
							  Shader& shader);
	};

	struct DebugObject
	{
		Mesh m_mesh;
		Transform m_transform;
	};

	class DebugPass : public RenderingPass
	{
		public:
			virtual void init(Renderer& renderer, Shader& shader);

			virtual void draw(std::vector<Entity>& entities,
							  Renderer& renderer,
							  Shader& shader);

			void drawLine(const Vec& p1, const Vec& p2); 

		private:
			std::stack<DebugObject> m_objects;
	};
}
