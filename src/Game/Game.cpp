#include "Game/Game.h"

DebugFunctions Debug;

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

	void Game::updateAll(const Real dt, World& world, Context& context)
	{
//		for(Entity entity : m_entities)
//			entity->update(entity, m_renderer, dt);

		update(dt, world, context);

		world.update(m_renderer, dt);

		for(std::shared_ptr<System> system : m_systems)
		{
			system->update(dt, world, context);
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

/*	Entity Game::newEntity()
	{
		Entity entity = std::make_shared<EntityData>();
		m_entities.push_back(entity);
		return entity;
	}*/

	std::shared_ptr<Renderer> Game::getRenderer() const
	{
		return m_renderer;
	}

	void Game::setRenderer(const std::shared_ptr<Renderer> renderer)
	{
		m_renderer = renderer;
	}


	void Game::addSystem(const std::shared_ptr<System> system)
	{
		m_systems.push_back(system);
	}
}
