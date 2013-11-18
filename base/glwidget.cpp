#include <iostream>
#include <fstream>
#include "glwidget.h"


using namespace std;


GLWidget::GLWidget(QWidget *parent) :
	QGLWidget(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	mesh = NULL;
    smoothMesh = NULL;
    collapsedMesh = NULL;
    bWireframe = false;
    gRendertype = RENDER_NORMAL;
    gRendermesh = RENDER_ORIGINAL;
}


bool GLWidget::loadMesh(const QString &filename)
{
	if(mesh != NULL)
		closeMesh();
    mesh = new TriangleMesh();

	if(mesh->load(filename.toAscii().data()))
    {
        mesh->computeCurvatures();
		resetCamera();
		bWireframe = false;
		return true;
	}

	return false;
}

void GLWidget::closeMesh()
{
    if (mesh) {
        delete mesh;
        mesh = NULL;
    }
    if (smoothMesh) {
        delete smoothMesh;
        smoothMesh = NULL;
    }
    if (collapsedMesh) {
        delete collapsedMesh;
        collapsedMesh = NULL;
    }
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

    TriangleMesh* renderMesh;
    switch (gRendermesh) {
        case RENDER_ORIGINAL:   renderMesh = mesh; break;
        case RENDER_SMOOTHED:   renderMesh = smoothMesh; break;
        case RENDER_COLLAPSED:  renderMesh = collapsedMesh; break;
        default:                renderMesh = mesh; break;
    }

    if(renderMesh != NULL) {

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (bWireframe) {
            glPolygonOffset(10, 10);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glColor3f(1.f, 1.f, 1.f);
            renderMesh->renderNormal();
            glDisable(GL_POLYGON_OFFSET_FILL);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        switch (gRendertype) {
            case RENDER_NORMAL:
                if (bWireframe) glColor3f(0.0f, 0.0f, 0.0f);
                else            glColor3f(0.7f, 0.7f, 0.9f);
                renderMesh->renderNormal();
                break;
            case RENDER_VALENCE:
                renderMesh->renderVertexValence();
                break;
            case RENDER_CORNERS:
                renderMesh->renderCornerColors();
                break;
            case RENDER_CURVATURE:
                renderMesh->renderCurvature();
                break;
            default:
                break;
        }
    }
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

void GLWidget::setSmoothParameters(int numiters, double lambda) {
    if (!mesh) return;
    if (smoothMesh)
        delete smoothMesh;

    smoothMesh = mesh->getCopy();
    smoothMesh->laplacianSmoothing(numiters, lambda);
    this->updateGL();
}

void GLWidget::setCollapseParameters(int numiters, double threshold, int collapses) {
    if (!mesh) return;
    if (collapsedMesh)
        delete collapsedMesh;

    collapsedMesh = mesh->getCopy();
    collapsedMesh->edgeCollapse(numiters, threshold, collapses);
    this->updateGL();
}
