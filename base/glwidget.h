#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QGLWidget>
#include <QMouseEvent>
#include "trianglemesh.h"
#include "camera.h"


class GLWidget : public QGLWidget
{
	Q_OBJECT

private:
	Camera cam;
	int initx, inity;
	bool bWireframe;
    bool bCornerColors;
	TriangleMesh *mesh;

public:
	GLWidget(QWidget *parent = 0);

	bool loadMesh(const QString &filename);
	void closeMesh();

	void resetCamera();

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
