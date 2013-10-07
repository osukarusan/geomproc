#include <iostream>
#include <fstream>
#include "glwidget.h"


using namespace std;


GLWidget::GLWidget(QWidget *parent) :
	QGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	mesh = NULL;
	bWireframe = false;
}


bool GLWidget::loadMesh(const QString &filename)
{
	if(mesh != NULL)
		closeMesh();
	mesh = new TriangleMesh();

	if(mesh->load(filename.toAscii().data()))
	{
		resetCamera();
		bWireframe = false;
		return true;
	}

	return false;
}

void GLWidget::closeMesh()
{
	delete mesh;
	mesh = NULL;
}

void GLWidget::resetCamera()
{
	BBox bbox;

	mesh->getBBox(bbox);
	cam.initFromBBox(bbox);
	updateGL();
}

void GLWidget::initializeGL()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	cam.changeAspect(float(w) / float(h));
}

void GLWidget::paintGL()
{
	cam.setOpenGLMatrices();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(bWireframe)
		glDisable(GL_LIGHTING);
	else
		glEnable(GL_LIGHTING);
	if(mesh != NULL)
		mesh->render(bWireframe);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		initx = event->x();
		inity = event->y();
	}
	else if(event->button() == Qt::RightButton)
	{
		inity = event->y();
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons() & Qt::LeftButton)
	{
		cam.rotate(event->y()-inity, event->x()-initx);
		initx = event->x();
		inity = event->y();
		updateGL();
	}
	else if(event->buttons() & Qt::RightButton)
	{
		cam.move((event->y()-inity) * 0.01f);
		inity = event->y();
		updateGL();
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->buttons() & Qt::LeftButton)
	{
		cam.rotate(event->y()-inity, event->x()-initx);
		initx = event->x();
		inity = event->y();
		updateGL();
	}
	else if(event->buttons() & Qt::RightButton)
	{
		cam.move((event->y()-inity) * 0.01f);
		inity = event->y();
		updateGL();
	}
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_F1)
	{
		bWireframe = !bWireframe;
		updateGL();
	}
}

