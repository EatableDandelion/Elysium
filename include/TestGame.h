#pragma once
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <Circe/Circe.h>
#include "Game/GameLoop.h"
#include "Rendering/Display.h"
#include "Rendering/Shader.h"
#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"
#include "Rendering/Model.h"
#include "Rendering/RenderingComponent.h"
#include "Game/Camera.h"
#include "Rendering/Lights.h"
#include "Rendering/Assets.h"
#include "Rendering/RenderingEngine.h"
#include "Game/Player.h"
#include "Game/Input.h"
#include "Game/Game.h"
#include "Physics/PhysicsComponent.h"
#include "Physics/Constraint.h"
#include "Physics/Physics.h"
#include "Physics/CollisionDetection.h"

using namespace Elysium;


class TestGame : public Elysium::Game
{
	public:
		virtual void init(std::shared_ptr<Context> context);

		virtual void update(const Real dt,std::shared_ptr<Context> context);

	private:
		std::shared_ptr<Mover> mover;
		Entity sprite1, sprite2, sprite3;
};
