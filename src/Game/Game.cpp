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

	std::vector<unsigned int> Context::query
			(const std::shared_ptr<Circe::PrimitiveVolume
			 						<Constants::BroadCollider>> volume)
	{
		return m_physics->query(volume);
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

	std::shared_ptr<Input> Context::getInput()
	{
		return m_input;
	}
	
	std::shared_ptr<Renderer> Context::getRenderer() const
	{
		return m_renderer;
	}

	Vec3 Context::getCameraPosition() const
	{
		return m_renderer->getCamera().getTransform()->getPosition();
	}

	void Context::setCameraPosition(const Vec3 position)
	{
		m_renderer->getCamera().getTransform()->setPosition(position);
	}

	Real Context::getTimeStep() const
	{
		return m_dt;
	}

	Real Context::getTotalTime() const
	{
		return m_totalTime;
	}

	void Context::setTime(const Real dt, const Real totalTime)
	{
		m_dt = dt;
		m_totalTime = totalTime;
	}

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

	void Context::setRenderer(const std::shared_ptr<Renderer> renderer)
	{
		m_renderer = renderer;
	}

	void Context::setPhysicsEngine
				(const std::shared_ptr<Physics::IPhysicsEngine> physics)
	{
		m_physics = physics;
		addSystem(physics);
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

	void Game::updateAll(const Real dt, const Real totalTime)
	{
		m_context->setTime(dt, totalTime);
		update(m_context);

		for(auto pair : m_context->getEntities())
		{
			pair.second->update(pair.second, m_context);
		}

		for(std::shared_ptr<System> system : m_context->getSystems())
		{
			system->update(m_context);
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
