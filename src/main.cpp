#include "Game/Game.h"
#include "TestGame.h"

int main()
{
	std::shared_ptr<TestGame> game = std::make_shared<TestGame>();

	Elysium::GameLoop loop(game);

	loop.start();
}

