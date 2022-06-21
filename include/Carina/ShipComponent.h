#pragma once
#include "Game/Game.h"
#include "Physics/Physics.h"

namespace Carina
{
	struct Thruster
	{
		Thruster(const Vec2& position, const Vec2& direction,
				 const Real maxThrust, const bool rcs); 

		bool m_rcs;
		Vec2 m_position;
		Real m_intensity;	
		Vec2 m_thrust;	
		Vec3 m_weights; // x, y, momentum
	};

	class ThrusterComponent : public Component
	{
		public:
			void addThruster(const Thruster& thruster);

			void useRCS(const Mat& command);

		private:
			std::vector<Thruster> m_RCSThrusters;
			std::vector<Thruster> m_mainThrusters;
			Mat m_RCSMat;

			void assignWeights();

			friend class ThrusterSystem;
	};

	class ThrusterSystem : public System
	{
		public:
			ThrusterSystem();
				
			virtual void update(std::shared_ptr<GameInterface> game);
	};
}
