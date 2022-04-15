#pragma once
#include <memory>
#include "Rendering/RenderingPass.h"


class DebugFunctions
{
	public:
		void setDebugRenderer(const std::shared_ptr<Elysium::DebugPass> 
															debugDrawer);	

		void drawLine(const Vec& p1, const Vec& p2, 
					  const Vec3& color = Vec3(1,1,1));

		void drawVector(const Vec& position, const Vec& vec, 
				   const Real scale = 1.0, const Vec3& color = Vec3(1,1,1));

		void drawEllipse(const Vec& position, const Vec& scale, 
		    			 const Vec3& color = Vec3(1,1,1), 
						 const Quaternion& q = Quaternion());

		void drawBox(const Transform transform, 
					 const Vec3& color = Vec3(1,1,1));

		void drawBox(const Vec& center, const Vec& halfWidth, 
					  const Vec3& color = Vec3(1,1,1));


	private:
		std::shared_ptr<Elysium::DebugPass> m_pass;
};

extern DebugFunctions Debug;
