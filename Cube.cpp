#include "Cube.h"
#include "Angel.h"
#include "Drawable.h"

Cube::Cube( GLuint p) {
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	currentCubeIndex = 0;
	modelmatrix = Translate(2, 0, -4);
	vec4 v[] = { vec4(-0.4,-0.4,0.4,1.0),vec4(-0.4,0.4,0.4,1.0),vec4(0.4,0.4,0.4,1.0),vec4(0.4,-0.4,0.4,1.0), vec4(-0.4,-0.4,-0.4,1.0),vec4(-0.4,0.4,-0.4,1.0),vec4(0.4,0.4,-0.4,1.0),vec4(0.4,-0.4,-0.4,1.0) };
	vec4 c[] = { vec4(0,0,0,1), vec4(1,0,0,1), vec4(0,1,0,1), vec4(0,0,1,1), vec4(1,1,0,1), vec4(1,0,1,1),vec4(0,1,1,1),vec4(0.5f,0.2f,0.4f,1) };
	setVertices(v);
	setColors(c);
	buildCube();
	program = p;//InitShader("vshader01_v150.glsl", "fshader01_v150.glsl");
	build();
}

void Cube::makeQuad(int a, int b, int c, int d) {

	vec3 af = vec3(vertices[a].x, vertices[a].y, vertices[a].z);
	vec3 bf = vec3(vertices[b].x, vertices[b].y, vertices[b].z);
	vec3 cf = vec3(vertices[c].x, vertices[c].y, vertices[c].z);
	vec4 N = normalize(cross(bf - af, cf - af));
	N.w=0;
	//Triangle #1
	
	cubeVertexLocations[currentCubeIndex] = vertices[a];
	cubeVertexColors[currentCubeIndex] = colors[currentCubeIndex % 8];
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[b];
	cubeVertexColors[currentCubeIndex] = cubeVertexColors[currentCubeIndex - 1];
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[c];
	cubeVertexColors[currentCubeIndex] = cubeVertexColors[currentCubeIndex - 1];
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	//Triangle #2
	cubeVertexLocations[currentCubeIndex] = vertices[c];
	cubeVertexColors[currentCubeIndex] = cubeVertexColors[currentCubeIndex - 1];
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[d];
	cubeVertexColors[currentCubeIndex] = cubeVertexColors[currentCubeIndex - 1];
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[a];
	cubeVertexColors[currentCubeIndex] = cubeVertexColors[currentCubeIndex - 1];
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;
}


void Cube::buildCube() {
	makeQuad(1, 0, 3, 2);  //front
	makeQuad(2, 3, 7, 6);  //right
	makeQuad(3, 0, 4, 7);  //bottom
	makeQuad(6, 5, 1, 2);  //top
	makeQuad(4, 5, 6, 7);  //back
	makeQuad(5, 4, 0, 1);  //left
}

void Cube::draw(Camera* c, vector<Light> lights) {
	glUseProgram(program);
	glBindVertexArray(VAO);


	GLuint modelMatrix_loc = glGetUniformLocation(program, "model_matrix");

	glUniformMatrix4fv(modelMatrix_loc, 1, GL_TRUE, modelmatrix);

	GLuint viewMatrix_loc = glGetUniformLocation(program, "view_matrix");
	glUniformMatrix4fv(viewMatrix_loc, 1, GL_TRUE, c->viewMatrix);

	GLuint projMatrix_loc = glGetUniformLocation(program, "proj_matrix");
	glUniformMatrix4fv(projMatrix_loc, 1, GL_TRUE, c->proMatrix);

	//set the materials
	GLuint ambient_loc = glGetUniformLocation(program, "matAmbient");
	glUniform4fv(ambient_loc, 1, matAmbient);
	GLuint diffuse_loc = glGetUniformLocation(program, "matDiffuse");
	glUniform4fv(diffuse_loc, 1, matDiffuse);
	GLuint specular_loc = glGetUniformLocation(program, "matSpecular");
	glUniform4fv(specular_loc, 1, matSpecular);
	GLuint alpha_loc = glGetUniformLocation(program, "matAlpha");
	glUniform1f(alpha_loc, shininess);


	//set up the lighting 
	GLuint light_loc = glGetUniformLocation(program, "lightPos");
	glUniform4fv(light_loc, 1, lights[0].getPosition());
	ambient_loc = glGetUniformLocation(program, "lightAmbient");
	glUniform4fv(ambient_loc, 1, lights[0].getAmbient());
	diffuse_loc = glGetUniformLocation(program, "lightDiffuse");
	glUniform4fv(diffuse_loc, 1, lights[0].getDiffuse());
	specular_loc = glGetUniformLocation(program, "lightSpecular");
	glUniform4fv(specular_loc, 1, lights[0].getSpecular());


	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void Cube::draw(Camera* c, GLuint p) {
	glUseProgram(program);
	glBindVertexArray(VAO);


	GLuint modelMatrix_loc = glGetUniformLocation(program, "model_matrix");

	glUniformMatrix4fv(modelMatrix_loc, 1, GL_TRUE, modelmatrix);

	GLuint viewMatrix_loc = glGetUniformLocation(program, "view_matrix");
	glUniformMatrix4fv(viewMatrix_loc, 1, GL_TRUE, c->viewMatrix);

	GLuint projMatrix_loc = glGetUniformLocation(program, "projection_matrix");
	glUniformMatrix4fv(projMatrix_loc, 1, GL_TRUE, c->proMatrix);






	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

int Cube::testColision(vec4 v, Camera* c) {
	return 1;   
}

void Cube::build() {
	//glGenBuffers(1, &VBO);
	//glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexLocations) + sizeof(vertexNormals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertexLocations), cubeVertexLocations);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertexLocations), sizeof(vertexNormals), vertexNormals);

	GLuint vPosition_loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition_loc);
	glVertexAttribPointer(vPosition_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal_loc = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal_loc);
	glVertexAttribPointer(vNormal_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(cubeVertexLocations)));

	/*GLuint vColor_loc = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor_loc);
	glVertexAttribPointer(vColor_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(cubeVertexLocations)));*/

	glEnable(GL_DEPTH_TEST);
}

void Cube::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
	matDiffuse = d;
	matAmbient = a;
	matSpecular = s;
	shininess = sh;
}