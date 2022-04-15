#pragma once

#include "Circe/Circe.h"
#include "Game/Debug.h"

/** TODO persistence to warm-start: build simplex with former point 
	at beginning of GJK => Done
	TODO algo in 3D
	TODO EPA: instead of testing all the edges every time in getClosestEdge,
	test the added edges => Didn't work
**/

namespace Physics
{
	struct AABB : public Circe::PrimitiveCollider<AABB>
	{
		AABB();

		AABB(const Vec& center, const Vec& width);

		void refit(const AABB& v1, const AABB& v2);

		bool isInside(const AABB& v) const;

		/** AABB to AABB intersection **/ 
		virtual bool intersects(const AABB& other) const;

		Real getUnionArea(const AABB& s1) const;

		Real getArea() const;

		void setPosition(const Vec& position);

		void setMargin(const Vec& v);

		void draw(const Vec3& color);

		Real getBarneHutRatio(const Vec& dr) const;

		Vec getCoG() const;

		Real getMass() const;

		void setMass(const Real m);

		Vec center;
		Vec halfWidth;
		Vec margin;
		Real mass;
		Vec cog;
		Real gravityWidth;
	};

	struct Point : public Circe::PrimitiveCollider<AABB>
	{
		Point(const Vec& location);

		virtual bool intersects(const AABB& box) const;

		Vec position;
	};

	//https://web.archive.org/web/20090803054252/http://tog.acm.org/resources/GraphicsGems/gems/RayBox.c
	struct Ray : public Circe::PrimitiveCollider<AABB>
	{
		Ray(const Vec& origin, const Vec& direction);

		virtual bool intersects(const AABB& box) const;

		Vec origin;
		Vec dir;
	};

	struct Segment : public Circe::PrimitiveCollider<AABB>
	{
		Segment(const Vec& pStart, const Vec& pEnd);

		virtual bool intersects(const AABB& box) const;

		Vec p0;
		Vec p1;
	};

	const int SHAPE_MAX_VERTICES = 200;

	struct Contact
	{
		Vec normal;
		Vec positionBody1;
		Vec positionBody2;
		Real depth;
	};

	class Collider
	{
		public:
			Collider();

			Collider(const std::vector<Vec>& points, 
				  	 const Transform transform);

			int getSupportIndex(const Vec& direction) const;

			Vec getPosition(const int index) const;

			Vec getCenter() const;

			Real getRadius() const;

			void draw(const Vec3& color = Vec3(1,1,1)) const;

		private:
			Transform m_transform;
			std::vector<Vec> m_points;
			Real m_radius;
	};


	struct Vertex
	{
		int index;
		Vec position;	// Position of vertex in Minkowski space
		Vec p1;			// Position of first vertex in global space
		Vec p2;			// Position of second vertex in global space
	};

	struct Edge
	{
		Vec normal;
		Real distance;
		int index;
		Vertex v1;
		Vertex v2;
	};

	class Simplex
	{
		public:
			void append(const Vertex& point);

			Vertex operator()(const int index) const;
			
			Vertex& operator()(const int index);

			void remove(const int index);

			int getSize() const;	

			void draw();

		private:
			std::vector<Vertex> m_points;
			int m_size = 0;
	};

	class Polytope
	{
		public:
			Polytope(const Simplex& simplex);

			void insert(const Vertex& point, const int index);

			Edge getClosestEdge() const;

			void draw();

		private:
			std::vector<Vertex> m_points;

			Edge createEdge(const int index) const;
	};

	class CollisionDetector
	{
		public:
			std::shared_ptr<Contact> collide
				(const Collider& s1, const Collider& s2, 
				 Simplex& simplex) const;

			std::shared_ptr<Contact> collide
				(const Collider& s1, const Collider& s2) const;

			std::shared_ptr<Contact> solveEPA
				(Simplex& simplex, 
				 const Collider& s1, const Collider& s2) const;

			bool solveGJK(Simplex& simplex, const Collider& s1, 
						const Collider& s2) const;

			bool broadPhase(const Collider& s1, const Collider& s2) const;

		private:
			bool isOriginInSimplex(Simplex& simplex, 
						   		   Vec& d) const;

			Vertex getSupport(const Collider& s1, const Collider& s2, 
						    const Vec& direction) const;

			Vec2 getBarycentricCoordinates(const Vec2& p, const Vec2& a, 
										   const Vec2& b) const;

			Vec3 getBarycentricCoordinates(const Vec3& p,
										   const Vec3& a, const Vec3& b,
										   const Vec3& c) const;

			std::shared_ptr<Contact> createContact(const Edge& edge) const;
	};
}
