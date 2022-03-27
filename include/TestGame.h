#pragma once
#include <iostream>
#include <Circe/Circe.h>
#include "Rendering/Display.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"
#include "Rendering/Model.h"
#include "Game/Camera.h"
#include "Rendering/Lights.h"
#include "Rendering/Assets.h"
#include "Rendering/RenderingEngine.h"
#include "Game/Event.h"
#include "Game/Player.h"
#include "Game/Input.h"
#include "Game/Game.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/Constraint.h"
#include "Physics/Physics.h"

using namespace Elysium;
class TestGame : public Elysium::Game
{
	public:
		TestGame(const std::string& name, 
				 const int width, 
				 const int height);

		virtual void init(RenderingEngine& renderingEngine);

		virtual void update(const Real dt, std::vector<Entity>& entities);

	private:
		std::shared_ptr<Mover> mover;
		Entity sprite1, sprite2, sprite3;
};
