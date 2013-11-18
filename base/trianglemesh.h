#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H


#include <fstream>
#include <vector>
#include "glm/glm.hpp"
#include "bbox.h"
#include "cornertable.h"


using namespace std;
using namespace glm;


class TriangleMesh
{

public:
	TriangleMesh();

	bool load(const char *filename);
	void free();
    TriangleMesh* getCopy();

	void getBBox(BBox &bbox);

    void computeCurvatures();
    void laplacianSmoothing(int iterations, double lambda);
    void edgeCollapse(int iterations, double threshold, int maxCollapses = -1);

    void renderNormal();
    void renderCornerColors();
    void renderVertexValence();
    void renderCurvature();

private:
	bool loadHeader(ifstream &fin);
	void loadVertices(ifstream &fin);
	void loadFaces(ifstream &fin);

	void addTriangle(const int tri[3]);
	void computeNormalsPerFace();


    int nVertices,  nFaces;
    vector<vec3>    vertices, normals;
    vector<int>     vTable;

    vector<double>  gaussianCurvature;
    double          minKg, maxKg;

    CornerTable cornerTable;

};


#endif // TRIANGLEMESH_H
