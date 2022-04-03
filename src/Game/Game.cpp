#include "Game/Game.h"

/** Game loop from https://gameprogrammingpatterns.com/game-loop.html */
namespace Elysium
{
	ResourceManager<Mesh, GeometryLoader> Context::m_geometries;

	Context::Context()
	{
		m_input = std::make_shared<Input>();
	}

	Shader Context::newShader(const std::string& name)
	{
		return m_shaders.getResource(name); 
	}
	
	Mesh Context::NewMesh(const std::string& name)
	{
		return m_geometries.getResource(name);
	}

	Texture Context::newTexture(const std::string& name)
	{
		return m_textures.getResource(name); 
	}

	Model Context::newModel(const std::string& name)
	{
		return m_models.getResource(name); 
	}
		
	std::shared_ptr<Input> Context::getInput()
	{
		return m_input;
	}	


	void Context::setShadersDirectory(const std::string& name)
	{
		m_shaders.setFolderLocation(name);
	}

	void Context::setModelsDirectory(const std::string& name)
	{
		m_models.setFolderLocation(name);
	}
	
	void Context::setTexturesDirectory(const std::string& name)
	{
		m_textures.setFolderLocation(name);
	}
	
	std::shared_ptr<RenderingEngine> Game::m_rendering;

	void Game::update(const Real dt, Context& context)
	{
		for(Entity entity : m_entities)
			entity->update(entity, dt);

		update(dt, m_entities, context);
		for(std::shared_ptr<System> system : m_systems)
		{
			system->update(dt, m_entities, context);
		}
	}

	/*	
	Entity Game::newSprite(const std::string& name)
	{		
		Entity sprite = newEntity();
			
		sprite->addComponent<RenderingComponent>(
						Model(Game::NewMesh("plane3.obj"), 
							  Game::NewTexture(name)));

		return sprite;
	}*/

	Entity Game::newEntity()
	{
		Entity entity = std::make_shared<EntityData>();
		m_entities.push_back(entity);
		return entity;
	}

	void Game::SetRenderer(const std::shared_ptr<RenderingEngine> renderer)
	{
		m_rendering = renderer;
	}

	std::shared_ptr<RenderingEngine> Game::Renderer()
	{
		return m_rendering;
	}

	void Game::addSystem(const std::shared_ptr<System> system)
	{
		m_systems.push_back(system);
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
	{}

	void GameLoop::start()
	{
		if(!m_running)
			run();
	}

	void GameLoop::run()
	{
		m_game->init(m_context);

		double SEC_PER_FRAME = 1.0/60.0;
		m_running = true;
		Time::time_point current, previous, sysCurrent, sysPrevious;
		double lag = 0.0;
		previous = Time::clock::now();
		while(!m_context.getInput()->isTerminated())
		{	

			current = Time::clock::now();

			double elapsed = 
					Time::duration(current - previous).count();
			
			previous = current;

			lag += elapsed;

			std::cout << "\r" << "Execution time: " << elapsed 
					  << " s           " 			<< std::flush;
		
			m_context.getInput()->poll();

			while(lag >= SEC_PER_FRAME)
			{
				sysCurrent = Time::clock::now();
				elapsed = Time::duration(sysCurrent - sysPrevious).count();
				sysPrevious = sysCurrent;

				m_game->update(elapsed, m_context);

				lag -= SEC_PER_FRAME;
			}

			Game::Renderer()->draw();	
		}		
		m_running = false;
	}

}
