#ifndef BBOX_H
#define BBOX_H


#include "glm/glm.hpp"


using namespace glm;


struct BBox
{

	vec3 limits[2];

	BBox();
	BBox(const vec3 &minPoint, const vec3 &maxPoint);

	void set(const vec3 &minPoint, const vec3 &maxPoint);

	vec3 getCenter() const;
	float getCircumRadius() const;

};


#endif // BBOX_H
