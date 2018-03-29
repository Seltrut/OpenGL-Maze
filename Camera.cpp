#include "Angel.h"
#include "Camera.h"



Camera::Camera(){
	//program = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl");
	fov = 65.0;
	ratio = 1.0;
	nearPlane = 1.0;
	farPlane = 100.0;
	t = nearPlane*tan(fov / 2.0);
	r = t*ratio;

	ortho = Ortho(-1, 1, -1, 1, 1.0, 3.0);
	proj  = Perspective( 65, 1, 1, 100);
	computeInvPro();

	u = vec4(1,0,0,0);
	v = vec4(0,1,0,0);
	n = vec4(0,0,1,0);
	eye = vec4(0, 0, 0, 1.0);//vec4(-7,24.1965,8.6071,1.0);
	at = eye - n;
	viewMatrix = LookAt( eye, at, v ); 
	setProMatrix(proj);
	//build();
	parallel = false;
}

Camera::Camera(float x, float y, float z, vec4 a, bool m, vec4 up) {
	//program = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl");
	//ortho = Ortho(-1, 1, -1, 1, 1.0, 3.0);
	proj = Perspective(65, 1, 1, 100);
	computeInvPro();
	moveable = m;

	at = a;
	u = vec4(1, 0, 0, 0);
	v = vec4(0, 0, 1, 0);// up;
	n = vec4(0, 1, 0, 0);
	at = eye - n;
	eye = vec4(x, y, z, 1.0);
	viewMatrix = LookAt(eye, at, v);
	setProMatrix(proj);
	//build();
	parallel = false;
}

Camera::~Camera() {

}

void Camera::perspectiveSwap(){
	if( parallel == false ){
		setProMatrix( ortho );
	}else{
		setProMatrix( proj );
	}
	//build();
	parallel = !parallel;
}

void Camera::remakeView(){
	at = eye - n;
	viewMatrix = LookAt( eye, at, v );
}

void Camera::computeInvPro() {
	invProMatrix = mat4(
		vec4(r / nearPlane, 0, 0, 0),
		vec4(0, t / nearPlane, 0, 0),
		vec4(0, 0, 0, -1.0),
		vec4(0, 0, (nearPlane - farPlane) / (2.0 * nearPlane*farPlane), (nearPlane + farPlane) / (2.0 * nearPlane*farPlane))
	);
}

void Camera::computeInvView() {
	
	invViewMatrix = mat4(
		vec4(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0], eye.x),
		vec4(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], eye.y),
		vec4(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2], eye.z),
		vec4(0, 0, 0, 1)
	);
}

