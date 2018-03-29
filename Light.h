#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "Angel.h"

class Light {
public:
	Light(vec4 p, vec4 a, vec4 s, vec4 d) : position(p), ambient(a), specular(s), diffuse(d), onDiff(d), offDiff(vec4(0,0,0,0)) {}	
	vec4 getPosition() { return position; }
	vec4 getAmbient() { return ambient; }
	vec4 getDiffuse() { return diffuse; }
	vec4 getSpecular() { return specular; }
	int getEnabled(){ return enabled; }
	void turnOn(){
		enabled = 1;
	}
	void turnOff(){
		enabled = 0;
	}
	void setPostition(vec4 pos) { 
 		position = pos; 
	}
	void setAmbient(vec4 am) { diffuse = am; }
	
	void onOff() {
		if (on) {
			diffuse = offDiff;
		}
		else {
			diffuse = onDiff;
		}
		on = !on;
	}
private:
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 onDiff;
	vec4 offDiff;
	vec4 specular;
	int enabled;
	bool on;
};


#endif
