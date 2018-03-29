#include  "Angel.h"
#include "Camera.h"
#include "Sphere.h"
#include "Shape.h"
#include "Cube.h"
#include "Plane.h"
#include "Door.h"
#include "Key.h"
#include <vector>

Sphere *s;
//vector<Camera> cams;
int camIndex = 0;
Camera *curr;
Camera *c;
Camera *c2;
Cube *cube;
Door *d;
Door *d2;
Door *d3;
Key *k;
Key *k2;
Key *k3;
Plane *pl;
vector<Plane*> walls;
vector<Light> lights;
bool have1, have2, have3;
int found1, found2, found3;
int foundIndex;
//Shape *shape;


//declarations of functions
void init();
void display();
void keyboard(unsigned char, int, int);
void SpecialInput(int key, int x, int y);
void timer(int value);
void onClick(int button, int state, int x, int y);
void swapView();
mat4 mat4Inverse(mat4 m);
void close();
void makeMaze();
void opend1(int value);
void opend2(int value);
void opend3(int value);


const int height = 500;
const int width = 500;

int main(int argc, char **argv) {
	have1 = false;
	have2 = false;
	have3 = false;
	foundIndex = 1;
	glutInit(&argc, argv);	//initialize glut
#ifdef __APPLE__
	glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
#endif
	glutInitWindowSize(width, height);	

	glutCreateWindow("Sphero World");  

									  //initialize glew if necessary (don't need to on Macs)
#ifndef __APPLE__
	GLenum err = glewInit();
#endif

	init();  //do some initialize for our program

	//set up the callback functions
	glutDisplayFunc(display);  //REQUIRED.  What to do when it's time to draw
	glutKeyboardFunc(keyboard);  //What to do if a keyboard event is detected
	glutSpecialFunc(SpecialInput); //special input stuff
	glutMouseFunc(onClick);
	glutWMCloseFunc(close);
	glutMainLoop();  //start infinite loop, listening for events
	return 0;
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glDisable(GL_CULL_FACE);
	s = new Sphere();
	//cams.push_back(Camera());
	c2 = new Camera(-20.5, 52.402, -18.3363, vec4(0, -1, 0, 0), true, vec4(1, 0, 0, 0));
	c = new Camera();
	cube = new Cube(s->program);
	pl = new Plane(s->program);
	d = new Door(vec3(-5.01, -0.6, -8), 1);
	d2 = new Door(vec3(-31, -0.6, -16), 1);
	d3 = new Door(vec3(-20, -0.6, -23), 3);
	k = new Key(vec3(-10, -1, -28),1);
	k3 = new Key(vec3(-44, -1, -24), 3);
	k2 = new Key(vec3(-18, -1, -28), 2);
	makeMaze();
	
	//pl = new Plane(20,vec3(-1,0,-2),true);

	s->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	cube->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	pl->setMaterial(vec4(0.0f, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	pl->setTexture("grass.ppm");
	d->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	d2->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	d3->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	k->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	k2->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	k3->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	//pl->setMaterial(vec4(1, 0.8f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
	curr = c;
	lights.push_back(Light(curr->eye, vec4(0, 0, 0, 0), vec4(0.0f, 0.0f, 0.0f, 1), vec4(1, 1, 1, 1)));
	lights[0].turnOn();
	lights.push_back(Light(vec4(0,-1000000,0,0), vec4(0, 0, 0, 0), vec4(0.0f, 0.0f, 0.0f, 1), vec4(1, 1, 1, 1)));
	lights[1].turnOn();

//	lights.push_back(Light(curr->eye,))
	//shape = new Shape();
	
}

void display( void ) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  //clear out the color of the framebuffer and the depth info from the depth buffer
	glDisable(GL_CULL_FACE);
	//Camera *curr = cams[camIndex];
	//s->draw(curr, lights);
	//cube->draw(curr, lights);
	pl->draw(curr, lights);
	for (int i = 0; i < walls.size(); i++) {
		walls[i]->setMaterial(vec4(1, 0.0f, 0, 1), vec4(1, 0, 1, 1), vec4(1, 0.8f, 0, 1), 100);
		walls[i]->draw(curr, lights);
	}
	d->draw(curr, lights);
	d2->draw(curr, lights);
	d3->draw(curr, lights);
	if(have1 == false)
		k->draw(curr, lights);
	if (have2 == false)
		k2->draw(curr, lights);
	if (have3 == false)
		k3->draw(curr, lights);

	glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	
	if (curr->moveable == false) {
		switch (key) {
		case ' ':
			swapView();
			break;
		}
		return;
	}
	else {
		switch (key) {
		case 033:  // Escape key
		case 'q': case 'Q':
			exit(EXIT_SUCCESS);
			break;
		case 'c':
			curr->n = (cos(0.1)*curr->n) - (sin(0.1)*curr->u);
			curr->u = (sin(0.1)*curr->n) + (cos(0.1)*curr->u);
			break;
		case 'C':
			curr->n = (cos(-0.1)*curr->n) - (sin(-0.1)*curr->u);
			curr->u = (sin(-0.1)*curr->n) + (cos(-0.1)*curr->u);
			break;
		case ' ':
			swapView();
			std::cout << c->eye.x<<endl;
			std::cout << c->eye.y<<endl;
			std::cout << c->eye.z<<endl;
			//glutTimerFunc(100, opend1, 0);
			//lights[0].onOff();
			break;
		}
		curr->remakeView();
		glutPostRedisplay();
	}
}

void close() {
	s->deleteBuff();
}

void SpecialInput(int key, int x, int y)
{
	if (curr->eye.x < -15 && curr->eye.x > -24 && curr->eye.z > -25 && curr->eye.z < -19) {
		walls[17]->setTexture("winner.ppm");
		glutPostRedisplay();
	}
	//mat4 vm;
	switch (key)
	{
	case GLUT_KEY_UP:
		//do something here
		curr->eye = curr->eye - 0.5*curr->n;
		curr->viewMatrix = curr->viewMatrix*Translate(0, 0, -0.1);//vec4(0,0,0.1,0);
								   //c->remakeView();

		break;
	case GLUT_KEY_DOWN:
		//do something here
		curr->eye = curr->eye + 0.5*curr->n;//vec4(0,0,0.1,0);
		curr->viewMatrix = curr->viewMatrix*Translate(0, 0, 0.1);
								   //c->remakeView();
		break;
	case GLUT_KEY_LEFT:
		//do something here
		curr->eye = curr->eye - 0.5*curr->u;//vec4(0.1,0,0,0);
		curr->viewMatrix = curr->viewMatrix*Translate(-0.1, 0, 0);
								   //c->rema
		break;
	case GLUT_KEY_RIGHT:
		//do something here
		curr->eye = curr->eye + 0.5*curr->u;//vec4(0.1,0,0,0);
		curr->viewMatrix = curr->viewMatrix*Translate(0.1, 0, 0);
		break;
	}
	lights[0].setPostition(curr->eye);
	curr->remakeView();
	glutPostRedisplay();
}

void onClick(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) {
		float wx = 2.0 * (1.0*x / width) - 1.0;
		float wy = 1.0 - (2.0 * y / height);
		float z = -1.0;
		vec4 pfront = vec4(wx, wy, z, 1.0);
		vec4 pcam = vec4(curr->r*pfront.x, curr->t*pfront.y, -curr->getNear(), 1);//c->invProMatrix*pfront*c->getNear();

		curr->computeInvView();
		int what = curr->invViewMatrix[0][0];

		vec4 pworld = curr->invViewMatrix*pcam;// c->getInvView()*pcam;

		vec4 ray = pworld - curr->eye;
		int od1 = -1;
		int od2 = -1;
		int od3 = -1;
		switch (foundIndex) {
			//getting the first key, doesnt move on until you have it
		case 1:
			found1 = k->testColision(ray, curr);
			if (found1 > -1) {
				foundIndex = foundIndex + 1;
				have1 = true;
			}
			break;
			//opening the first door, doesnt move on until you have it
		case 2:
			od1 = d->testColision(ray, curr);
			if (od1 > -1 && have1 == true) {
				foundIndex = foundIndex + 1;
				glutTimerFunc(100, opend1, 0);
			}
			break;
			//getting the second key
		case 3:
			found2 = k2->testColision(ray, curr);
			if (found2 > -1) {
				foundIndex = foundIndex + 1;
				have2 = true;
			}
			break;
			//opening the second door
		case 4:
			od2 = d2->testColision(ray, curr);
			if (od2 > -1 && have2 == true) {
				foundIndex = foundIndex + 1;
				glutTimerFunc(100, opend2, 0);
			}
			break;
			//getting the third key
		case 5:
			found3 = k3->testColision(ray, curr);
			if (found1 > -1) {
				foundIndex = foundIndex + 1;
				have3 = true;
			}
			break;
			//opening the third and final door
		case 6:
			od3 = d3->testColision(ray, curr);
			if (od3 > -1 && have3 == true) {
				foundIndex = foundIndex + 1;
				glutTimerFunc(100, opend3, 0);
			}
			break;
		}
		/*int found1 = k->testColision(ray, curr);
		if (found1 > -1) {
			od1 = d->testColision(ray, curr);
		}
		int found2 = k->testColision(ray, curr);
		int od2 = d->testColision(ray, curr);
		int found3 = k->testColision(ray, curr);
		int od3 = d->testColision(ray, curr);

		std::cout << found1 << endl;
		std::cout << od1 << endl;
		if (found1 > -1)
			have1 = true;

		if (found3 > -1)
			have3 = true;
		if (found2 > -1)
			have2 = true;

		if (od1 > -1 && have1 == true)
			glutTimerFunc(100, opend1, 0);*/
	}
	
}

void swapView(){
	switch (camIndex) {
	case 0:
		curr = c2;
		camIndex = 1;
		break;
	case 1:
		curr = c;
		camIndex = 0;
		break;
	}
	glutPostRedisplay();
}

void makeMaze() {
	//front and back original
	//walls.push_back(new Plane(20, vec3(-3, 0, -3), true));
	//walls.push_back(new Plane(27, vec3(4, 0, -20), true));
	walls.push_back(new Plane(8, vec3(2, 0, -4), false));
	walls.push_back(new Plane(8, vec3(1, 0, -12), true));
	walls.push_back(new Plane(4, vec3(-2, 0, -4), false));
	walls.push_back(new Plane(4, vec3(-8, 0, -8), true));
	walls.push_back(new Plane(4, vec3(6, 0, -3), true));//5
	walls.push_back(new Plane(8, vec3(1, 0, -16), true));
	walls.push_back(new Plane(8, vec3(2, 0, -17), false));
	walls.push_back(new Plane(4, vec3(-7, 0, -17), false));//8
	walls.push_back(new Plane(4, vec3(-3, 0, -21), true));
	walls.push_back(new Plane(8, vec3(-2, 0, -22), false));
	walls.push_back(new Plane(8, vec3(6, 0, -30), true));
	walls.push_back(new Plane(8, vec3(1, 0, -34), true));
	walls.push_back(new Plane(8, vec3(-7, 0, -26), false));
	walls.push_back(new Plane(4, vec3(-8, 0, -25), true));
	walls.push_back(new Plane(27, vec3(-12, 0, -9), false));//15
	walls.push_back(new Plane(32, vec3(7, 0, -4), false));
	walls.push_back(new Plane(12, vec3(-13, 0, -20), true));
	walls.push_back(new Plane(4, vec3(-25, 0, -21), false));
	walls.push_back(new Plane(8, vec3(-17, 0, -25), true));
	walls.push_back(new Plane(8, vec3(-16, 0, -26), false));
	walls.push_back(new Plane(16, vec3(-17, 0, -34), true));
	walls.push_back(new Plane(8, vec3(-33, 0, -26), false));//22
	walls.push_back(new Plane(4, vec3(-29, 0, -25), true));
	walls.push_back(new Plane(12, vec3(-28, 0, -13), false));
	walls.push_back(new Plane(8, vec3(-29, 0, -12), true));
	walls.push_back(new Plane(4, vec3(-37, 0, -13), false));
	walls.push_back(new Plane(4, vec3(-33, 0, -17), false));
	walls.push_back(new Plane(12, vec3(-34, 0, -21), true));
	walls.push_back(new Plane(24, vec3(-42, 0, -9), false));//29
	walls.push_back(new Plane(24, vec3(-18, 0, -8), true));
	walls.push_back(new Plane(8, vec3(-17, 0, -9), false));
	walls.push_back(new Plane(7, vec3(-18, 0, -17), true));
	walls.push_back(new Plane(4, vec3(-25, 0, -13), false));
	walls.push_back(new Plane(4, vec3(-21, 0, -12), true));//34
	walls.push_back(new Plane(4, vec3(-38, 0, -33), true));
	walls.push_back(new Plane(7, vec3(-37, 0, -26), false));
	walls.push_back(new Plane(32, vec3(-46, 0, -4), false));
	walls.push_back(new Plane(43, vec3(-3, 0, -3), true));
	walls.push_back(new Plane(52, vec3(6, 0, -36), true));
	//sides original
	//walls.push_back(new Plane(16, vec3(5, 0, -4), false));
	//walls.push_back(new Plane(16, vec3(-23, 0, -4), false));
}

void opend1(int value) {
	mat4 mm = d->getModelMatrix();
	int deg = -5;
	if (d->open == false) {
		d->shrink();
	}
	value = value + 1;
	if (value > 100) {
		d->open = true;
		return;
	}
	glutPostRedisplay();
	glutTimerFunc(100, opend1, value);
}

void opend3(int value) {
	mat4 mm = d3->getModelMatrix();
	int deg = -5;
	if (d3->open == false) {
		d3->shrink();
	}
	value = value + 1;
	if (value > 100) {
		d3->open = true;
		return;
	}
	glutPostRedisplay();
	glutTimerFunc(100, opend3, value);
}

void opend2(int value) {
	mat4 mm = d2->getModelMatrix();
	int deg = -5;
	if (d2->open == false) {
		d2->shrink();
	}
	value = value + 1;
	if (value > 100) {
		d2->open = true;
		return;
	}
	glutPostRedisplay();
	glutTimerFunc(100, opend2, value);
}