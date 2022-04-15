#include "Rendering/RenderingComponent.h"

namespace Elysium
{
	RenderingComponent::RenderingComponent(const Model& model)
		:m_model(model)
	{}

	void RenderingComponent::draw(Entity& entity, 
								  std::shared_ptr<Renderer> renderer)
	{
		renderer->draw(m_model, entity->getTransform());
	}

	Model RenderingComponent::getModel() const
	{
		return m_model;
	}
}
