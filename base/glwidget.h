#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QGLWidget>
#include <QMouseEvent>
#include <QTimer>
#include "trianglemesh.h"
#include "camera.h"


enum RenderType { RENDER_NORMAL, RENDER_VALENCE, RENDER_CORNERS,
                  RENDER_GAUSSIAN_CURVATURE, RENDER_MEDIAN_CURVATURE, NUM_RENDER_TYPES };
enum RenderMesh { RENDER_ORIGINAL, RENDER_SMOOTHED, RENDER_COLLAPSED, NUM_MESH_TYPES };


class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
    RenderType gRendertype;
    RenderMesh gRendermesh;

    int    curvatureScaleType;
    double curvatureRenderMin;
    double curvatureRenderMax;

private:
	Camera cam;
	int initx, inity;
    bool bWireframe;
	TriangleMesh *mesh;
    TriangleMesh *smoothedMesh;
    TriangleMesh *collapsedMesh;

public:
	GLWidget(QWidget *parent = 0);

	bool loadMesh(const QString &filename);
	void closeMesh();

	void resetCamera();

    void getCurvatureBounds(float& min, float& max);

    void setLaplacianSmoothParameters (int numiters, double lambda);
    void setTaubinSmoothParameters    (int numiters, double lambda, double mu);
    void setTangentialSmoothParameters(int numiters, double lambda);
    void setCollapseParameters(int numiters, double threshold, int collapses);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);

    TriangleMesh* getDisplayMesh() const;

};

#endif // GLWIDGET_H
