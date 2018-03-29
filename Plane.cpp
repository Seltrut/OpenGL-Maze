#include "Plane.h"
#include "Angel.h"
#include "Drawable.h"

Plane::Plane(GLuint p) {
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	currentCubeIndex = 0;
	modelmatrix = Translate(0, -1, -4)*RotateY(90)*Scale(100);
	vec4 v[] = { vec4(-1.0,0.0,1.0,1.0),vec4(4.0,0.0,1.0,1.0),vec4(4.0,0.0,-1.0,1.0),vec4(-1.0,0.0,-1.0,1.0)};
	setVertices(v);
	buildCube();
	setTexture("rock.ppm");
	program = InitShader("vshaderLighting_v150.glsl", "fshaderLighting_v150.glsl");
	build();
}

Plane::Plane(int scale, vec3 pos, bool rot) {
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	currentCubeIndex = 0;
	if(rot)
		modelmatrix = Translate(pos.x, pos.y, pos.z)*RotateZ(90)*RotateX(90);
	else
		modelmatrix = Translate(pos.x, pos.y, pos.z)*RotateZ(90);
	vec4 v[] = { vec4(-1.0,0.0,1.0,1.0),vec4(4.0,0.0,1.0,1.0),vec4(4.0,0.0,-1.0*scale,1.0),vec4(-1.0,0.0,-1.0*scale,1.0) };
	setVertices(v);
	buildCube();
	setTexture("rock.ppm");
	program = InitShader("vshaderLighting_v150.glsl", "fshaderLighting_v150.glsl");
	build();
}
Plane::~Plane() {

}
void Plane::setTexture(char* s) {
	file = s;
	build();
}

void Plane::makeQuad(int a, int b, int c, int d) {

	vec3 af = vec3(vertices[a].x, vertices[a].y, vertices[a].z);
	vec3 bf = vec3(vertices[b].x, vertices[b].y, vertices[b].z);
	vec3 cf = vec3(vertices[c].x, vertices[c].y, vertices[c].z);
	vec3 N = normalize(cross( cf - af, bf - af));
	//Triangle #1

	cubeVertexLocations[currentCubeIndex] = vertices[a];
	//cubeVertexColors[currentCubeIndex] = colors[currentCubeIndex % 8];
	vertexNormals[currentCubeIndex] = N;
	vertexTextureLocations[currentCubeIndex] = vec2(0, 0);
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[b];
	//cubeVertexColors[currentCubeIndex] = cubeVertexColors[currentCubeIndex - 1];
	vertexNormals[currentCubeIndex] = N;
	vertexTextureLocations[currentCubeIndex] = vec2(1, 0);
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[c];
	vertexTextureLocations[currentCubeIndex] = vec2(1, 1);
	//cubeVertexColors[currentCubeIndex] = cubeVertexColors[currentCubeIndex - 1];
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	//Triangle #2
	cubeVertexLocations[currentCubeIndex] = vertices[c];
	vertexTextureLocations[currentCubeIndex] = vec2(1, 1);
	//cubeVertexColors[currentCubeIndex] = cubeVertexColors[currentCubeIndex - 1];
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[d];
	vertexTextureLocations[currentCubeIndex] = vec2(0, 1);
	//cubeVertexColors[currentCubeIndex] = cubeVertexColors[currentCubeIndex - 1];
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[a];
	vertexTextureLocations[currentCubeIndex] = vec2(0, 0);
	//cubeVertexColors[currentCubeIndex] = cubeVertexColors[currentCubeIndex - 1];
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;
}


void Plane::buildCube() {
	makeQuad(1, 0, 3, 2);  //front vec4 v[] = { vec4(-1.0,0.0,1.0,1.0),vec4(1.0,0.0,1.0,1.0),vec4(-1.0,0.0,1.0,1.0),vec4(-1.0,0.0,-1.0,1.0)};
	//makeQuad(2, 3, 7, 6);  //right
	//makeQuad(3, 0, 4, 7);  //bottom
	//makeQuad(6, 5, 1, 2);  //top
	//makeQuad(4, 5, 6, 7);  //back
	//makeQuad(5, 4, 0, 1);  //left
}

void Plane::draw(Camera* c, vector<Light> lights) {

	glBindVertexArray(VAO);
	glUseProgram(program);

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


	

	//stuff from the example
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "textureID"), 0);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void Plane::draw(Camera* c, GLuint p) {
	glUseProgram(program);
	glBindVertexArray(VAO);


	GLuint modelMatrix_loc = glGetUniformLocation(program, "model_matrix");

	glUniformMatrix4fv(modelMatrix_loc, 1, GL_TRUE, modelmatrix);

	GLuint viewMatrix_loc = glGetUniformLocation(program, "view_matrix");
	glUniformMatrix4fv(viewMatrix_loc, 1, GL_TRUE, c->viewMatrix);

	GLuint projMatrix_loc = glGetUniformLocation(program, "proj_matrix");
	glUniformMatrix4fv(projMatrix_loc, 1, GL_TRUE, c->proMatrix);






	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

int Plane::testColision(vec4 v, Camera* c) {
	return 1;
}

void Plane::build() {
	//glGenBuffers(1, &VBO);
	//glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexLocations) + sizeof(vertexNormals) + sizeof(vertexTextureLocations), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertexLocations), cubeVertexLocations);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertexLocations), sizeof(vertexNormals), vertexNormals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertexLocations) + sizeof(vertexNormals), sizeof(vertexTextureLocations), vertexTextureLocations);


	GLuint vPosition_loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition_loc);
	glVertexAttribPointer(vPosition_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal_loc = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal_loc);
	glVertexAttribPointer(vNormal_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(cubeVertexLocations)));

	GLuint vTex_loc = glGetAttribLocation(program, "vTexture");
	glEnableVertexAttribArray(vTex_loc);
	glVertexAttribPointer(vTex_loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(cubeVertexLocations)+sizeof(vertexNormals)));

	//get the texture data for the quad
	glGenTextures(1, &texture);
	int imgsize = 256;
	GLubyte *data = ppmRead(file, &imgsize, &imgsize);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //move the data onto the GPU
	delete[] data;  //dont' need this data now that its on the GPU

					//set the texturing parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

void Plane::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
	matDiffuse = d;
	matAmbient = a;
	matSpecular = s;
	shininess = sh;
}