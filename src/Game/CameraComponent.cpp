#include "Game/CameraComponent.h"

namespace Elysium
{
	CameraSystem::CameraSystem()
	{
		System::addComponentType<CameraComponent>();
	}

	void CameraSystem::update(std::shared_ptr<GameInterface> game)
	{
		Real shift = 0.0;

		if(game->getInput()->isActive("zoom out"))
		{
			shift -= 1.0; 
		}
		if(game->getInput()->isActive("zoom in"))
		{
			shift += 1.0; 
		}

		for(EntityID entityID : System::m_entities)
		{
			Vec3 camPos = game->getCameraPosition();
			Vec pos(0,0);// = game->getEntity(entityID)->getPosition();
	

			game->setCameraPosition(Vec3(pos(0), pos(1), camPos(2)+shift));	
		}
	}
}
