#include "Physics/Constraint.h"

namespace Physics
{
	Joint::Joint(const std::shared_ptr<PhysicsComponent> component1,
				 const std::shared_ptr<PhysicsComponent> component2):
				 component1(component1),
				 component2(component2)
	{
		
	}


	void Joint::restrictLambda(Mat& lambda)
	{}

	Mat Joint::getV()
	{
		Vec2 v1 = component1->getVelocity();	
		Vec2 v2 = component2->getVelocity();	
		Real o1 = component1->getAngularVelocity();
		Real o2 = component2->getAngularVelocity();
		return Mat(6,1,{v1(0),v1(1),o1,v2(0),v2(1),o2});
	}
	
	Mat Joint::getK(const Mat& J)
	{
		Real M_inv1 = component1->getMassInv();
		Real M_inv2 = component2->getMassInv();
		Real I_inv1 = component1->getInertiaInv();
		Real I_inv2 = component2->getInertiaInv();

		Mat J0 = J;
		for(int i = 0; i<J.getNbRows(); i++)
		{
			J0(i,0) = J(i,0)*M_inv1;
			J0(i,1) = J(i,1)*M_inv1;
			J0(i,2) = J(i,2)*I_inv1;
			J0(i,3) = J(i,3)*M_inv2;
			J0(i,4) = J(i,4)*M_inv2;
			J0(i,5) = J(i,5)*I_inv2;
		}
		return J0.multTranspose(J);
	}

	void Joint::setAnchor1(const Vec& worldCoordinate)
	{
		anchor1=component1->getTransform()->toLocal(worldCoordinate, true);
	}

	void Joint::setAnchor2(const Vec& worldCoordinate)
	{
		anchor2=component2->getTransform()->toLocal(worldCoordinate, true);
	}

	Vec Joint::getR1() const
	{
		return component1->getTransform()
						 ->toGlobal(anchor1, false);
	}
	
	Vec Joint::getR2() const
	{
		return component2->getTransform()
						 ->toGlobal(anchor2, false);
	}

	void Joint::update(const Real& dt, const Mat& J, const Mat& lambda)
	{
		Mat F = J.transposeMult(lambda/dt);

		Vec r1 = getR1();
		Real moment = r1(0)*F(1)-r1(1)*F(0);
		component1->update(dt, Vec3(-F(0),-F(1),-F(2)-dt*moment));
		
		Vec r2 = getR2();
		moment = r2(0)*F(4)-r2(1)*F(3);	
		component2->update(dt, Vec3(-F(3),-F(4),-F(5)-dt*moment));
	}

	void ConstraintSolver::applyImpulse(const Real dt, 
										std::shared_ptr<Joint> joint)
	{
		Mat J = joint->getJ();
		Mat v = joint->getV();
		Mat K = joint->getK(J);
		Mat b = joint->getC()/dt;
	/*	Mat motor(b.getNbRows(), 1);
		motor(2) = 1.0*cos(0.05*t);
		b = b + motor;
	*/	t += dt;
		Mat lambda = (J*v+b)/K; 

		if(firstStep)
		{
			firstStep = false;
//			lambda = lambdaNew;
		}

		Real w = 0.9;
//		lambda = lambdaNew*w + lambda*(1.0-w);

		joint->restrictLambda(lambda);

		joint->update(dt, J, lambda);
	}


	Rod::Rod(const std::shared_ptr<PhysicsComponent> component1,
			 const std::shared_ptr<PhysicsComponent> component2)
			:Joint(component1,component2)
	{
		setAnchor1(component1->getPosition());
		setAnchor2(component1->getPosition());
		
		angle0 = component2->getTransform()->
				 angleWith(*(component1->getTransform()));
	}
			

	Mat Rod::getJ()
	{
		Vec r1 = getR1();
		Vec r2 = getR2();

		Mat J(3, 6);

		J(0,0) = -1.0;
		J(1,1) = -1.0;
		J(0,2) = r1(1);
		J(1,2) = -r1(0);

		J(0,3) = 1.0;
		J(1,4) = 1.0;
		J(0,5) = -r2(1);
		J(1,5) = r2(0);

		J(2,2) = 1.0;
		J(2,5) = -1.0;

		return J;
	}

	Mat Rod::getC()
	{
		Vec r1 = getR1();
		Vec r2 = getR2();

		Vec C = component2->getPosition() + r2 
			  - component1->getPosition() - r1;

		double angle = component2->getTransform()->
					angleWith(*(component1->getTransform()));

		return Mat(3,1, {C(0),C(1), angle-angle0})*0.3;
	}

	Slider::Slider(const std::shared_ptr<PhysicsComponent> component1,
			 const std::shared_ptr<PhysicsComponent> component2)
			:Joint(component1,component2)
	{
		setAnchor1(component2->getPosition());
		setAnchor2(component1->getPosition());
		Vec axis = component2->getPosition()-component1->getPosition();
		axis = normalize(axis);
		axis = component1->getTransform()->toLocal(axis,false);
		n(0) = axis(1);
		n(1) =-axis(0);
		a = axis;
	}

	Mat Slider::getJ()
	{
		Vec r1 = getR1();
		Vec r2 = getR2();

		Vec u = Circe::normalize(component2->getPosition() 
							   - component1->getPosition());

		Vec2 n1 = component1->getTransform()->toGlobal(n,false);
		Vec2 a1 = component1->getTransform()->toGlobal(a,false);

		Mat J(2, 6);
		J(0,0) = -n1(0);
		J(0,1) = -n1(1);
		J(0,2) = -Circe::cross(r1+u,n1);
		J(1,2) = -1.0;

		J(0,3) = n1(0);
		J(0,4) = n1(1);
		J(0,5) = Circe::cross(r2, n1);
		J(1,5) = 1.0;

	//	J(2,0) = a1(0);
	//	J(2,1) = a1(1);
	//	J(2,3) =-a1(0);
	//	J(2,4) =-a1(1);

		return J;
	}

	Mat Slider::getC()
	{
		Vec r1 = getR1();
		Vec r2 = getR2();

		Vec C = component2->getPosition() + r2 
			  - component1->getPosition() - r1;

		Vec2 n1 = component1->getTransform()->toGlobal(n,false);	

		double angle = component1->getTransform()->
					angleWith(*(component2->getTransform()));

		Real limit = 20.0*3.141593/180.0;
		angle = std::max(-limit, std::min(limit, angle));

		Debug.drawVector(component1->getPosition(), r1);	

		Debug.drawVector(component2->getPosition(), r2);	

		Debug.drawLine(component1->getPosition(), 
							   component2->getPosition(),
							   Vec3(0,1,1));	

		return Mat(2, 1, {std::max(-10.0,std::min(10.0,dot(C,n1)*0.15)), 
						  std::max(-10.0,std::min(10.0,angle*0.15))});
	}

	Hinge::Hinge(const std::shared_ptr<PhysicsComponent> component1,
			 	 const std::shared_ptr<PhysicsComponent> component2,
				 const Real w)
			:Joint(component1,component2)
	{
		Vec p = ((1.0-w)*component1->getPosition()
				+w*component2->getPosition());
		setAnchor1(p);
		setAnchor2(p);
	}

	Mat Hinge::getJ()
	{
		Vec r1 = getR1();
		Vec r2 = getR2();

		Mat J(3, 6);

		J(0,0) = -1.0;
		J(1,1) = -1.0;
		J(0,2) = r1(1);
		J(1,2) = -r1(0);

		J(0,3) = 1.0;
		J(1,4) = 1.0;
		J(0,5) = -r2(1);
		J(1,5) = r2(0);

		J(2,2) = -1.0;
		J(2,5) = 1.0;

		Debug.drawVector(component1->getPosition(), r1);	

		Debug.drawVector(component2->getPosition(), r2);	

		return J;
	}

	Mat Hinge::getC()
	{
		Vec r1 = getR1();
		Vec r2 = getR2();

		Vec C = component2->getPosition() + r2 
			  - component1->getPosition() - r1;

		return Mat(3,1, {C(0),C(1),0})*0.3;
	}
	
	Collision::Collision(const std::shared_ptr<PhysicsComponent> component1,
			 			 const std::shared_ptr<PhysicsComponent> component2,
						 const Vec& r1, const Vec& r2)
			:Joint(component1, component2)
	{
		setAnchor1(r1);
		setAnchor2(r2);
		n = r2-r1;
	}
			
	Mat Collision::getJ()
	{
		Vec r1 = getR1();
		Vec r2 = getR2();

//		Vec u = Circe::normalize(component2->getPosition() 
//							   - component1->getPosition());

		Vec2 n1 = component1->getTransform()->toGlobal(n,false);

		Mat J(1, 6);
		J(0,0) = -n1(0);
		J(0,1) = -n1(1);
		J(0,2) = -Circe::cross(r1, n1);
//		J(1,2) = -1.0;

		J(0,3) = n1(0);
		J(0,4) = n1(1);
		J(0,5) = Circe::cross(r2, n1);
//		J(1,5) = 1.0;

		return J;
	}

	Mat Collision::getC()
	{
		Vec r1 = getR1();
		Vec r2 = getR2();

		Real o1 = component1->getAngularVelocity();
		Real o2 = component2->getAngularVelocity();

		Vec C_rest = component2->getVelocity() + Vec2(-r2(1),r2(0))*o2 
			  - component1->getVelocity() - Vec2(-r1(1),r1(0))*o1;

		Vec2 n1 = component1->getTransform()->toGlobal(n,false);

		Real b_rest = dot(C_rest,n1)*0.01;


		Vec C_pen = component2->getPosition() + r2
				  - component1->getPosition() - r1;

		Real b_pen = dot(C_pen, n1)*0.02;

		return Mat(1,1, {b_pen+b_rest});
//		return Mat(1,1, {0});
	}
	
	void Collision::restrictLambda(Mat& lambda)
	{
		lambda(0) = std::max(0.0, lambda(0));
	}
}
