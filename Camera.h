#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Angel.h"

class Camera {

public:
	Camera();
	Camera(float, float, float, vec4, bool, vec4);
	~Camera();
	void perspectiveSwap();

	GLuint program;
	bool parallel;// = false;
	vec4 eye;
	vec4 u, v, n;
	vec4 at;
	//vec4 up;
	//vec4 v;
	mat4 proMatrix;
	mat4 invProMatrix;
	mat4 ortho;
	mat4 proj;
	mat4 viewMatrix;
	mat4 invViewMatrix;
	mat4 getInvView() { return invViewMatrix; }
	void setViewMatrix(mat4 vm){ viewMatrix = vm; }
	void setProMatrix(mat4 vm){proMatrix = vm;}
	mat4 getProMatrix(){ return proMatrix; }
	void remakeView();
	float getNear() { return nearPlane; }
	void computeInvPro();
	void computeInvView();
	bool moveable;
	float t;
	float r;

private:
	float fov;
	float ratio;
	float nearPlane;
	float farPlane;




	void build();
	void buildVm();



};

#endif