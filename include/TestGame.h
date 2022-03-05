#pragma once
#include <iostream>
#include <Circe/Circe.h>
#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Lights.h"
#include "Assets.h"
#include "RenderingEngine.h"
#include "Event.h"
#include "Player.h"
#include "Input.h"
#include "Game.h"

class TestGame : public Elysium::Game
{
	public:	
		virtual void init(Elysium::RenderingEngine& renderingEngine);

		virtual void update();

	private:
		Elysium::Model sprite;
		Elysium::Model cyborg;
		std::shared_ptr<Elysium::Mover> mover;
};
