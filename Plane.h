#ifndef __PLANE_H__
#define __PLANE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"

class Plane :public Drawable {
public:
	//Plane();
	Plane(GLuint p);
	Plane(int, vec3, bool);
	void draw(Camera*, vector<Light>);
	void draw(Camera* c, GLuint p);
	void setMaterial(vec4 d, vec4 a, vec4 s, float sh);
	int testColision(vec4, Camera*);
	void setTexture(char* s);
	//void blackenTri(int);
	~Plane();
private:
	GLuint texture;
	static const int NumVertices = 6;
	int currentCubeIndex;
	vec4 cubeVertexLocations[NumVertices];
	vec3 vertexNormals[NumVertices];
	vec2 vertexTextureLocations[NumVertices];
	vec4 vertices[4];
	char* file;
	//vec4 vertices[8];

	vec4 colors[4];

	void setVertices(vec4 v[4]) {
		for (int i = 0; i < 4; i++) {
			vertices[i] = v[i];
		}
	}
	void setColors(vec4 c[4]) {
		for (int i = 0; i < 4; i++) {
			colors[i] = c[i];
		}
	}
	void makeQuad(int a, int b, int c, int d);
	void buildCube();
	void build();
	//light stuff
	vec4 matDiffuse, matSpecular, matAmbient;
	float shininess;
	vec4 matDiffuse2, matSpecular2, matAmbient2;
	float shininess2;

};
#endif