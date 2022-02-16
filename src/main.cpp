#include "Game.h"
#include "TestGame.h"

int main()
{
	std::shared_ptr<TestGame> game = std::make_shared<TestGame>();

	Elysium::GameLoop loop("Elysium", 800, 600, game);

	loop.start();


/*	float counter = 0.0f;
	while(!elysium.isClosed())
	{
//		cyborg.getTransform()->rotate(Circe::Vec3(0,1,0),0.003f);
	//	light->m_direction = Circe::normalize(Circe::Vec3(std::cos(counter),0.0,std::sin(counter)));
		elysium.draw();	

		counter += 0.03f;	
	}*/
}

