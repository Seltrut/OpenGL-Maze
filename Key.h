#ifndef __KEY_H__
#define __KEY_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"

class Key :public Drawable {
public:
	Key();
	Key(vec3, int);
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
	~Key();
private:
	static const int NumVertices = 12;
	int currentCubeIndex;
	vec4 cubeVertexLocations[NumVertices];
	vec4 vertexNormals[NumVertices];
	vec2 vertexTextureLocations[NumVertices];
	vec4 cubeVertexColors[NumVertices];
	vec4 vertices[12];
	char* file;
	GLuint texture;
	//bool o;
	int n;

	//vec4 vertices[8];
	
	vec4 colors[8];

	void setVertices(vec4 v[12]) {
		for (int i = 0; i < 12; i++) {
			vertices[i] = v[i];
		}
	}
	void setColors(vec4 c[12]) {
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