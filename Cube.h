#ifndef __CUBE_H__
#define __CUBE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"

class Cube :public Drawable {
public:
	Cube();
	Cube(GLuint p);
	void draw(Camera*, vector<Light>);
	void draw(Camera* c, GLuint p);
	void setMaterial(vec4 d, vec4 a, vec4 s, float sh);
	int testColision(vec4, Camera*);
	//void blackenTri(int);
	~Cube();
private:
	static const int NumVertices = 36;
	int currentCubeIndex;
	vec4 cubeVertexLocations[NumVertices];
	vec4 vertexNormals[NumVertices];
	vec4 cubeVertexColors[NumVertices];
	vec4 vertices[8];
	//vec4 vertices[8];
	
	vec4 colors[8];

	void setVertices(vec4 v[8]) {
		for (int i = 0; i < 8; i++) {
			vertices[i] = v[i];
		}
	}
	void setColors(vec4 c[8]) {
		for (int i = 0; i < 8; i++) {
			colors[i] = c[i];
		}
	}
	void makeQuad(int a, int b, int c, int d);
	void buildCube();
	void build();
	//light stuff
	vec4 matDiffuse, matSpecular, matAmbient;
	float shininess;

};
#endif