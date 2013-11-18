#include <iostream>
#include <string.h>
#ifdef MACOSX
#include <OpenGL/glu.h>
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#include <GL/glu.h>
#endif
#include "trianglemesh.h"
#include <cmath>
#include <queue>


const float cornerColors[8][3] = {
    {1.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 1.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 1.0f},
    {0.0f, 0.0f, 0.0f}
};


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

    cornerTable.buildTable(vTable);

	return true;
}

void TriangleMesh::free()
{
	vertices.clear();
	normals.clear();
}

TriangleMesh* TriangleMesh::getCopy()
{
    TriangleMesh* res = new TriangleMesh;
    res->nFaces = this->nFaces;
    res->nVertices = this->nVertices;
    res->vertices = this->vertices;
    res->normals = this->normals;
    res->vTable = this->vTable;
    res->cornerTable = this->cornerTable;
    return res;
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

void TriangleMesh::renderNormal()
{
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < (int)vTable.size(); i += 3)
    {
        glNormal3fv(&normals[i/3].x);
        glVertex3fv(&vertices[vTable[i]].x);
        glVertex3fv(&vertices[vTable[i+1]].x);
        glVertex3fv(&vertices[vTable[i+2]].x);
    }
    glEnd();
}

void TriangleMesh::renderCornerColors()
{
    std::vector<int> colorIndex(vTable.size(), -1);
    for (int i = 0; i < (int)vTable.size(); i++) {
        if (colorIndex[i] >= 0) continue;

        int colori = 0;
        int corner = i;
        do {
            colorIndex[corner] = colori;
            corner = cornerTable.clockwise(corner);
            colori = (colori+1)%8;
        } while (corner != i && corner >= 0);
    }

    glBegin(GL_TRIANGLES);
    for(int i = 0; i < (int)vTable.size(); i += 3)
    {
        glNormal3fv(&normals[i/3].x);
        glColor3fv(cornerColors[colorIndex[i]]);
        glVertex3fv(&vertices[vTable[i]].x);
        glColor3fv(cornerColors[colorIndex[i+1]]);
        glVertex3fv(&vertices[vTable[i+1]].x);
        glColor3fv(cornerColors[colorIndex[i+2]]);
        glVertex3fv(&vertices[vTable[i+2]].x);
    }
    glEnd();
}

void TriangleMesh::renderVertexValence()
{
    std::vector<int> valence(nVertices, 0);
    for (int i = 0; i < (int)vTable.size(); i++) {
        int vid = cornerTable.vertex(i);
        if (!valence[vid]) {
            valence[vid] = cornerTable.valence(i);
            if (valence[vid] < 4)
                valence[vid] = 0;
            else
                valence[vid] = 1;
        }
    }

    glBegin(GL_TRIANGLES);
    for(int i = 0; i < (int)vTable.size(); i += 3)
    {
        glNormal3fv(&normals[i/3].x);
        glColor3fv(cornerColors[valence[cornerTable.vertex(i)]%8]);
        glVertex3fv(&vertices[vTable[i]].x);
        glColor3fv(cornerColors[valence[cornerTable.vertex(i+1)]%8]);
        glVertex3fv(&vertices[vTable[i+1]].x);
        glColor3fv(cornerColors[valence[cornerTable.vertex(i+2)]%8]);
        glVertex3fv(&vertices[vTable[i+2]].x);
    }
    glEnd();
}

void TriangleMesh::renderCurvature()
{
    std::vector<vec3> vcolor(nVertices);
    for (int i = 0; i < nVertices; i++) {
        float r, g, b;
        float t = 100*gaussianCurvature[i];
        if (t < 0) {
            r = t/minKg;
            g = 1.0 - t/minKg;
            b = 0.0;
        }
        else if (t >= 0) {
            r = 0.0;
            g = 1.0 - t/maxKg;
            b = t/maxKg;
        }
        vcolor[i] = vec3(r, g, b);
    }

    glBegin(GL_TRIANGLES);
    for(int i = 0; i < (int)vTable.size(); i += 3)
    {
        glNormal3fv(&normals[i/3].x);
        glColor3fv(&vcolor[vTable[i]].x);
        glVertex3fv(&vertices[vTable[i]].x);
        glColor3fv(&vcolor[vTable[i+1]].x);
        glVertex3fv(&vertices[vTable[i+1]].x);
        glColor3fv(&vcolor[vTable[i+2]].x);
        glVertex3fv(&vertices[vTable[i+2]].x);
    }
    glEnd();
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
    normals.clear();

	for(i=0; i<vTable.size(); i+=3)
	{
        normal = cross(vertices[vTable[i+1]] - vertices[vTable[i]],
                       vertices[vTable[i+2]] - vertices[vTable[i]]);
        if(length(normal) < 1e-5)
            normal = vec3(0.f, 0.f, 0.f);
        else
			normal = normalize(normal);
		normals.push_back(normal);
	}
}

void TriangleMesh::computeCurvatures()
{
    gaussianCurvature = std::vector<double>(nVertices, 0);

    std::vector<double> angleSum(nVertices, 0);
    std::vector<double> areaSum (nVertices, 0);

    for (int i = 0; i < (int)vTable.size(); i++) {

        int vid = vTable[i];
        if (vid < 0) continue;

        // get the neighbor vertices
        int vi1 = cornerTable.vertex(cornerTable.next(i));
        int vi2 = cornerTable.vertex(cornerTable.prev(i));

        if (vi1 >= 0 || vi2 >= 0) {
            // vectors from center to neighbor vertices
            vec3 v0 = vertices[vid];
            vec3 v1 = vertices[vi1] - v0;
            vec3 v2 = vertices[vi2] - v0;

            if (length(v1) > 1e-5 && length(v2) > 1e-5) {
                // sum the area of this corner
                areaSum[vid] += 0.5*(cross(v1, v2).length());

                // sum the angle of this corner
                v1 = normalize(v1);
                v2 = normalize(v2);
                angleSum[vid] += acos(dot(v1, v2));
            }
        }
    }

    // Kg = (2*pi - Sum(alpha))/(1/3 * Sum(area))
    for (int i = 0; i < nVertices; i++) {
        gaussianCurvature[i] = (2.0*M_PI - angleSum[i])/((1.0/3.0)*areaSum[i]);
    }

    minKg = gaussianCurvature[0];
    maxKg = gaussianCurvature[0];
    for (int i = 1; i < nVertices; i++) {
        minKg = std::min(minKg, gaussianCurvature[i]);
        maxKg = std::max(maxKg, gaussianCurvature[i]);
    }

    std::cout << "Kg min = " << minKg << std::endl;
    std::cout << "Kg max = " << maxKg << std::endl;

}

void TriangleMesh::laplacianSmoothing(int iterations, double lambda) {

    // precompute neighbor information
    std::vector<std::vector<int> > neighbors(nVertices);
    for (int i = 0; i < (int)vTable.size(); i++) {
        int vi  = cornerTable.vertex(i);
        int vij = cornerTable.vertex(cornerTable.next(i));
        neighbors[vi].push_back(vij);
    }

    // smoothing
    std::vector<vec3>& v = vertices;
    for (int iter = 0; iter < iterations; iter++) {

        // L(v_i) = 1/N*Sum(v_ij) - v_i
        std::vector<vec3> laplacian(nVertices);
        for (int i = 0; i < nVertices; i++) {
            vec3 sum(0,0,0);
            for (int j = 0; j < (int)neighbors[i].size(); j++) {
                sum += v[neighbors[i][j]];
            }
            laplacian[i] = sum/float(neighbors[i].size()) - v[i];
        }

        // v_i' = v_i + lambda * L(v_i)
        for (int i = 0; i < nVertices; i++) {
            v[i] = v[i] + float(lambda)*laplacian[i];
        }
    }
    computeNormalsPerFace();
    computeCurvatures();
}


void TriangleMesh::edgeCollapse(int iterations, double threshold, int maxCollapses) {

    BBox bbox;
    this->getBBox(bbox);
    double tlen = 2*threshold*bbox.getCircumRadius();
    if (maxCollapses < 0) maxCollapses = vTable.size();

    for (int iter = 0; iter < iterations; iter++) {

        std::vector<bool> validVertex(nVertices, true);
        std::vector<bool> validTriangle( nFaces, true);
        int finalFaces = nFaces;
        int collapses  = 0;

        // priorize edges from smallest to largest
        std::priority_queue<std::pair<float, int> > PQ;
        for (int i = 0; i < (int)vTable.size(); i++) {
            int vi = cornerTable.vertex(i);
            int vf = cornerTable.vertex(cornerTable.next(i));
            // collapse only edges smaller than threshold
            float elen = length(vertices[vi] - vertices[vf]);
            if (elen <= tlen) {
                PQ.push(std::pair<float, int>(-elen, i));
            }
        }

        // for each edge in the priority queue
        while (PQ.size() && collapses < maxCollapses) {

            // remove top
            std::pair<float, int> pqi = PQ.top();
            PQ.pop();

            // get edge information
            int ci = pqi.second;                // corner ini
            int cf = cornerTable.next(ci);      // corner final
            int cl = cornerTable.prev(ci);      // corner at edge left side
            int cr = cornerTable.opposite(cl);  // corner at edge right side
            int vi = cornerTable.vertex(ci);
            int vf = cornerTable.vertex(cf);
            int tl = cornerTable.triangle(cl);
            int tr = cornerTable.triangle(cr);

            // check both vertices and corners are still valid
            if (   !validVertex[vi]   || !validVertex[vf]
                || !validTriangle[tl] || !validTriangle[tr])
                continue;

            // collapse only edges smaller than threshold
            if (length(vertices[vi] - vertices[vf]) > tlen)
                continue;

            // if vertex valence is 3 or less, do not collapse
            if (cornerTable.valence(cl) < 4 || cornerTable.valence(cr) < 4)
                continue;

            // if final opposite vertices are the same, do not collapse
            int crp  = cornerTable.prev(cr);
            int crn  = cornerTable.next(cr);
            int cio1 = cornerTable.opposite(ci);
            int cfo1 = cornerTable.opposite(cf);
            int cio2 = cornerTable.opposite(crp);
            int cfo2 = cornerTable.opposite(crn);
            if (   cornerTable.vertex(cio1) == cornerTable.vertex(cfo1)
                || cornerTable.vertex(cio2) == cornerTable.vertex(cfo2))
                continue;

            // if face normals would invert, do not collapse
            bool inversion = true;
            int corner = cornerTable.clockwise(ci);
            int lastci = cornerTable.counterclockwise(ci);
            do {
                vec3 vcn = vertices[cornerTable.vertex(cornerTable.next(corner))];
                vec3 vcp = vertices[cornerTable.vertex(cornerTable.prev(corner))];
                vec3 nb = cross(vcn - vertices[vi], vcp - vertices[vi]);
                vec3 na = cross(vcn - vertices[vf], vcp - vertices[vf]);
                inversion = dot(nb, na) < 0 || length(nb) < 1e-5 || length(na) < 1e-5;
                corner = cornerTable.clockwise(corner);
            } while (!inversion && corner != lastci && corner >= 0);
            if (inversion)
                continue;

            // update vtable
            std::vector<int>& vtable = cornerTable.getVTable();
            corner = ci;
            do {
                vTable[corner] = vf;
                vtable[corner] = vf;
                corner = cornerTable.clockwise(corner);
            } while (corner != ci && corner >= 0);

            // update otable
            std::vector<int>& otable = cornerTable.getOTable();
            otable[cio1] = cfo1;
            otable[cfo1] = cio1;
            otable[cio2] = cfo2;
            otable[cfo2] = cio2;

            // mark vertex and corners as invalid
            validVertex  [vi] = false;
            validTriangle[tl] = false;
            validTriangle[tr] = false;
            finalFaces -= 2;
            collapses++;
        }

        // final cleanup
        std::vector<int> vertmap(nVertices, -2);
        nVertices = 0;
        for (int i = 0; i < (int)validVertex.size(); i++) {
            if (validVertex[i])
                vertmap[i] = nVertices++;
            else
                vertices.erase(vertices.begin() + nVertices);
        }

        std::vector<int> newfaces(3*finalFaces, -1);
        int fid = 0;
        for (int i = 0; i < (int)vTable.size(); i += 3) {
            if (validTriangle[i/3]) {
                if (vertmap[vTable[i]] >= 0 &&
                    vertmap[vTable[i+1]] >= 0 &&
                    vertmap[vTable[i+2]] >= 0)
                {
                    newfaces[fid    ] = vertmap[vTable[i    ]];
                    newfaces[fid + 1] = vertmap[vTable[i + 1]];
                    newfaces[fid + 2] = vertmap[vTable[i + 2]];
                    fid += 3;
                }
                else
                {
                    finalFaces--;
                    newfaces.resize(3*finalFaces);
                }
            }
        }

        vTable = newfaces;
        nFaces = finalFaces;

        cornerTable.buildTable(vTable);

        std::cout << "Verts = " << nVertices << std::endl;
        std::cout << "Faces = " << nFaces << std::endl;
    }

    computeNormalsPerFace();
    computeCurvatures();

}

