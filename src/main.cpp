#include <iostream>
#include <Circe/Circe.h>
#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Assets.h"
#include "RenderingEngine.h"

int main()
{
	using namespace Elysium;

	RenderingEngine elysium("Elysium", 800, 600, 3);

	elysium.getCamera().getTransform().translate(Circe::Vec3(0,0,-3));

	ResourceManager<Shader, ShaderLoader> shaders("../res/shaders/");
	ResourceManager<Mesh, MeshLoader> meshes("../res/meshes/");
	ResourceManager<Texture, TextureLoader> textures("../res/textures/");
	ResourceManager<Model, ModelLoader> models("../res/models/");

	PassID geoID = 
	   elysium.addPass<GeometryPass>(shaders.getResource("geometry"));

	PassID finalID =
	   elysium.addPass<FinalPass>(shaders.getResource("screen"));

	Model cyborg = models.getResource("cyborg/cyborg.obj");
	cyborg.getTransform()->translate(Circe::Vec3(0,-2,0));

	elysium(geoID)->addModel(cyborg);

	while(!elysium.isClosed())
	{
		cyborg.getTransform()->rotate(Circe::Vec3(0,1,0),0.01f);
		elysium.draw();		
	}
}
