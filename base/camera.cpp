#include <iostream>
#include <GL/glu.h>
#include "camera.h"


using namespace std;


#define MIN_ZOOM 0.01f
#define MAX_ZOOM 10.f


void Camera::changeAspect(float newAspect)
{
	aspect = newAspect;
}

void Camera::initFromBBox(const BBox &bbox)
{
	float radius = bbox.getCircumRadius();

	VRP = bbox.getCenter();
	fovy = 60.f;
	initDistance = 2.f * radius;
	distance = initDistance;
	zNear = radius;
	zFar = 3.f * radius;
	angX = 0.f;
	angY = 0.f;
}

void Camera::rotate(float incrAngX, float incrAngY)
{
	angX += incrAngX;
	if(angX < -60.f)
		angX = -60.f;
	else if(angX > 60.f)
		angX = 60.f;
	angY += incrAngY;
}

void Camera::move(float incrDistance)
{
	float move = incrDistance * initDistance;

	if(zNear + move < MIN_ZOOM * initDistance)
	{
		distance += MIN_ZOOM * initDistance - zNear;
		zFar += MIN_ZOOM * initDistance - zNear;
		zNear = MIN_ZOOM * initDistance;
	}
	else if(zFar + move > MAX_ZOOM * initDistance)
	{
		distance += MAX_ZOOM * initDistance - zFar;
		zNear += MAX_ZOOM * initDistance - zFar;
		zFar = MAX_ZOOM * initDistance;
	}
	else
	{
		distance += move;
		zNear += move;
		zFar += move;
	}
}

void Camera::setOpenGLMatrices()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -distance);
	glRotatef(angX, 1, 0, 0);
	glRotatef(angY, 0, 1, 0);
	glTranslatef(-VRP.x, -VRP.y, -VRP.z);
}

