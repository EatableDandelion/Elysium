#include "Game/Game.h"

/** Game loop from https://gameprogrammingpatterns.com/game-loop.html */
namespace Elysium
{
	Game::Game(const std::string& name, const int width, const int height)
				:m_renderingEngine(name, width, height, 3)
	{}

	void Game::init()
	{
		init(m_renderingEngine);
	}

	void Game::update(const Real dt)
	{
		update(dt, entities);
		for(std::shared_ptr<System> system : systems)
		{
			system->update(dt, entities);
		}
	}

	void Game::draw()
	{
		m_renderingEngine.draw(entities);
	}

	std::shared_ptr<Input> Game::getInput()
	{
		return m_input;
	}

	void Game::pollInput()
	{
		m_input->poll();
	}

	bool Game::isTerminated() const
	{
		return m_input->isTerminated();
	}

	Shader Game::newShader(const std::string& name)
	{
		return m_shaders.getResource(name); 
	}

	Mesh Game::newMesh(const std::string& name)
	{
		return m_meshes.getResource(name); 
	}
	
	Mesh Game::newGeometry(const std::string& name)
	{
		return m_geometries.getResource(name);
	}

	Texture Game::newTexture(const std::string& name)
	{
		return m_textures.getResource(name); 
	}

	Model Game::newModel(const std::string& name)
	{
		return m_models.getResource(name); 
	}
			
	Entity Game::newSprite(const std::string& name)
	{		
		Entity sprite = newEntity();
			
		sprite->addComponent<RenderingComponent>(
						Model(newMesh("plane3.obj"), newTexture(name)));

		return sprite;
	}

	Entity Game::newEntity()
	{
		Entity entity = std::make_shared<EntityData>();
		entities.push_back(entity);
		return entity;
	}
	
	void Game::addSystem(const std::shared_ptr<System> system)
	{
		systems.push_back(system);
	}

	namespace Time
	{
		typedef std::chrono::steady_clock clock;
		typedef std::chrono::duration<Real> duration;
		typedef clock::time_point time_point;
	}

	GameLoop::GameLoop(const std::shared_ptr<Game> game)
						:m_running(false),
						 m_game(game)
	{
	}

	void GameLoop::start()
	{
		if(!m_running)
			run();
	}

	void GameLoop::run()
	{
		m_game->init();

		double SEC_PER_FRAME = 1.0/60.0;
		m_running = true;
		Time::time_point current, previous, sysCurrent, sysPrevious;
		double lag = 0.0;
		previous = Time::clock::now();

		while(!m_game->isTerminated())
		{	

			current = Time::clock::now();

			double elapsed = 
					Time::duration(current - previous).count();
			
			previous = current;

			lag += elapsed;

			std::cout << "\r" << "Execution time: " << elapsed 
					  << " s           " 			<< std::flush;
		
			m_game->pollInput();
				
			while(lag >= SEC_PER_FRAME)
			{
				sysCurrent = Time::clock::now();
				elapsed = Time::duration(sysCurrent - sysPrevious).count();
				sysPrevious = sysCurrent;

				m_game->update(elapsed);

				lag -= SEC_PER_FRAME;
			}

			m_game->draw();	
		}		
		m_running = false;
	}

}
