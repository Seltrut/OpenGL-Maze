#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__
#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include <vector>

using namespace std;

class Drawable {
protected:

	GLuint VAO;
	GLuint VBO;
	mat4 modelmatrix;
	vec4 color;
	vec4 origColor;
	int num;
//	float bVal = 1.0;
	
public:
	GLuint program;
	virtual void draw(Camera*, vector<Light>)=0;
	virtual int testColision(vec4, Camera*) = 0;
	void setModelMatrix(mat4 mm) { modelmatrix = mm; }
	void setColor(vec4 c) { color = c; }

	void reset() { 
		color = origColor; 
		//bVal = 1.0;
	}
	mat4 getModelMatrix() { return modelmatrix; }
	vec4 getColor() { return color; }
	void deleteBuff() { glDeleteBuffers(1, &VBO); }
	static unsigned char* ppmRead(char* filename, int* width, int* height);
	
};

#endif
