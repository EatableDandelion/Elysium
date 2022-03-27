#include "TestGame.h"


TestGame::TestGame(const std::string& name, const int width, 
				   const int height):Game(name, width, height)
{}

void TestGame::init(RenderingEngine& elysium)
{
	using namespace Elysium;

	
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

	m_shaders.setFolderLocation("../res/shaders/");
	m_meshes.setFolderLocation("../res/meshes/");
	m_textures.setFolderLocation("../res/textures/");
	m_models.setFolderLocation("../res/models/");


	elysium.addPass<GeometryPass>(newShader("geometry"));
	
	elysium.addPass<DirectionalLightPass>(newShader("directionalLight"), 
					 					  newMesh("plane3.obj"));
//	elysium.addPass<PointLightPass>(newShader("pointLight"), 
//					 				newMesh("plane3.obj"));

	elysium.addPass<AmbientPass>(newShader("screen"),
								 newMesh("plane3.obj"), 0.0f);

	elysium.addPass<DebugPass>(newShader("debug"));


	Entity directionalLight = newEntity();
	directionalLight->addComponent<DirectionalLight>(Vec3(0.5,0,-1),
										   			 Vec3(1.0,1.0,1.0),
										   			 1.0f);

	Entity cyborg = newEntity();
	cyborg->addComponent<RenderingComponent>(newModel("cyborg/cyborg.obj"));

	elysium.getCamera().getTransform()->translate(Vec3(0,0,-20));


	sprite1 = newSprite("bricks2.png");
	sprite1->getTransform()->translate(Vec(4,0));
	sprite1->addComponent<PhysicsComponent>(1.0, Vec(1,1),
										   sprite1->getTransform());

	sprite1->getComponent<PhysicsComponent>()->addForce(Vec(-50,50.0));
//	sprite1->getComponent<PhysicsComponent>()->addTorque(10.0);
/*	sprite1->getComponent<PhysicsComponent>()
		  ->addForceGenerator(std::make_shared<GravityGenerator>(2));
	sprite1->getComponent<PhysicsComponent>()
		  ->addForceGenerator(std::make_shared<SpringGenerator>(2, 0.1, 1,
					  									   Vec2(0.0, 0.0)));
*/
	sprite2 = newSprite("bricks2.png");
	sprite2->getTransform()->translate(Vec(1,1));
	sprite2->addComponent<PhysicsComponent>(1.0, Vec(1,1),
										   sprite2->getTransform());
//	sprite2->getComponent<PhysicsComponent>()->addTorque(2.0);
//	sprite2->getComponent<PhysicsComponent>()
//		  ->addForceGenerator(std::make_shared<GravityGenerator>(2.1));


	mover = std::make_shared<Mover>(elysium.getCamera().getTransform());

	m_input = std::make_shared<Input>();
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
	std::shared_ptr<PhysicsEngine> phi = std::make_shared<PhysicsEngine>();
	
	std::shared_ptr<Slider> joint = std::make_shared<Slider>(
					  sprite1->getComponent<PhysicsComponent>(),
					  sprite2->getComponent<PhysicsComponent>());

//	phi->addConstraint(joint);

	addSystem(phi);

	sprite3 = newSprite("bricks2.png");
}

void TestGame::update(const Real dt, std::vector<Entity>& entities)
{
	sprite3->getTransform()->lineUp(sprite1->getTransform()->getPosition(),
									sprite2->getTransform()->getPosition(),
									0.5);

	
}
