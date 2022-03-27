#include "Game/Game.h"
#include "TestGame.h"

int main()
{
	std::shared_ptr<TestGame> game = 
						std::make_shared<TestGame>("Elysium", 800, 600);

	Elysium::GameLoop loop(game);

	loop.start();
}

