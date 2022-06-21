#include "TestGame.h"

void TestGame::init(std::shared_ptr<Context> context)
{
	using namespace Elysium;

	context->setRenderer(std::make_shared<Renderer>("Elysium",800,600,3));

	context->setShadersDirectory("../res/shaders/");
	context->setTexturesDirectory("../res/textures/");
	context->setModelsDirectory("../res/models/");

	context->setPhysicsEngine(std::make_shared<Physics::PhysicsEngine>());
	context->addSystem(std::make_shared<Carina::ThrusterSystem>());
	context->addSystem(std::make_shared<Elysium::CameraSystem>());


	context->getRenderer()->addGeometryPass<GeometryPass>
						   (context->newShader("geometry"));

	context->getRenderer()->addPostProcessPass<DirectionalLightPass>(
							context->newShader("directionalLight"), 
					 		context->newMesh("sprite"));

	context->getRenderer()->addPostProcessPass<AmbientPass>(
							context->newShader("screen"),
						    context->newMesh("sprite"), 0.0f);

	context->getRenderer()->addPostProcessPass<DebugPass>(
							context->newShader("debug"));

	Debug.setDebugRenderer(context->getRenderer()->getPass<DebugPass>());

	context->getRenderer()
		   ->getPass<DirectionalLightPass>()
		   ->addLight(DirectionalLight(Vec3(0.5,0,-1), 
									   Vec3(1.0,1.0,1.0), 1.0f));

	context->getRenderer()->getCamera().getTransform()
						  ->translate(Vec3(0,0,-10));

	sprite1 = context->newEntity();

	sprite1->addComponent<Elysium::CameraComponent>();
	sprite1->addComponent<RenderingComponent>(
						Model(context->newMesh("sprite"), 
							  context->newTexture("bricks2.png")));

//	sprite1->getTransform()->translate(Vec(1.5,7.7));

	std::vector<Vec> collisionPoints({Vec(-1,-1),Vec(-1,1),
									  Vec(1,1),Vec(1,-1)});
	
	sprite1->addComponent<Physics::PhysicsComponent>(10.0,
						   sprite1->getTransform(), collisionPoints);

	sprite1->addComponent<Carina::ThrusterComponent>();

	sprite1->getComponent<Carina::ThrusterComponent>()->addThruster
					(Carina::Thruster(Vec(0,-1),Vec2(0,1), 50.0, false));
	sprite1->getComponent<Carina::ThrusterComponent>()->addThruster
					(Carina::Thruster(Vec(-1,0),Vec2(0,1), 10.0, true));
	sprite1->getComponent<Carina::ThrusterComponent>()->addThruster
					(Carina::Thruster(Vec(1,0),Vec2(0,1), 10.0, true));
	sprite1->getComponent<Carina::ThrusterComponent>()->addThruster
					(Carina::Thruster(Vec(0,1),Vec2(1,0), 10.0, true));

	sprite1->getTransform()->rotate(2.0);

	Entity planet = context->newEntity();

	planet->getTransform()->translate(Vec2(3,-58));
	planet->getTransform()->resize(5.0);

	planet->addComponent<RenderingComponent>(
						Model(context->newMesh("sprite"), 
							  context->newTexture("bricks2.png")));
	planet->addComponent<Physics::PhysicsComponent>(10.0, 
		 planet->getTransform(),
		 Physics::ColliderFromMesh(m_geometries.newRectangle(false)));


	context->getInput()->addActionToKey(Input::KEY_SPACE, "main thruster");
	context->getInput()->addActionToKey(Input::KEY_A, "move left");
	context->getInput()->addActionToKey(Input::KEY_D, "move right");
	context->getInput()->addActionToKey(Input::KEY_S, "move backward");
	context->getInput()->addActionToKey(Input::KEY_W, "move forward");
	context->getInput()->addActionToKey(Input::KEY_Q, "steer left");
	context->getInput()->addActionToKey(Input::KEY_E, "steer right");
	context->getInput()->addActionToKey(Input::KEY_E, "steer right");
	context->getInput()->addActionToKey(Input::KEY_E, "steer right");
	context->getInput()->addActionToKey(Input::KEY_KP_MINUS, "zoom out");
	context->getInput()->addActionToKey(Input::KEY_KP_PLUS, "zoom in");
}

void TestGame::update(std::shared_ptr<Context> context)
{}

