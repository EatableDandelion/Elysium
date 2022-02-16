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

	shaders.setFolderLocation("../res/shaders/");
	meshes.setFolderLocation("../res/meshes/");
	textures.setFolderLocation("../res/textures/");
	models.setFolderLocation("../res/models/");

	elysium.getCamera().getTransform()->translate(Circe::Vec3(0,0,-5));

	elysium.addPass<GeometryPass>(shaders.getResource("geometry"));
	
	elysium.addPass<LightPass<DirectionalLight>>
					(shaders.getResource("directionalLight"), 
					 meshes.getResource("plane3.obj"));

	elysium.addPass<LightPass<PointLight>>
					(shaders.getResource("pointLight"), 
					 meshes.getResource("plane3.obj"));

	elysium.addPass<AmbientPass>(shaders.getResource("screen"),
								 meshes.getResource("plane3.obj"), 0.0f);

	cyborg = models.getResource("cyborg/cyborg.obj");
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

	elysium.get<LightPass<PointLight>>()->addLight(plight);


	//mover = std::make_shared<Mover>(elysium.getCamera().getTransform());
	mover = std::make_shared<Mover>(cyborg.getTransform());

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
