#include "Rendering/RenderingComponent.h"

namespace Elysium
{
	RenderingComponent::RenderingComponent(const Model& model)
		:m_model(model)
	{}

	void RenderingComponent::update(Entity& entity,
									std::shared_ptr<GameInterface> game)
	{
		game->getRenderer()->draw(m_model, entity->getTransform());
	}

	Model RenderingComponent::getModel() const
	{
		return m_model;
	}
}
