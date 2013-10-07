#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H


#include <fstream>
#include <vector>
#include "glm/glm.hpp"
#include "bbox.h"


using namespace std;
using namespace glm;


class TriangleMesh
{

private:
	int nVertices, nFaces;
	vector<vec3> vertices, normals;
	vector<int> vTable;

public:
	TriangleMesh();

	bool load(const char *filename);
	void free();
	void getBBox(BBox &bbox);
	void render(bool bWireframe);

private:
	bool loadHeader(ifstream &fin);
	void loadVertices(ifstream &fin);
	void loadFaces(ifstream &fin);

	void addTriangle(const int tri[3]);
	void computeNormalsPerFace();

};


#endif // TRIANGLEMESH_H
