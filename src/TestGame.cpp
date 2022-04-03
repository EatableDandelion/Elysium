#include "TestGame.h"


void TestGame::init(Context& context)
{
	using namespace Elysium;

	Game::SetRenderer(std::make_shared<RenderingEngine>
											("Elysium", 800, 600, 3));
	
	/*
	Emitter<int> emitter;
	std::shared_ptr<Listener<int>> listener = 
					std::make_shared<Listener<int>>();
	listener->setCallback([](int input)
			{
				std::cout << input << std::endl;
			});
	emitter.addListener(listener);
*/
	
	context.setShadersDirectory("../res/shaders/");
	context.setTexturesDirectory("../res/textures/");
	context.setModelsDirectory("../res/models/");

	Game::Renderer()->setFirstPass<GeometryPass>
						   (context.newShader("geometry"));

	Game::Renderer()->addPass<DirectionalLightPass>(
							context.newShader("directionalLight"), 
					 		Context::NewMesh("sprite"));

	Game::Renderer()->addPass<AmbientPass>(context.newShader("screen"),
								 Context::NewMesh("sprite"), 0.0f);

	Game::Renderer()->addPass<DebugPass>(context.newShader("debug"));


	Game::Renderer()->getPass<DirectionalLightPass>()
					->addLight(DirectionalLight(Vec3(0.5,0,-1), 
												Vec3(1.0,1.0,1.0), 1.0f));

/*
	Entity cyborg = newEntity();
	cyborg->addComponent<RenderingComponent>(
							context.newModel("cyborg/cyborg.obj"));
*/

	Game::Renderer()->getCamera().getTransform()
					   ->translate(Vec3(0,0,-10));

	sprite1 = newEntity();
	sprite1->addComponent<RenderingComponent>(
						Model(Context::NewMesh("sprite"), 
							  context.newTexture("bricks2.png")));

	sprite1->getTransform()->translate(Vec(0.5,2.1));
	sprite1->addComponent<Physics::PhysicsComponent>(1.0, Vec(1,1),
										   sprite1->getTransform());

//	sprite1->getComponent<Physics::PhysicsComponent>()->addForce(Vec(0,-10.0));
	sprite1->getComponent<Physics::PhysicsComponent>()->addTorque(5.0);
/*	sprite1->getComponent<PhysicsComponent>()
		  ->addForceGenerator(std::make_shared<GravityGenerator>(2));
	sprite1->getComponent<PhysicsComponent>()
		  ->addForceGenerator(std::make_shared<SpringGenerator>(2, 0.1, 5,
					  									   Vec2(0.0, 0.0)));
*/
	sprite2 = newEntity();
	sprite2->addComponent<RenderingComponent>(
						Model(Context::NewMesh("sprite"), 
							  context.newTexture("bricks2.png")));

//	sprite2->getTransform()->translate(Vec(0.1,0.5));
	sprite2->addComponent<Physics::PhysicsComponent>(1.0, Vec(1,1),
										   sprite2->getTransform());
//	sprite2->getComponent<PhysicsComponent>()->addForce(Vec(0,-10.0));
	sprite2->getComponent<Physics::PhysicsComponent>()->addTorque(-5.0);
/*	sprite2->getComponent<PhysicsComponent>()
		  ->addForceGenerator(std::make_shared<GravityGenerator>(2));
*/

//	mover = std::make_shared<Mover>(elysium.getCamera().getTransform());

	//m_input = std::make_shared<Input>();
/*
	getInput()->getKeyListener(Input::KEY_W)
						.addListener(mover->getForwardListener());
	getInput()->getKeyListener(Input::KEY_A)
						.addListener(mover->getLeftListener());
	getInput()->getKeyListener(Input::KEY_S)
						.addListener(mover->getBackwardListener());
	getInput()->getKeyListener(Input::KEY_D)
						.addListener(mover->getRightListener());
	getInput()->getMouseMotion()
						.addListener(mover->getRotateListener());
*/
	std::shared_ptr<Physics::PhysicsEngine> phi = std::make_shared<Physics::PhysicsEngine>();
	
	std::shared_ptr<Physics::Rod> joint = std::make_shared<Physics::Rod>(
					  sprite1->getComponent<Physics::PhysicsComponent>(),
					  sprite2->getComponent<Physics::PhysicsComponent>());

//	phi->addConstraint(joint);

	addSystem(phi);

	s1 = Physics::Shape(std::vector<Vec>({Vec(-1,-1),Vec(-1,1),Vec(1,1),Vec(1,-1)}), sprite1->getTransform());
	
	s2 = Physics::Shape(std::vector<Vec>({Vec(-1,-1),Vec(-1,1),Vec(1,1),Vec(1,-1)}), sprite2->getTransform());

//sprite1->getTransform()->rotate(10.0*3.14/180.0);
}

void TestGame::update(const Real dt, std::vector<Entity>& entities,
					  Context& context)
{
//	simplex = Physics::Simplex();
	Physics::CollisionDetector cd;
	cd.nearCollision(simplex, s1, s2);
}
