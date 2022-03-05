#include "TestGame.h"

void TestGame::init(Elysium::RenderingEngine& elysium)
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
	
	elysium.addPass<LightPass<DirectionalLight>>
					(newShader("directionalLight"), 
					 newMesh("plane3.obj"));

	elysium.addPass<LightPass<PointLight>>
					(newShader("pointLight"), 
					 newMesh("plane3.obj"));

	elysium.addPass<AmbientPass>(newShader("screen"),
								 newMesh("plane3.obj"), 0.0f);

	elysium.getCamera().getTransform()->translate(Circe::Vec3(0,0,-5));

	sprite = newSprite("bricks2.png");
	elysium.get<GeometryPass>()->addModel(sprite);

	cyborg = newModel("cyborg/cyborg.obj");
	cyborg.getTransform()->translate(Circe::Vec3(0,-2,0));
	elysium.get<GeometryPass>()->addModel(cyborg);

	std::shared_ptr<DirectionalLight> dlight = 
		std::make_shared<DirectionalLight>(Circe::Vec3(0.5,0,-1),
										   Circe::Vec3(0.0,1.0,0.0),
										   1.0f);

	elysium.get<LightPass<DirectionalLight>>()->addLight(dlight);

	std::shared_ptr<PointLight> plight = 
		std::make_shared<PointLight>(Circe::Vec3(0.2,0,-1),
									 Circe::Vec3(1.0,0.0,0.0),
									 Circe::Vec3(1,0,1));

//	elysium.get<LightPass<PointLight>>()->addLight(plight);


	mover = std::make_shared<Mover>(elysium.getCamera().getTransform());

	m_input = std::make_shared<Input>();

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
}

void TestGame::update()
{
		cyborg.getTransform()->rotate(Circe::Vec3(0,1,0),0.003f);
}
