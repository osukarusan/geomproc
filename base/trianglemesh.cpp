#include <iostream>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "trianglemesh.h"


TriangleMesh::TriangleMesh()
{
}

bool TriangleMesh::load(const char *filename)
{
	ifstream fin;

	fin.open(filename, ios_base::in | ios_base::binary);
	if(!fin.is_open())
		return false;
	if(!loadHeader(fin))
	{
		fin.close();
		return false;
	}
	loadVertices(fin);
	loadFaces(fin);
	fin.close();
	cout << "Mesh loaded" << endl << endl;

	return true;
}

void TriangleMesh::free()
{
	vertices.clear();
	normals.clear();
}

void TriangleMesh::getBBox(BBox &bbox)
{
	int i;

	bbox.limits[0] = vec3(1e10, 1e10, 1e10);
	bbox.limits[1] = vec3(-1e10, -1e10, -1e10);
	for(i=0; i<(int)vertices.size(); i++)
	{
		bbox.limits[0] = min(bbox.limits[0], vertices[i]);
		bbox.limits[1] = max(bbox.limits[1], vertices[i]);
	}
}

void TriangleMesh::render(bool bWireframe)
{
	int i;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(bWireframe)
	{
		glPolygonOffset(1, 1);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glColor3f(1.f, 1.f, 1.f);
	}
	else
	{
		glColor3f(0.7f, 0.7f, 0.9f);
	}
	glBegin(GL_TRIANGLES);
	for(i=0; i<(int)vTable.size(); i+=3)
	{
		glNormal3fv(&normals[i/3].x);
		glVertex3fv(&vertices[vTable[i]].x);
		glVertex3fv(&vertices[vTable[i+1]].x);
		glVertex3fv(&vertices[vTable[i+2]].x);
	}
	glEnd();
	if(bWireframe)
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.f, 0.f, 0.f);
		glBegin(GL_TRIANGLES);
		for(i=0; i<(int)vTable.size(); i+=3)
		{
			glNormal3fv(&normals[i/3].x);
			glVertex3fv(&vertices[vTable[i]].x);
			glVertex3fv(&vertices[vTable[i+1]].x);
			glVertex3fv(&vertices[vTable[i+2]].x);
		}
		glEnd();
	}
}

bool TriangleMesh::loadHeader(ifstream &fin)
{
	char line[100];

	fin.getline(line, 100);
	if(strncmp(line, "ply", 3) != 0)
		return false;
	nVertices = 0;
	fin.getline(line, 100);
	while(strncmp(line, "end_header", 10) != 0)
	{
		if(strncmp(line, "element vertex", 14) == 0)
			nVertices = atoi(&line[15]);
		fin.getline(line, 100);
		if(strncmp(line, "element face", 12) == 0)
			nFaces = atoi(&line[13]);
	}
	if(nVertices <= 0)
		return false;
	cout << "Loading triangle mesh" << endl;
	cout << "\tVertices = " << nVertices << endl;
	cout << "\tFaces = " << nFaces << endl;
	cout << endl;

	return true;
}

void TriangleMesh::loadVertices(ifstream &fin)
{
	int i;

	vertices.resize(nVertices);
	for(i=0; i<nVertices; i++)
	{
		fin.read((char *)&vertices[i].x, sizeof(float));
		fin.read((char *)&vertices[i].y, sizeof(float));
		fin.read((char *)&vertices[i].z, sizeof(float));
	}
}

void TriangleMesh::loadFaces(ifstream &fin)
{
	int i, tri[3];
	unsigned char nVrtxPerFace;

	for(i=0; i<nFaces; i++)
	{
		fin.read((char *)&nVrtxPerFace, sizeof(unsigned char));
		fin.read((char *)&tri[0], sizeof(int));
		fin.read((char *)&tri[1], sizeof(int));
		fin.read((char *)&tri[2], sizeof(int));
		addTriangle(tri);
		for(; nVrtxPerFace>3; nVrtxPerFace--)
		{
			tri[1] = tri[2];
			fin.read((char *)&tri[2], sizeof(int));
			addTriangle(tri);
		}
	}
	computeNormalsPerFace();
}

void TriangleMesh::addTriangle(const int tri[3])
{
	vTable.push_back(tri[0]);
	vTable.push_back(tri[1]);
	vTable.push_back(tri[2]);
}

void TriangleMesh::computeNormalsPerFace()
{
	unsigned int i;
	vec3 normal;

	for(i=0; i<vTable.size(); i+=3)
	{
		normal = cross(vertices[vTable[i+1]] - vertices[vTable[i]], vertices[vTable[i+2]] - vertices[vTable[i]]);
		if(length(normal) < 1e-5)
			normal = vec3(0.f, 0.f, 0.f);
		else
			normal = normalize(normal);
		normals.push_back(normal);
	}
}

