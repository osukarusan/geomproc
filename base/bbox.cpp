#include "bbox.h"


BBox::BBox()
{
}

BBox::BBox(const vec3 &minPoint, const vec3 &maxPoint)
{
	limits[0] = minPoint;
	limits[1] = maxPoint;
}


void BBox::set(const vec3 &minPoint, const vec3 &maxPoint)
{
	limits[0] = minPoint;
	limits[1] = maxPoint;
}

vec3 BBox::getCenter() const
{
	vec3 center = vec3((limits[0].x + limits[1].x) / 2.f,
										 (limits[0].y + limits[1].y) / 2.f,
										 (limits[0].z + limits[1].z) / 2.f);

	return center;
}

float BBox::getCircumRadius() const
{
	vec3 size = limits[1] - limits[0];
	return length(size) / 2.f;
}

