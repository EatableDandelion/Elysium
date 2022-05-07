#include "Game/Game.h"

DebugFunctions Debug;

/** Game loop from https://gameprogrammingpatterns.com/game-loop.html */
namespace Elysium
{
	//ResourceManager<Mesh, GeometryLoader> Context::m_geometries;

	Context::Context(std::shared_ptr<Input> input):m_input(input)
	{
//		m_input = std::make_shared<Input>();
	}

	Entity Context::getEntity(const EntityID id) const
	{
		return m_entities.at(id);
	}

	Entity Context::newEntity()
	{
		Entity entity = 
				std::make_shared<EntityData>(shared_from_this());

		m_entities.insert(
				std::pair<EntityID, Entity>(entity->getID(), entity));

		return entity;
	}

	void Context::removeEntity(const EntityID id)
	{
		m_entities.erase(id);
	}

	std::map<EntityID, Entity> Context::getEntities() const
	{
		return m_entities;
	}

	void Context::onComponentAdded(const EntityID entityID,
							   	   const ComponentID c)
	{
		Entity entity = getEntity(entityID);
		for(std::shared_ptr<System> system : m_systems)
		{
			system->onComponentAdded(getEntity(entityID), c);
		}
	}

	void Context::onComponentRemoved(const EntityID entityID,
							   	   	 const ComponentID c)
	{
		Entity entity = getEntity(entityID);
		for(std::shared_ptr<System> system : m_systems)
		{
			system->onComponentRemoved(getEntity(entityID), c);
		}
	}

	/*void Context::update(const Real dt)
	{
		for(auto pair : m_entities)
		{
			pair.second->update(pair.second, m_renderer, dt);
		}
	}*/

	Shader Context::newShader(const std::string& name)
	{
		return m_shaders.getResource(name); 
	}
	
	Mesh Context::newMesh(const std::string& name)
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
	std::shared_ptr<Renderer> Context::getRenderer() const
	{
		return m_renderer;
	}

	void Context::setRenderer(const std::shared_ptr<Renderer> renderer)
	{
		m_renderer = renderer;
	}

	void Context::addSystem(const std::shared_ptr<System> system)
	{
		m_systems.push_back(system);
	}

	std::vector<std::shared_ptr<System>> Context::getSystems() const
	{
		return m_systems;
	}


	Game::Game() 
		: m_context(std::make_shared<Context>(std::make_shared<Input>()))
	{}

	void Game::init()
	{
		init(m_context);
	}

	void Game::updateAll(const Real dt)
	{
		update(dt, m_context);

		for(auto pair : m_context->getEntities())
		{
			pair.second->update(pair.second, m_context->getRenderer(), dt);
		}
//		m_world.update(dt, m_context->getRenderer());

		for(std::shared_ptr<System> system : m_context->getSystems())
		{
			system->update(dt, m_context);
		}
	}

	void Game::draw()
	{
		m_context->getRenderer()->draw();
	}

	bool Game::isTerminated()
	{
		m_context->getInput()->poll();
		return m_context->getInput()->isTerminated();
	}
}
