#include <iostream>
#include <Circe/Circe.h>
#include "Display.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Assets.h"

int main()
{
	using namespace Elysium;
	Display display(800, 600, "Elysium");

	ResourceManager<Shader, ShaderLoader> shaders("../res/shaders/");
	Shader shader = shaders.getResource("basicShader");

//	ResourceManager<Mesh, MeshLoader> meshes("../res/meshes/");
//	Mesh mesh1 = meshes.getResource("monkey3.obj");
//	Mesh mesh2 = meshes.getResource("plane3.obj");

	ResourceManager<Texture, TextureLoader> textures("../res/textures/");
	Texture texture = textures.getResource("bricks2.png");

	ResourceManager<Model, ModelLoader> models("../res/models/");
	Model model = models.getResource("cyborg/cyborg.obj");

	Camera camera(Circe::Vec3(0,0,-3), 70.0f, 
				  (float)display.getWidth()/(float)display.getHeight(),
				  0.01f, 1000.0f);

	Circe::Transform3 transform1, transform2;

	transform1.setScale(Circe::Vec3(0.2,0.2,0.2));
	transform2.setScale(Circe::Vec3(5.0,5.0,5.0));
	transform2.getPosition()(1) = -5.0f;

	float counter = 0.0f;

	while(!display.isClosed())
	{
		display.clear(0.0f,0.0f,0.0f,1.0f);

		transform1.rotate(Circe::Vec3(0,1,0),0.01f); 
		//transform1.translate(Circe::Vec3(0,0,-0.01));

		shader.bind();
		texture.bind(0);
		shader.updateUniforms(transform1, camera);
		model.draw();
		shader.updateUniforms(transform2, camera);
		model.draw();
		display.update();
		counter += 0.01f;
	}
}
