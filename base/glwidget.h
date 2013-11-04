#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include "trianglemesh.h"
#include "camera.h"


enum RenderType { RENDER_NORMAL, RENDER_CORNERS, RENDER_CURVATURE, NUM_RENDER_TYPES };
enum RenderMesh { RENDER_ORIGINAL, RENDER_SMOOTHED, RENDER_COLLAPSED, NUM_MESH_TYPES };


class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
    RenderType gRendertype;
    RenderMesh gRendermesh;

private:
	Camera cam;
	int initx, inity;
    bool bWireframe;
	TriangleMesh *mesh;
    TriangleMesh *smoothMesh;
    TriangleMesh *collapsedMesh;

public:
	GLWidget(QWidget *parent = 0);

	bool loadMesh(const QString &filename);
	void closeMesh();

	void resetCamera();

    void setSmoothParameters(int numiters, double lambda);
    void setCollapseParameters(int numiters, double threshold);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);

};

#endif // GLWIDGET_H
