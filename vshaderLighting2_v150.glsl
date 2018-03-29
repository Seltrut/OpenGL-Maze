#version 150

in vec4 vPosition;	//vertex attribute for vertex position
in vec3 vNormal;

out vec3 fN;
out vec3 fE;
out vec3 fL;

out vec3 fN2;
out vec3 fE2;
out vec3 fL2;


uniform mat4 model_matrix;  //the model matrix to transpose vertices from object coord to world coord
uniform mat4 proj_matrix;
uniform mat4 view_matrix;

//lighting stuff...
uniform vec4 lightPos;
uniform vec4 lightPos2;

  
void main() 
{ 
	//light1
	//the vertex in camera coordinates
	vec3 pos = (view_matrix*model_matrix*vPosition).xyz;
	
	// the light in camera coordinates
	vec3 lightPosInCam = (view_matrix*lightPos).xyz;
	
	//normal in vamera coordinates
	fN = normalize(view_matrix*model_matrix*vec4(vNormal,0)).xyz;
	
	//the ray from the vertex towards the camera 
	fE = normalize(vec3(0,0,0)-pos);
	
	fL = normalize(lightPosInCam.xyz-pos);
	
	
	//light2
	//the vertex in camera coordinates
	vec3 pos2 = (view_matrix*model_matrix*vPosition).xyz;
	
	// the light in camera coordinates
	vec3 lightPosInCam2 = (view_matrix*lightPos).xyz;
	
	//normal in vamera coordinates
	fN2 = normalize(view_matrix*model_matrix*vec4(vNormal,0)).xyz;
	
	//the ray from the vertex towards the camera 
	fE2 = normalize(lightPos2.xyz-pos2);
	
	fL2 = normalize(lightPosInCam2.xyz-pos2);
	

  	gl_Position = proj_matrix*view_matrix*model_matrix*vPosition;  //assign the vertex position to the vPosition attribute multiplied by the model matrix

}