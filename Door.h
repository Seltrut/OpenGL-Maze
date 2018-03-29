#ifndef __DOOR_H__
#define __DOOR_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"

class Door :public Drawable {
public:
	Door();
	Door(vec3, int);
	void draw(Camera*, vector<Light>);
	void draw(Camera* c, GLuint p);
	void setMaterial(vec4 d, vec4 a, vec4 s, float sh);
	void setModelMatrix(mat4 mm) {
		modelmatrix = mm;
	}
	mat4 getModelMatrix() {
		return modelmatrix;
	}
	int testColision(vec4, Camera*);
	void setTexture(char* s) {
		file = s;
	};
	void shrink();
	bool open;
	//void blackenTri(int);
	~Door();
private:
	static const int NumVertices = 36;
	int currentCubeIndex;
	vec4 cubeVertexLocations[NumVertices];
	vec4 vertexNormals[NumVertices];
	vec2 vertexTextureLocations[NumVertices];
	vec4 cubeVertexColors[NumVertices];
	vec4 vertices[8];
	char* file;
	GLuint texture;
	//bool o;
	int n;

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
	static void open1(int);
	void open2();
	void open3();

};
#endif