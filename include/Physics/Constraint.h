#pragma once
#include "Circe/Circe.h"
#include "Physics/PhysicsComponent.h"

namespace Elysium
{
	class Joint
	{
		public:
			Joint(const std::shared_ptr<PhysicsComponent> component1,
			 const std::shared_ptr<PhysicsComponent> component2 = nullptr);
			
			virtual Mat getJ() = 0;

			virtual Mat getC() = 0;

			Mat getV();

			Mat getK(const Mat& J);

			void update(const Real& dt, const Mat& J, const Mat& lambda);

			void setAnchor1(const Vec& worldCoordinate);

			void setAnchor2(const Vec& worldCoordinate);

		protected:
			Vec getR1() const;

			Vec getR2() const;


			std::shared_ptr<Joint> joint;
			std::shared_ptr<PhysicsComponent> component1;
			std::shared_ptr<PhysicsComponent> component2;

//		private:	
			Vec anchor1;
			Vec anchor2;
			Real angle0;
	};

	class ConstraintSolver
	{
		public:
			void applyImpulse(const Real dt, std::shared_ptr<Joint> joint);
	};

	class Rod : public Joint
	{
		public:
			Rod(const std::shared_ptr<PhysicsComponent> component1,
			 	const std::shared_ptr<PhysicsComponent> component2);
			
			virtual Mat getJ();

			virtual Mat getC();

	};
	
	class Slider : public Joint
	{
		public:
			Slider(const std::shared_ptr<PhysicsComponent> component1,
			 	   const std::shared_ptr<PhysicsComponent> component2);
			
			virtual Mat getJ();

			virtual Mat getC();

		private:
			Vec n;
	};
}
