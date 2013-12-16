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

    void getGaussianCurvatures(float& min, float& max) const;
    void getMedianCurvatures  (float& min, float& max) const;

    void computeGaussianCurvatures();
    void computeMedianCurvatures();
    void laplacianSmoothing(int iterations, double lambda);
    void taubinSmoothing(int iterations, double lambda, double mu);
    void tangentialSmoothing(int iterations, double lambda);
    void edgeCollapse(int iterations, double threshold, int maxCollapses = -1);

    void renderNormal();
    void renderCornerColors();
    void renderVertexValence();
    void renderGaussianCurvature(int scaletype, float min, float max);
    void renderMedianCurvature(int scaletype, float min, float max);

private:
	bool loadHeader(ifstream &fin);
	void loadVertices(ifstream &fin);
	void loadFaces(ifstream &fin);

	void addTriangle(const int tri[3]);
	void computeNormalsPerFace();

    int nVertices,  nFaces;
    vector<vec3>    vertices, normals;
    vector<int>     vTable;

    vector<float>   gaussianCurvature;
    vector<float>   medianCurvature;
    float           minKg, maxKg;
    float           minKm, maxKm;

    CornerTable cornerTable;

};


inline void TriangleMesh::getGaussianCurvatures(float &min, float &max) const {
    min = minKg;
    max = maxKg;
}

inline void TriangleMesh::getMedianCurvatures(float &min, float &max) const {
    min = minKm;
    max = maxKm;
}


#endif // TRIANGLEMESH_H
