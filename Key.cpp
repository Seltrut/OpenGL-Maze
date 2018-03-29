#include "Key.h"
#include "Angel.h"
#include "Drawable.h"

//void open1(int value);
Key::Key( vec3 pos, int num ) {
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	currentCubeIndex = 0;
	n = num;
	open = false;
	switch (n) {
	case 1:
		modelmatrix = Translate(pos.x, pos.y, pos.z);
		break;
	case 2:
		modelmatrix = Translate(pos.x, pos.y, pos.z)*RotateY(90);
		break;
	case 3:
		modelmatrix = Translate(pos.x, pos.y, pos.z);
		break;
	}
	
	vec4 v[] = { vec4(0,0,0,1.0), vec4(0,2.0,0,1.0), vec4(0.5,2.0,0,1.0), vec4(0.25,0.5,0,1.0), vec4(1,0.5,0,1.0), vec4(1,0,0,1.0)};
	setVertices(v);
	buildCube();
	setTexture("aluminum.ppm");
	program = InitShader("vshaderLighting_v150.glsl", "fshaderLighting_v150.glsl");
	build();
}

void Key::makeQuad(int a, int b, int c, int d) {

	vec3 af = vec3(vertices[a].x, vertices[a].y, vertices[a].z);
	vec3 bf = vec3(vertices[b].x, vertices[b].y, vertices[b].z);
	vec3 cf = vec3(vertices[c].x, vertices[c].y, vertices[c].z);
	vec4 N = normalize(cross( bf - af, cf - af ));
	N.w=0;

	//Triangle #1
	cubeVertexLocations[currentCubeIndex] = vertices[a];
	vertexNormals[currentCubeIndex] = N;
	vertexTextureLocations[currentCubeIndex] = vec2(0, 0);
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[b];
	vertexTextureLocations[currentCubeIndex] = vec2(1, 0);
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[c];
	vertexTextureLocations[currentCubeIndex] = vec2(1, 1);
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	//Triangle #2
	cubeVertexLocations[currentCubeIndex] = vertices[c];
	vertexTextureLocations[currentCubeIndex] = vec2(1, 1);
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[d];
	vertexTextureLocations[currentCubeIndex] = vec2(0, 1);
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;

	cubeVertexLocations[currentCubeIndex] = vertices[a];
	vertexTextureLocations[currentCubeIndex] = vec2(0, 0);
	vertexNormals[currentCubeIndex] = N;
	currentCubeIndex++;
}


void Key::buildCube() {
	makeQuad(0,1, 2, 3);  //front
	makeQuad(0, 3, 4, 5);
}
void Key::draw(Camera* c, vector<Light> lights) {
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

	//stuff from the example
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "textureID"), 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, NumVertices);
	//glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void Key::draw(Camera* c, GLuint p) {
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

int Key::testColision(vec4 ray, Camera* c) {
	vec4 v1, v2, v3;
	int closestT = -1;
	int ind = -1;
	bool inside;
	for (int i = 0; i < NumVertices; i += 3) {
		inside = true;

		//take the coordinates of the triangle and apply model matrix
		v1 = modelmatrix*cubeVertexLocations[i];
		v2 = modelmatrix*cubeVertexLocations[i + 1];
		v3 = modelmatrix*cubeVertexLocations[i + 2];
		//compute the normal of the triangle by taking the cross product of 2 of the triangles vectors     N = a x b
		vec4 normal = cross((v2 - v1), (v3 - v1));
		//force w value in resulting vector to be 0 because vector
		normal.w = 0;
		normal = normalize(normal);
		ray = normalize(ray);
		//compute t = (-N*EYE + E(point on the triangle)*N)/N*Direction
		float time = -(dot(normal, c->eye) + dot(-v1, normal)) / (dot(normal, ray));

		//easy checks to see if the ray intersects the plane
		if (time <= 1) {
			continue;
		}
		if (dot(normal, ray) == 0) {
			continue;
		}


		vec4 P = c->eye + time*ray;

		vec4 x = cross(v2 - v1, (P - v1));
		float doot = dot(x, normal);
		if (doot < 0) {
			inside = false;
		}
		x = cross(v3 - v2, (P - v2));//, P);
		doot = dot(x, normal);
		if (doot < 0) {
			inside = false;
		}
		x = cross(v1 - v3, (P - v3));// , P);
		doot = dot(x, normal);
		if (doot < 0) {
			inside = false;
		}

		if (inside) {
			if (time < closestT || closestT == -1) {
				closestT = time;
				ind = i;
			}
		}
	}

	return ind;
}

void Key::build() {
	//glGenBuffers(1, &VBO);
	//glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexLocations) + sizeof(vertexNormals)+sizeof(vertexTextureLocations), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertexLocations), cubeVertexLocations);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertexLocations), sizeof(vertexNormals), vertexNormals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertexLocations) + sizeof(vertexNormals), sizeof(vertexTextureLocations), vertexTextureLocations);

	GLuint vPosition_loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition_loc);
	glVertexAttribPointer(vPosition_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal_loc = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal_loc);
	glVertexAttribPointer(vNormal_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(cubeVertexLocations)));

	GLuint vTex_loc = glGetAttribLocation(program, "vTexture");
	glEnableVertexAttribArray(vTex_loc);
	glVertexAttribPointer(vTex_loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(cubeVertexLocations) + sizeof(vertexNormals)));

	glGenTextures(1, &texture);
	int imgsize = 128;
	int imgsize2 = 128;
	GLubyte *data = ppmRead(file, &imgsize, &imgsize2);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgsize, imgsize, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  //move the data onto the GPU
	delete[] data;  //dont' need this data now that its on the GPU

					//set the texturing parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Key::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
	matDiffuse = d;
	matAmbient = a;
	matSpecular = s;
	shininess = sh;
}


