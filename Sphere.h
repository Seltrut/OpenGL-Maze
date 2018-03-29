#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"

class Sphere:public Drawable {
public:
	Sphere();
	
	void draw(Camera*, vector<Light>);
	int testColision(vec4, Camera*);
	void setMaterial(vec4 d, vec4 a, vec4 s, float sh);
	void blackenTri(int);
	~Sphere();
private:
	static const unsigned int numVertices = 3072;//12582912;//3072;

	void build();
	unsigned int index;
	GLuint vpos, cpos, mmpos, vmpos, pmpos;

	vec4 vertexLocations[numVertices];
	vec4 vertexColors[numVertices];
	vec3 vertexNormals[numVertices];
	float sqrt2, sqrt6;
	void tetrahedron(int);
	void divideTriangle(vec4, vec4, vec4, int);
	void triangle(vec4, vec4, vec4);
	vec4 unit(vec4);

	//light stuff
	vec4 matDiffuse, matSpecular, matAmbient;
	float shininess;

		//light stuff
	vec4 matDiffuse2, matSpecular2, matAmbient2;
	float shininess2;
};
#endif
