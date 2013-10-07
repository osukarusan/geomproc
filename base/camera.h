#ifndef CAMERA_H
#define CAMERA_H


#include "glm/glm.hpp"
#include "bbox.h"


class Camera
{

private:
	float aspect, fovy, zNear, zFar;
	float initDistance, distance, angX, angY;
	vec3 VRP;

public:
	void changeAspect(float newAspect);
	void initFromBBox(const BBox &bbox);

	void rotate(float incrAngX, float incrAngY);
	void move(float incrDistance);

	void setOpenGLMatrices();

};


#endif // CAMERA_H
