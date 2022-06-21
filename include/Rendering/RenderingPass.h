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

	class RenderContext
	{
		public:
			RenderContext(const int width, 
					 	  const int height, 
					 	  const int nbBuffers);

			RenderContext(const RenderContext&) = delete;
			RenderContext& operator=(const RenderContext&) = delete;

			Camera& getCamera();

			void enable(const RenderBit kw);

			void disable(const RenderBit kw);

			void clear(const Real color, const bool diffuse = true, 
					   const bool depth = true, const bool stencil = true);

			void applyDepthBuffer();

			void writeFBO();

			void readFBO();

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
			virtual void init(RenderContext& renderer, Shader& shader)
			{}

			/** Set up the pass, draw all the models, finish pass */
			virtual void draw(RenderContext& renderer,
							  Shader& shader) = 0;
	

			/** Draw a single model outside of the main pipeline */
			virtual void draw(Model& model,
							  const Transform transform, 
							  RenderContext& renderer,
							  Shader& shader){};

	};

	class GeometryPass : public RenderingPass
	{
		public:

			void draw(Model& model,
					  const Transform transform, 
					  RenderContext& renderer,
					  Shader& shader);

			virtual void draw(RenderContext& renderer,
							  Shader& shader);

		private:
			bool wasInit = false;
	};

	struct DebugObject
	{
		Mesh m_mesh;
		Transform m_transform;
		Vec3 m_color;

		DebugObject();
	};

	class DebugPass : public RenderingPass
	{
		public:

			virtual void draw(RenderContext& renderer,
							  Shader& shader);

			void drawLine(const Vec& p1, const Vec& p2,
						  const Vec3& color = Vec3(1,1,1)); 

			void drawVector(const Vec& position, const Vec& vec,
							const Real scale = 1.0, 
							const Vec3& color = Vec3(1,1,1)); 

			void drawBox(const Transform transform,
						 const Vec3& color = Vec3(1,1,1));

			void drawBox(const Vec& center, const Vec& halfWidth,
						 const Vec3& color = Vec3(1,1,1)); 

			void drawEllipse(const Transform transform,
						 	 const Vec3& color = Vec3(1,1,1));

			void drawEllipse(const Vec& position,
							 const Vec& scale, 
							 const Vec3& color = Vec3(1,1,1),
							 const Quaternion& q = Quaternion());

		private:
			std::stack<DebugObject> m_objects;
			ResourceManager<Mesh, GeometryLoader> m_geometries;
	};
}
