#include "TestGame.h"

void TestGame::init(std::shared_ptr<Context> context)
{
	using namespace Elysium;

	context->setRenderer(std::make_shared<Renderer>("Elysium",800,600,3));

	context->setShadersDirectory("../res/shaders/");
	context->setTexturesDirectory("../res/textures/");
	context->setModelsDirectory("../res/models/");

	std::shared_ptr<Physics::PhysicsEngine> phi = 
							std::make_shared<Physics::PhysicsEngine>();
	
	context->addSystem(phi);



	context->getRenderer()->setFirstPass<GeometryPass>
						   (context->newShader("geometry"));

	context->getRenderer()->addPass<DirectionalLightPass>(
							context->newShader("directionalLight"), 
					 		context->newMesh("sprite"));

	context->getRenderer()
			->addPass<AmbientPass>(context->newShader("screen"),
								   context->newMesh("sprite"), 0.0f);

	context->getRenderer()->addPass<DebugPass>(context->newShader("debug"));

	Debug.setDebugRenderer(context->getRenderer()->getPass<DebugPass>());

	context->getRenderer()
		   ->getPass<DirectionalLightPass>()
		   ->addLight(DirectionalLight(Vec3(0.5,0,-1), 
									   Vec3(1.0,1.0,1.0), 1.0f));

	context->getRenderer()->getCamera().getTransform()
						  ->translate(Vec3(0,0,-10));

	sprite1 = context->newEntity();
	sprite1->addComponent<RenderingComponent>(
						Model(context->newMesh("sprite"), 
							  context->newTexture("bricks2.png")));

//	sprite1->getTransform()->translate(Vec(1.5,7.7));

	std::vector<Vec> collisionPoints({Vec(-1,-1),Vec(-1,1),
									  Vec(1,1),Vec(1,-1)});
	
	sprite1->addComponent<Physics::PhysicsComponent>(10.0, Vec(1,1),
						   sprite1->getTransform(), collisionPoints);

	context->getInput()->addActionToKey(Input::KEY_SPACE, "main thruster");
}

void TestGame::update(const Real dt, std::shared_ptr<Context> context)
{
//	if(context->getInput()->isActive("main thruster"))
//	   std::cout << "active" <<std::endl;
}

