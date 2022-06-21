#include "Carina/ShipComponent.h"

namespace Carina
{
	Thruster::Thruster(const Vec2& position, const Vec2& direction,
				 	   const Real maxThrust, const bool rcs)
		: m_position(position), m_thrust(direction), m_rcs(rcs)
	{
		normalize(m_thrust);
		m_thrust = m_thrust * maxThrust;
	}


	void ThrusterComponent::addThruster(const Thruster& thruster)
	{
		if(thruster.m_rcs)
			m_RCSThrusters.push_back(thruster);
		else
			m_mainThrusters.push_back(thruster);

		assignWeights();
	}

	void ThrusterComponent::useRCS(const Mat& command)
	{
		Mat intensities = command/m_RCSMat;
		for(int i = 0; i<m_RCSThrusters.size(); i++)
		{
			m_RCSThrusters[i].m_intensity = intensities(i);
		}
	}

	void ThrusterComponent::assignWeights()
	{
		int nbRCS = m_RCSThrusters.size();

		if(nbRCS < 3) return;

		m_RCSMat = Mat(3, nbRCS);

		int j = 0;
		for(Thruster rcs : m_RCSThrusters)
		{
			Real l = norm(rcs.m_thrust);
			m_RCSMat(0,j) = rcs.m_thrust(0)/l;
			m_RCSMat(1,j) = rcs.m_thrust(1)/l;
			m_RCSMat(2,j) = cross(rcs.m_thrust/l, rcs.m_position);
			j++;
		}
	
		/*Mat weights = b/A;

		for(int i = 0; i<3; i++)
		{
			for(int j =0; j<nbRCS; j++)
			{
				m_RCSThrusters[j].m_weights(i) = weights(j);
			}
		}*/
	}


	ThrusterSystem::ThrusterSystem()
	{
		System::addComponentType<ThrusterComponent>();
		System::addComponentType<Physics::PhysicsComponent>();
	}

	void ThrusterSystem::update(std::shared_ptr<GameInterface> game)
	{
		if(game->getInput()->isActive("main thruster"))
		{
			for(EntityID id : m_entities)
			{
				Entity entity = game->getEntity(id);

				std::shared_ptr<Physics::PhysicsComponent> physics 
					= entity->getComponent<Physics::PhysicsComponent>();

				std::shared_ptr<ThrusterComponent> thrusterComponent 
					= entity->getComponent<ThrusterComponent>();

				for(Thruster thruster : thrusterComponent->m_mainThrusters)
				{
					physics->addLocalForce(thruster.m_thrust,
									  	   thruster.m_position);
				}
			}
		}

		bool displacement = false;
		Mat command(3,1);

		if(game->getInput()->isActive("move left"))
		{
			command(0) += -1.0;
			displacement = true;
		}
		if(game->getInput()->isActive("move right"))
		{
			command(0) += 1.0;
			displacement = true;
		}
		if(game->getInput()->isActive("move backward"))
		{
			command(1) += -1.0;
			displacement = true;
		}
		if(game->getInput()->isActive("move forward"))
		{
			command(1) += 1.0;
			displacement = true;
		}
		if(game->getInput()->isActive("steer left"))
		{
			command(2) += -1.0;
			displacement = true;
		}
		if(game->getInput()->isActive("steer right"))
		{
			command(2) += 1.0;
			displacement = true;
		}

		if(displacement)
		{
			for(EntityID id : m_entities)
			{
				Entity entity = game->getEntity(id);

				std::shared_ptr<Physics::PhysicsComponent> physics 
					= entity->getComponent<Physics::PhysicsComponent>();

				std::shared_ptr<ThrusterComponent> thrusterComponent 
					= entity->getComponent<ThrusterComponent>();

				thrusterComponent->useRCS(command);

				for(Thruster thruster : thrusterComponent->m_RCSThrusters)
				{
					physics->addLocalForce(thruster.m_thrust
									 	  *thruster.m_intensity,
									  	   thruster.m_position);
				}
			}
		}
	}
}
