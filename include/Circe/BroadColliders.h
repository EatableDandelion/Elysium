#pragma once

#include "BVH.h"

namespace Circe
{
	struct AABB : public PrimitiveVolume<AABB>
	{
		AABB();

		AABB(const Vec<DIMENSION>& center, const Vec<DIMENSION>& width);

		void refit(const AABB& v1, const AABB& v2);

		bool isInside(const AABB& v) const;

		/** AABB to AABB intersection **/ 
		virtual bool intersects(const AABB& other) const;

		Real getUnionArea(const AABB& s1) const;

		Real getArea() const;

		void setPosition(const Vec<DIMENSION>& position);
		
		void setSize(const Vec<DIMENSION>& size);

		void setMargin(const Vec<DIMENSION>& v);

		Vec<DIMENSION> center;
		Vec<DIMENSION> halfWidth;
		Vec<DIMENSION> margin;
	};

	struct Point : public PrimitiveVolume<AABB>
	{
		Point(const Vec<DIMENSION>& location);

		virtual bool intersects(const AABB& box) const;

		Vec<DIMENSION> position;
	};

	//https://web.archive.org/web/20090803054252/http://tog.acm.org/resources/GraphicsGems/gems/RayBox.c
	struct Ray : public PrimitiveVolume<AABB>
	{
		Ray(const Vec<DIMENSION>& origin, const Vec<DIMENSION>& direction);

		virtual bool intersects(const AABB& box) const;

		Vec<DIMENSION> origin;
		Vec<DIMENSION> dir;
	};

	struct Segment : public Circe::PrimitiveVolume<AABB>
	{
		Segment(const Vec<DIMENSION>& pStart, const Vec<DIMENSION>& pEnd);

		virtual bool intersects(const AABB& box) const;

		Vec<DIMENSION> p0;
		Vec<DIMENSION> p1;
	};
}
