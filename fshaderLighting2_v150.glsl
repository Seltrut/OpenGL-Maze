#version 150
//in vec4 color;	//get the interpolated color from the vertex shader

in vec3 fN;
in vec3 fL;
in vec3 fE;

in vec3 fN2;
in vec3 fL2;
in vec3 fE2;


uniform vec4 lightAmbient, lightDiffuse, lightSpecular;
uniform vec4 matAmbient, matDiffuse, matSpecular;
uniform float matAlpha;

uniform vec4 lightAmbient2, lightDiffuse2, lightSpecular2;
uniform vec4 matAmbient2, matDiffuse2, matSpecular2;
uniform float matAlpha2;

uniform int enabled;
uniform int enabled2;

out vec4 fColor;


void main() 
{ 
	vec4 color1 = vec4(0, 0, 0, 0);
	vec4 color2 = vec4(0, 0, 0, 0);
	
	//computations for the first light
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);
	
	vec3 H = normalize(L+E);
	
	vec4 ambient = lightAmbient*matAmbient;
	
	float Kd = max(dot(L, N),0.0);
	vec4 diffuse = Kd*lightDiffuse*matDiffuse;
	
	float Ks = pow(max(dot(N, H),0.0),matAlpha);
	vec4 spec = Ks*lightSpecular*matSpecular;
	
	if(dot(L,N)< 0.0)
		spec = vec4(0,0,0,1);
		
	//computations for the second light
	vec3 N2 = normalize(fN2);
	vec3 E2 = normalize(fE2);
	vec3 L2 = normalize(fL2);
	
	vec3 H2 = normalize(L2+E2);
	
	vec4 ambient2 = lightAmbient2*matAmbient2;
	
	float Kd2 = max(dot(L2, N2),0.0);
	vec4 diffuse2 = Kd2*lightDiffuse2*matDiffuse2;
	
	float Ks2 = pow(max(dot(N2, H2),0.0),matAlpha2);
	vec4 spec2 = Ks2*lightSpecular2*matSpecular2;
	
	if(dot(L2,N2)< 0.0)
		spec2 = vec4(0,0,0,1);
		
	if( enabled == 1 )
		color1 = ambient + diffuse + spec;
	
	if( enabled2 == 1)
		color2 = ambient2 + diffuse2 + spec2;
		
	fColor = color1 + color2;
	fColor.a = 1.0;
    //fColor = color;  //apply the uniform color to the fragment
} 

