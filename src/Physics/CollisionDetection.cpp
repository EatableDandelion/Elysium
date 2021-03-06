#include "Physics/CollisionDetection.h"

//https://dyn4j.org/2010/04/gjk-gilbert-johnson-keerthi/#gjk-origin
//https://www.youtube.com/watch?v=ajv46BSqcK4
namespace Physics
{
	std::vector<Vec> ColliderFromMesh(const Elysium::MeshData& mesh)
	{
		std::vector<Vec> positions;
		for(Elysium::Vertex vertex : mesh.vertices)
		{
			Vec v;
			v(0) = vertex.x;
			v(1) = vertex.y;
			if(DIMENSION == 3)
				v(2) = vertex.z;

			positions.push_back(v);
		}

		return positions;
	}

	Collider::Collider()
	{}

	Collider::Collider(const std::vector<Vec>& points, 
					   const Transform transform)
					  : m_transform(transform)
	{
		int n = 0;
		Vec center;
		for(Vec p : points)
		{
			center = center + p;
			n++;
		}
		center = center/n;

		m_radius = 0.0;
		for(Vec p : points)
		{
			Vec newPos= p - center;
			for(int i = 0; i<DIMENSION; i++) 
				newPos(i) *= transform->getScale()(i);

			m_points.push_back(newPos);
			m_radius = std::max(std::sqrt(dot(newPos,newPos)), m_radius);
		}
	}

	int Collider::getSupportIndex(const Vec& direction) const
	{
		Vec dir = m_transform->toLocal(direction, false);
		Real dmax = dot(dir, m_points[0]);
		int index = 0;

		int i = 0;
		for(Vec p : m_points)
		{
			Real d = dot(dir, p);
			if(d > dmax)
			{
				index = i;
				dmax = d;
			}
			i++;
		}

		return index;
	}

	Vec Collider::getPosition(const int index) const
	{
		return m_transform->toGlobal(m_points[index], true);
	}

	Vec Collider::getCenter() const
	{
		return m_transform->getPosition();
	}

	Real Collider::getRadius() const
	{
		return m_radius;
	}

	void Collider::draw(const Vec3& c) const
	{
		for(int i = 0; i<m_points.size()-1; i++)
		{
			Vec p1 = m_transform->toGlobal(m_points[i], true);
			Vec p2 = m_transform->toGlobal(m_points[i+1], true);
			Debug.drawLine(p1, p2, c);	
		}
		Vec p1 = m_transform->toGlobal(m_points[m_points.size()-1], true);
		Vec p2 = m_transform->toGlobal(m_points[0], true);
		
		Debug.drawLine(p1, p2, c);	
	}

	void Simplex::append(const Vertex& point)
	{
		m_points.push_back(point);
		m_size++;
	}

	Vertex Simplex::operator()(const int index) const
	{
		return m_points[index];
	}

	Vertex& Simplex::operator()(const int index)
	{
		return m_points[index];
	}

	void Simplex::remove(const int index)
	{
		m_points.erase(m_points.begin()+index);
		m_size--;
	}

	int Simplex::getSize() const
	{
		return m_size;
	}

	void Simplex::draw()
	{
		for(int i = 0; i<m_size-1; i++)
		{
			Debug.drawLine(m_points[i].position, m_points[i+1].position);	
		}
		Debug.drawLine(m_points[m_size-1].position, m_points[0].position);	

	}

	Polytope::Polytope(const Simplex& simplex)
	{
		for(int i = 0; i<simplex.getSize(); i++)
		{
			m_points.push_back(simplex(i));
		}
/*
		m_closestEdge = createEdge(0);

		for(int i = 1; i<simplex.getSize(); i++)
		{
			Edge e = createEdge(i);
			if(e.distance < m_closestEdge.distance)
			{
				m_closestEdge = e;
			}	
		}
*/	}

	void Polytope::insert(const Vertex& point, const int index)
	{
	
		m_points.insert(m_points.begin()+index, point);

		/** 
		  	I'm assuming that everytime we add a vertex, one of the 
		    2 new edges is closer than the previous edge.
		**/
/*		Edge leftEdge = createEdge(index-1);
		Edge rightEdge = createEdge(index);
	
		if(m_closestEdge.distance < std::min(leftEdge.distance,
											 rightEdge.distance))
			return false;

		if(leftEdge.distance < rightEdge.distance)
		{
			m_closestEdge = leftEdge;
		}
		else
		{
			m_closestEdge = rightEdge;
		}
		return true;
*/	}

	Edge Polytope::getClosestEdge() const
	{
		Edge minEdge = createEdge(0);
		for(int i = 1; i<m_points.size(); i++)
		{
			Edge edge = createEdge(i);

			if(edge.distance < minEdge.distance)
			{
				minEdge = edge;
			}
		}
		return minEdge;
	}

	Edge Polytope::createEdge(const int index) const
	{
		int i = index;
		if(i == -1) i = m_points.size()-1;
		int j = index+1;
		if(j == m_points.size()) j = 0;
	
		Vec A = m_points[i].position;
		Vec B = m_points[j].position;
		Vec AB = B - A;
		Vec n = normalize(tripleProduct(AB, A, AB));
		Real d = dot(n, A);

		Edge e;
		e.normal = n;
		e.distance = d;
		e.v1 = m_points[i];
		e.v2 = m_points[j];
		e.index = j; 

		return e;
	}


	void Polytope::draw()
	{
		Debug.drawLine(Vec2(-0.2,0.0),Vec2(0.2,0.0));	
		Debug.drawLine(Vec2(0.0,-0.2),Vec2(0.0,0.2));	

		for(int i = 0; i<m_points.size()-1; i++)
		{
			Debug.drawLine(m_points[i].position, m_points[i+1].position);	
		}
		Debug.drawLine(m_points[m_points.size()-1].position, 
								   m_points[0].position);	
	}

	std::shared_ptr<Contact> CollisionDetector::collide
								(const Collider& s1, 
								 const Collider& s2,
								 Simplex& simplex) const
	{
		if(broadPhase(s1, s2))
		{
			if(solveGJK(simplex, s1, s2))
			{
			//	s1.draw(Vec3(1,0,0));
			//	s2.draw(Vec3(1,0,0));
				return solveEPA(simplex, s1, s2);							
			}
		}
		return nullptr;
	}

	std::shared_ptr<Contact> CollisionDetector::collide
								(const Collider& s1, 
								 const Collider& s2) const
	{
		Simplex simplex;
		return collide(s1, s2, simplex);
	}

	std::shared_ptr<Contact> CollisionDetector::solveEPA
			(Simplex& simplex, const Collider& s1, const Collider& s2) const
	{
		int maxIteration = 10;
		int iteration = 0;

		Polytope polytope(simplex);
	
		while(iteration < maxIteration)
		{
			iteration++;

			Edge e = polytope.getClosestEdge();
	
			Vertex p = getSupport(s1, s2, e.normal);

			Real d = dot(p.position, e.normal);

			if(d - e.distance < 1e-3)
			{
				simplex.remove(2);
				simplex(0) = e.v1;
				simplex(1) = e.v2;

				return createContact(e);
			}
			else
			{
				polytope.insert(p, e.index);
			}

		}
		
		return nullptr;		
	}
			
	std::shared_ptr<Contact> CollisionDetector::createContact
				(const Edge& e) const
	{
		std::shared_ptr<Contact> contact = std::make_shared<Contact>();
		contact->normal = e.normal;
		contact->depth = e.distance;

		Vec w = getBarycentricCoordinates(Vec(), e.v1.position,
												 e.v2.position);

		Vec pos1 = e.v1.p1*w(0) + e.v2.p1*w(1);
		Vec pos2 = e.v1.p2*w(0) + e.v2.p2*w(1);

		contact->positionBody1 = pos1;
		contact->positionBody2 = pos2;

		
		Debug.drawEllipse(pos1,Vec2(0.1,0.1),Vec3(1,0,0));	
		Debug.drawEllipse(pos2,Vec2(0.1,0.1),Vec3(1,0,0));	
		Debug.drawVector(pos2,e.normal,e.distance);	
		return contact;

	}

	bool CollisionDetector::solveGJK(Simplex& simplex, 
									 const Collider& s1, 
									 const Collider& s2) const
	{	
		Vec d;
	   
		if(simplex.getSize() == 0)
		{
			d = s2.getCenter() - s1.getCenter();
			simplex.append(getSupport(s1, s2, d));
			d = Vec() - simplex(0).position;
		}
		else
		{
			for(int i = 0; i<simplex.getSize(); i++)
			{
				Vertex v = simplex(i);
				int index1 = v.index % SHAPE_MAX_VERTICES;
				int index2 = (v.index - index1) / SHAPE_MAX_VERTICES;
				v.p1 = s1.getPosition(index1);
				v.p2 = s2.getPosition(index2);
				v.position = v.p1 - v.p2;
				simplex(i) = v; 
			}
			if(isOriginInSimplex(simplex, d))
			{
				return true;
			}
		}	
		while(true)
		{
			Vertex A = getSupport(s1, s2, d);

			if(dot(A.position, d) < 1.0e-6)
				return false;

			simplex.append(A);
			if(isOriginInSimplex(simplex, d))
			{
				return true;
			}
		}

		return false;
	}

	bool CollisionDetector::broadPhase(const Collider& s1, 
									   const Collider& s2) const
	{
		Vec dx = s2.getCenter() - s1.getCenter();

		if(std::sqrt(dot(dx,dx)) <= (s1.getRadius() + s2.getRadius()))
		{
			return true;
		}

		return false;
	}

	bool CollisionDetector::isOriginInSimplex(Simplex& simplex, 
											  Vec& d) const
	{
		if(simplex.getSize() == 2)
		{
			Vec B = simplex(0).position;
			Vec A = simplex(1).position;
			Vec AB = B - A;
			Vec AO = Vec() - A;
			d = tripleProduct(AB, AO, AB);

			return false;
		}
		else if(simplex.getSize() == 3)
		{
			Vec C = simplex(0).position;
			Vec B = simplex(1).position;
			Vec A = simplex(2).position;

			Vec AB = B - A;
			Vec AC = C - A;
			Vec AO = Vec() - A;

			Vec ABperp = tripleProduct(AC, AB, AB);
			Vec ACperp = tripleProduct(AB, AC, AC);

			if(dot(ABperp, AO) > 0)
			{
				simplex.remove(0);
				d = ABperp;
				return false;
			}

			if(dot(ACperp, AO) > 0)
			{
				simplex.remove(1);
				d = ACperp;
				return false;
			}

			return true;
		}
		return false;
	}

	Vertex CollisionDetector::getSupport(const Collider& s1, const Collider& s2, 
									  const Vec& direction) const
	{
		Vertex v;

		int index1 = s1.getSupportIndex(direction);
		int index2 = s2.getSupportIndex(direction*(-1.0));

		v.index = index1 + index2 * SHAPE_MAX_VERTICES;

		v.p1 = s1.getPosition(index1);
		v.p2 = s2.getPosition(index2);
		
		v.position = v.p1 - v.p2;
		
		return v;
	}

	Vec2 CollisionDetector::getBarycentricCoordinates(const Vec2& P, 
													  const Vec2& A, 
													  const Vec2& B) const
	{
		Vec2 AB = B - A;
		Vec2 AP = P - A;
		Real w = dot(AP,AB)/dot(AB,AB);
		return Vec2(1.0-w, w);
	}


	Vec3 CollisionDetector::getBarycentricCoordinates(const Vec3& p,
													  const Vec3& a, 
													  const Vec3& b, 
													  const Vec3& c) const
	{
		//https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates					   			 
		Vec3 v0 = b - a;
		Vec3 v1 = c - a;
		Vec3 v2 = p - a;

		Real d00 = dot(v0, v0);
		Real d01 = dot(v0, v1);
		Real d11 = dot(v1, v1);
		Real d20 = dot(v2, v0);
		Real d21 = dot(v2, v1);

		Real denom = d00 * d11 - d01 * d01;

		Vec3 res;

		res(1) = (d11 * d20 - d01 * d21) / denom;
		res(2) = (d00 * d21 - d01 * d20) / denom;
		res(0) = 1.0 - res(1) - res(2);

		return res;
	}

}
