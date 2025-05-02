#include <string>
#include <iostream>
#include <list>
#include "headers/triangle_mesh.h"
#include "headers/stb_image.h"

// Code based on the given Pastebin (https://pastebin.com/CAgp9r15)

void TriangleMesh::readOBJ(const char *obj)
{ // N.B : My formatter changed the appearance of the function
	char matfile[255];
	char grp[255];

	FILE *f;
	f = fopen(obj, "r");
	int curGroup = -1;
	while (!feof(f))
	{
		char line[255];
		if (!fgets(line, 255, f))
			break;

		std::string linetrim(line);
		linetrim.erase(linetrim.find_last_not_of(" \r\t") + 1);
		strcpy(line, linetrim.c_str());

		if (line[0] == 'u' && line[1] == 's')
		{
			sscanf(line, "usemtl %[^\n]\n", grp);
			curGroup++;
		}

		if (line[0] == 'v' && line[1] == ' ')
		{
			Vector vec;

			Vector col;
			if (sscanf(line, "v %lf %lf %lf %lf %lf %lf\n", &vec[0], &vec[1], &vec[2], &col[0], &col[1], &col[2]) == 6)
			{
				col[0] = std::min(1., std::max(0., col[0]));
				col[1] = std::min(1., std::max(0., col[1]));
				col[2] = std::min(1., std::max(0., col[2]));

				vertices.push_back(vec);
				vertexcolors.push_back(col);
			}
			else
			{
				sscanf(line, "v %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
				vertices.push_back(vec);
			}
		}
		if (line[0] == 'v' && line[1] == 'n')
		{
			Vector vec;
			sscanf(line, "vn %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
			normals.push_back(vec);
		}
		if (line[0] == 'v' && line[1] == 't')
		{
			Vector vec;
			sscanf(line, "vt %lf %lf\n", &vec[0], &vec[1]);
			uvs.push_back(vec);
		}
		if (line[0] == 'f')
		{
			TriangleIndices t;
			int i0, i1, i2, i3;
			int j0, j1, j2, j3;
			int k0, k1, k2, k3;
			int nn;
			t.group = curGroup;

			char *consumedline = line + 1;
			int offset;

			nn = sscanf(consumedline, "%u/%u/%u %u/%u/%u %u/%u/%u%n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2, &offset);
			if (nn == 9)
			{
				if (i0 < 0)
					t.vtxi = vertices.size() + i0;
				else
					t.vtxi = i0 - 1;
				if (i1 < 0)
					t.vtxj = vertices.size() + i1;
				else
					t.vtxj = i1 - 1;
				if (i2 < 0)
					t.vtxk = vertices.size() + i2;
				else
					t.vtxk = i2 - 1;
				if (j0 < 0)
					t.uvi = uvs.size() + j0;
				else
					t.uvi = j0 - 1;
				if (j1 < 0)
					t.uvj = uvs.size() + j1;
				else
					t.uvj = j1 - 1;
				if (j2 < 0)
					t.uvk = uvs.size() + j2;
				else
					t.uvk = j2 - 1;
				if (k0 < 0)
					t.ni = normals.size() + k0;
				else
					t.ni = k0 - 1;
				if (k1 < 0)
					t.nj = normals.size() + k1;
				else
					t.nj = k1 - 1;
				if (k2 < 0)
					t.nk = normals.size() + k2;
				else
					t.nk = k2 - 1;
				indices.push_back(t);
			}
			else
			{
				nn = sscanf(consumedline, "%u/%u %u/%u %u/%u%n", &i0, &j0, &i1, &j1, &i2, &j2, &offset);
				if (nn == 6)
				{
					if (i0 < 0)
						t.vtxi = vertices.size() + i0;
					else
						t.vtxi = i0 - 1;
					if (i1 < 0)
						t.vtxj = vertices.size() + i1;
					else
						t.vtxj = i1 - 1;
					if (i2 < 0)
						t.vtxk = vertices.size() + i2;
					else
						t.vtxk = i2 - 1;
					if (j0 < 0)
						t.uvi = uvs.size() + j0;
					else
						t.uvi = j0 - 1;
					if (j1 < 0)
						t.uvj = uvs.size() + j1;
					else
						t.uvj = j1 - 1;
					if (j2 < 0)
						t.uvk = uvs.size() + j2;
					else
						t.uvk = j2 - 1;
					indices.push_back(t);
				}
				else
				{
					nn = sscanf(consumedline, "%u %u %u%n", &i0, &i1, &i2, &offset);
					if (nn == 3)
					{
						if (i0 < 0)
							t.vtxi = vertices.size() + i0;
						else
							t.vtxi = i0 - 1;
						if (i1 < 0)
							t.vtxj = vertices.size() + i1;
						else
							t.vtxj = i1 - 1;
						if (i2 < 0)
							t.vtxk = vertices.size() + i2;
						else
							t.vtxk = i2 - 1;
						indices.push_back(t);
					}
					else
					{
						nn = sscanf(consumedline, "%u//%u %u//%u %u//%u%n", &i0, &k0, &i1, &k1, &i2, &k2, &offset);
						if (i0 < 0)
							t.vtxi = vertices.size() + i0;
						else
							t.vtxi = i0 - 1;
						if (i1 < 0)
							t.vtxj = vertices.size() + i1;
						else
							t.vtxj = i1 - 1;
						if (i2 < 0)
							t.vtxk = vertices.size() + i2;
						else
							t.vtxk = i2 - 1;
						if (k0 < 0)
							t.ni = normals.size() + k0;
						else
							t.ni = k0 - 1;
						if (k1 < 0)
							t.nj = normals.size() + k1;
						else
							t.nj = k1 - 1;
						if (k2 < 0)
							t.nk = normals.size() + k2;
						else
							t.nk = k2 - 1;
						indices.push_back(t);
					}
				}
			}

			consumedline = consumedline + offset;

			while (true)
			{
				if (consumedline[0] == '\n')
					break;
				if (consumedline[0] == '\0')
					break;
				nn = sscanf(consumedline, "%u/%u/%u%n", &i3, &j3, &k3, &offset);
				TriangleIndices t2;
				t2.group = curGroup;
				if (nn == 3)
				{
					if (i0 < 0)
						t2.vtxi = vertices.size() + i0;
					else
						t2.vtxi = i0 - 1;
					if (i2 < 0)
						t2.vtxj = vertices.size() + i2;
					else
						t2.vtxj = i2 - 1;
					if (i3 < 0)
						t2.vtxk = vertices.size() + i3;
					else
						t2.vtxk = i3 - 1;
					if (j0 < 0)
						t2.uvi = uvs.size() + j0;
					else
						t2.uvi = j0 - 1;
					if (j2 < 0)
						t2.uvj = uvs.size() + j2;
					else
						t2.uvj = j2 - 1;
					if (j3 < 0)
						t2.uvk = uvs.size() + j3;
					else
						t2.uvk = j3 - 1;
					if (k0 < 0)
						t2.ni = normals.size() + k0;
					else
						t2.ni = k0 - 1;
					if (k2 < 0)
						t2.nj = normals.size() + k2;
					else
						t2.nj = k2 - 1;
					if (k3 < 0)
						t2.nk = normals.size() + k3;
					else
						t2.nk = k3 - 1;
					indices.push_back(t2);
					consumedline = consumedline + offset;
					i2 = i3;
					j2 = j3;
					k2 = k3;
				}
				else
				{
					nn = sscanf(consumedline, "%u/%u%n", &i3, &j3, &offset);
					if (nn == 2)
					{
						if (i0 < 0)
							t2.vtxi = vertices.size() + i0;
						else
							t2.vtxi = i0 - 1;
						if (i2 < 0)
							t2.vtxj = vertices.size() + i2;
						else
							t2.vtxj = i2 - 1;
						if (i3 < 0)
							t2.vtxk = vertices.size() + i3;
						else
							t2.vtxk = i3 - 1;
						if (j0 < 0)
							t2.uvi = uvs.size() + j0;
						else
							t2.uvi = j0 - 1;
						if (j2 < 0)
							t2.uvj = uvs.size() + j2;
						else
							t2.uvj = j2 - 1;
						if (j3 < 0)
							t2.uvk = uvs.size() + j3;
						else
							t2.uvk = j3 - 1;
						consumedline = consumedline + offset;
						i2 = i3;
						j2 = j3;
						indices.push_back(t2);
					}
					else
					{
						nn = sscanf(consumedline, "%u//%u%n", &i3, &k3, &offset);
						if (nn == 2)
						{
							if (i0 < 0)
								t2.vtxi = vertices.size() + i0;
							else
								t2.vtxi = i0 - 1;
							if (i2 < 0)
								t2.vtxj = vertices.size() + i2;
							else
								t2.vtxj = i2 - 1;
							if (i3 < 0)
								t2.vtxk = vertices.size() + i3;
							else
								t2.vtxk = i3 - 1;
							if (k0 < 0)
								t2.ni = normals.size() + k0;
							else
								t2.ni = k0 - 1;
							if (k2 < 0)
								t2.nj = normals.size() + k2;
							else
								t2.nj = k2 - 1;
							if (k3 < 0)
								t2.nk = normals.size() + k3;
							else
								t2.nk = k3 - 1;
							consumedline = consumedline + offset;
							i2 = i3;
							k2 = k3;
							indices.push_back(t2);
						}
						else
						{
							nn = sscanf(consumedline, "%u%n", &i3, &offset);
							if (nn == 1)
							{
								if (i0 < 0)
									t2.vtxi = vertices.size() + i0;
								else
									t2.vtxi = i0 - 1;
								if (i2 < 0)
									t2.vtxj = vertices.size() + i2;
								else
									t2.vtxj = i2 - 1;
								if (i3 < 0)
									t2.vtxk = vertices.size() + i3;
								else
									t2.vtxk = i3 - 1;
								consumedline = consumedline + offset;
								i2 = i3;
								indices.push_back(t2);
							}
							else
							{
								consumedline = consumedline + 1;
							}
						}
					}
				}
			}
		}
	}
	fclose(f);
}

static bool intersectTriangleBary(const Ray &ray, const Vector &A, const Vector &B, const Vector &C,
								  IntersectionInfo &rayIntersection)
{ // Function based on the formulas from page 42
	const double epsilon = 1e-6;
	const Vector AB = B - A;
	const Vector AC = C - A;
	const Vector triangleNormal = cross(AB, AC);
	const double alignment = dot(triangleNormal, ray.unitDirection);
	const double invAlignment = 1. / alignment;
	if (std::abs(alignment) < epsilon)
		return false;

	rayIntersection.dist = invAlignment * dot(triangleNormal, A - ray.origin);
	if (rayIntersection.dist < epsilon)
		return false;

	const Vector triangleCross = cross(A - ray.origin, ray.unitDirection);
	rayIntersection.beta = dot(triangleCross, AC) * invAlignment;
	rayIntersection.gamma = -dot(triangleCross, AB) * invAlignment;
	rayIntersection.alpha = 1. - rayIntersection.beta - rayIntersection.gamma;

	return (rayIntersection.alpha >= 0 && rayIntersection.beta >= 0 && rayIntersection.gamma >= 0);
}

bool TriangleMesh::intersect(const Ray &ray, IntersectionInfo &rayIntersection)
{ // Based on code from page 46 of the textbook
	bool intersected = false;
	double boundingBoxDist;

	if (!rootNode->boundingBox.intersect(ray, boundingBoxDist))
		return false;

	std::list<Node *> nodesToVisit;
	nodesToVisit.push_front(rootNode);
	rayIntersection.dist = std::numeric_limits<double>::max();

	while (!nodesToVisit.empty())
	{
		Node *currentNode = nodesToVisit.back();
		nodesToVisit.pop_back();
		if (currentNode->leftChild)
		{
			if (currentNode->leftChild->boundingBox.intersect(ray, boundingBoxDist) && boundingBoxDist < rayIntersection.dist)
				nodesToVisit.push_back(currentNode->leftChild);
			if (currentNode->rightChild->boundingBox.intersect(ray, boundingBoxDist) && boundingBoxDist < rayIntersection.dist)
				nodesToVisit.push_back(currentNode->rightChild);
		}
		else
		{
			for (int i = currentNode->startTriangle; i < currentNode->endTriangle; i++)
			{

				const Vector A = scale * vertices[indices[i].vtxi] + center;
				const Vector B = scale * vertices[indices[i].vtxj] + center;
				const Vector C = scale * vertices[indices[i].vtxk] + center;

				IntersectionInfo currentIntersection;
				if (intersectTriangleBary(ray, A, B, C, currentIntersection) && currentIntersection.dist < rayIntersection.dist)
				{
					rayIntersection.dist = currentIntersection.dist;
					rayIntersection.alpha = currentIntersection.alpha;
					rayIntersection.beta = currentIntersection.beta;
					rayIntersection.gamma = currentIntersection.gamma;

					rayIntersection.inter = ray.origin + ray.unitDirection * currentIntersection.dist;
					rayIntersection.triangleId = i;
					intersected = true;

					const Vector normalA = normals[indices[i].ni];
					const Vector normalB = normals[indices[i].nj];
					const Vector normalC = normals[indices[i].nk];
					rayIntersection.interNormal = normalize(rayIntersection.alpha * normalA + rayIntersection.beta * normalB + rayIntersection.gamma * normalC);
				}
			}
		}
	}
	return intersected;
}

BoundingBox TriangleMesh::computeBoundingBox(int &startTriangle, int &endTriangle)
{
	BoundingBox boundingBox;
	double BMax_x = std::numeric_limits<double>::min();
	double BMax_y = std::numeric_limits<double>::min();
	double BMax_z = std::numeric_limits<double>::min();
	double BMin_x = std::numeric_limits<double>::max();
	double BMin_y = std::numeric_limits<double>::max();
	double BMin_z = std::numeric_limits<double>::max();
	for (int i = startTriangle; i < endTriangle; i++)
	{
		std::vector<Vector> points;
		points.push_back(scale * vertices[indices[i].vtxi] + center);
		points.push_back(scale * vertices[indices[i].vtxj] + center);
		points.push_back(scale * vertices[indices[i].vtxk] + center);
		for (int j = 0; j < 3; j++)
		{
			if (points[j][0] > BMax_x)
				BMax_x = points[j][0];
			if (points[j][1] > BMax_y)
				BMax_y = points[j][1];
			if (points[j][2] > BMax_z)
				BMax_z = points[j][2];
			if (points[j][0] < BMin_x)
				BMin_x = points[j][0];
			if (points[j][1] < BMin_y)
				BMin_y = points[j][1];
			if (points[j][2] < BMin_z)
				BMin_z = points[j][2];
		}
	}
	boundingBox.BMin = Vector(BMin_x, BMin_y, BMin_z);
	boundingBox.BMax = Vector(BMax_x, BMax_y, BMax_z);
	return boundingBox;
}

Vector TriangleMesh::computeBarycenter(TriangleIndices &index) const
{
	return scale * (vertices[index.vtxi] + vertices[index.vtxj] + vertices[index.vtxk]) / 3. + center;
}

void TriangleMesh::BVH(Node *node, int &startTriangle, int &endTriangle)
{ // Function based on code from page 46 of the lecture notes
	node->boundingBox = computeBoundingBox(startTriangle, endTriangle);
	node->startTriangle = startTriangle;
	node->endTriangle = endTriangle;
	Vector diag = node->boundingBox.BMax - node->boundingBox.BMin;
	Vector middleDiag = node->boundingBox.BMin + 0.5 * diag;
	int longestAxis = getLongest(diag);
	int pivotIndex = startTriangle;
	for (int i = startTriangle; i < endTriangle; i++)
	{
		Vector barycenter = computeBarycenter(indices[i]);
		if (barycenter[longestAxis] < middleDiag[longestAxis])
		{
			std::swap(indices[i], indices[pivotIndex]);
			pivotIndex++;
		}
	}
	if (pivotIndex == startTriangle || pivotIndex == endTriangle)
		pivotIndex = 0.5 * (startTriangle + endTriangle);

	if (startTriangle + 5 > endTriangle || pivotIndex <= startTriangle || pivotIndex + 1 >= endTriangle)
		return;

	node->leftChild = new Node();
	node->rightChild = new Node();
	BVH(node->leftChild, startTriangle, pivotIndex);
	BVH(node->rightChild, pivotIndex, endTriangle);
}

void TriangleMesh::readPNG(const char *obj) { texture = stbi_load(obj, &textureWidth, &textureHeight, stbiComp, 3); }

Vector TriangleMesh::getTextureColor(double alpha, double beta, double gamma, const TriangleIndices &indices) const
{ // Code based on page 47 from textbook
	Vector coord = alpha * uvs[indices.uvi] + beta * uvs[indices.uvj] + gamma * uvs[indices.uvk];
	coord[0] -= std::floor(coord[0]);
	coord[1] -= std::floor(coord[1]);

	const int x = clamp(int(textureWidth * coord[0]), 0, textureWidth - 1);
	const int y = clamp(int(textureHeight * (1. - coord[1])), 0, textureHeight - 1);
	const unsigned char *data = texture + 3 * (textureWidth * y + x);

	// Remove gamma correction
	Vector color;
	color[0] = std::pow(data[0] / 255., 2.2);
	color[1] = std::pow(data[1] / 255., 2.2);
	color[2] = std::pow(data[2] / 255., 2.2);
	return color;
}