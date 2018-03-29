#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "Angel.h"
#include "Drawable.h"
#include "Camera.h"

class Shape: public Drawable{  //derived from drawable class


public:	

	//constructors
	Shape();
	
	//destructor
	~Shape();

	void build();
	void draw(Camera*);
	void draw(Camera*, GLuint);
	int testColision(vec4, Camera*);

private:
	vec4 vertexLocations[4];
			
	void buildTriangle();
};
#endif
