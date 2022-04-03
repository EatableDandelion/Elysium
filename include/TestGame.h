#pragma once
#include <iostream>
#include <Circe/Circe.h>
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
#include "Game/Event.h"
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
		virtual void init(Context& context);

		virtual void update(const Real dt, std::vector<Entity>& entities,
							Context& context);

	private:
		std::shared_ptr<Mover> mover;
		Entity sprite1, sprite2, sprite3;
		Physics::Shape s1, s2;
		Physics::Simplex simplex;
};
