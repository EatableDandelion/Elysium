#pragma once
#include <Circe/Circe.h>
#include <iostream>
#include "Rendering/Model.h"
#include "Rendering/Shader.h"
#include "Game/ECS.h"
#include "Game/Game.h"

namespace Elysium
{
	class RenderingComponent : public Component
	{
		public:
			RenderingComponent(const Model& model);

			virtual void update(Entity& entity,
								std::shared_ptr<GameInterface> game);

			Model getModel() const;

		private:
			Model m_model;
	};
}
