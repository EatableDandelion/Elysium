#pragma once

namespace Carina
{
	class Thruster : public Component
	{

	};

	class ThrusterSystem : public System
	{
		public:
			virtual void onComponentAdded(Entity entity, 
										  const ComponentID id);

			virtual void onComponentRemoved(Entity entity, 
										  	const ComponentID id);
	

		private:
			std::set<EntityID> m_entities;		
	};
}
