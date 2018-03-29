#include "Sphere.h"
#include "Cube.h"
#include "Angel.h"
#include "Light.h"
#include "Drawable.h"

GLuint vColor_loc;
Sphere::Sphere() {
	modelmatrix = Translate(0, 0, -4);
	program = InitShader("vshaderLighting2_v150.glsl", "fshaderLighting2_v150.glsl");
	//program = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl");
	build();
}

Sphere::~Sphere() {

}

void Sphere::build() {
	// Create and initialize the buffers
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	sqrt2 = (float)sqrt(2.0);
	sqrt6 = (float)sqrt(6.0);

	index = 0;

	tetrahedron(4);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexLocations) + sizeof(vertexNormals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexLocations), vertexLocations);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations), sizeof(vertexNormals), vertexNormals);

	GLuint vPosition_loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition_loc);
	glVertexAttribPointer(vPosition_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal_loc = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal_loc);
	glVertexAttribPointer(vNormal_loc, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations)));

	/*GLuint vColor_loc = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor_loc);
	glVertexAttribPointer(vColor_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations)));*/

	glEnable(GL_DEPTH_TEST);
}

void Sphere::tetrahedron(int count) {

	vec4 v[4] = {
		vec4(0,0,1,1),
		vec4(0,2 * sqrt2 / 3,-1.0f/3.0f,1),
		vec4(-sqrt6 / 3.0f, -sqrt2 / 3.0f, -1.0f / 3.0f, 1.0f),
		vec4(sqrt6 / 3.0f, -sqrt2 / 3.0f, - 1.0f / 3.0f, 1.0f)
	};
	
	divideTriangle(v[0], v[1], v[2], count);
	divideTriangle(v[3], v[2], v[1], count);
	divideTriangle(v[0], v[3], v[1], count);
	divideTriangle(v[0], v[2], v[3], count);
}

vec4 Sphere::unit(vec4 p) {
	float len = p.x*p.x + p.y*p.y + p.z*p.z;
	vec4 t;
	if (len > DivideByZeroTolerance) {
		t = p / sqrt(len);
		t.w = 1.0;
	}
	return t;
}

void Sphere::divideTriangle(vec4 a, vec4 b, vec4 c, int count) {
	if (count > 0) {
		vec4 v1 = unit(a + b);
		v1.w = 1.0;
		vec4 v2 = unit(a + c);
		v2.w = 1.0;
		vec4 v3 = unit(b + c);
		v3.w = 1.0;

		divideTriangle(a, v1, v2, count - 1);
		divideTriangle(c, v2, v3, count - 1);
		divideTriangle(b, v3, v1, count - 1);
		divideTriangle(v1, v3, v2, count - 1);
	}
	else {
		triangle(a, b, c);
	}
}

void Sphere::triangle(vec4 a, vec4 b, vec4 c) {
	vec3 af = vec3(a.x, a.y, a.z);
	vec3 bf = vec3(b.x, b.y, b.z);
	vec3 cf = vec3(c.x, c.y, c.z);
	vec3 N = normalize(cross(bf - af, cf - af));


	vec4 color(1.0*rand() / RAND_MAX, 1.0*rand() / RAND_MAX, 1.0*rand() / RAND_MAX, 1.0);
	vertexLocations[index] = a;
	/*
	N.x = vertexLocations[index].x;
	N.y = vertexLocations[index].y;
	N.z = vertexLocations[index].z;
	N = normalize(N
	*/
	vertexNormals[index] = N;
	vertexColors[index] = color;
	index++;

	vertexLocations[index] = b;
	/*
	N.x = vertexLocations[index].x;
	N.y = vertexLocations[index].y;
	N.z = vertexLocations[index].z;
	N = normalize(N);
	*/
	vertexNormals[index] = N;
	vertexColors[index] = color;
	index++;

	vertexLocations[index] = c;
	/*
	N.x = vertexLocations[index].x;
	N.y = vertexLocations[index].y;
	N.z = vertexLocations[index].z;
	N = normalize(N);
	*/
	vertexNormals[index] = N;
	vertexColors[index] = color;
	index++;
}

void Sphere::draw(Camera* c, vector<Light> lights) {
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
	//GLuint light_loc = glGetUniformLocation(program, "lightPos");
	//glUniform4fv(light_loc, 1, lights[0].getPosition());
	ambient_loc = glGetUniformLocation(program, "lightAmbient");
	glUniform4fv(ambient_loc, 1, lights[0].getAmbient());
	diffuse_loc = glGetUniformLocation(program, "lightDiffuse");
	glUniform4fv(diffuse_loc, 1, lights[0].getDiffuse());
	specular_loc = glGetUniformLocation(program, "lightSpecular");
	glUniform4fv(specular_loc, 1, lights[0].getSpecular());

	//set the materials
	GLuint ambient_loc2 = glGetUniformLocation(program, "matAmbient2");
	glUniform4fv(ambient_loc2, 1, matAmbient2);
	GLuint diffuse_loc2 = glGetUniformLocation(program, "matDiffuse2");
	glUniform4fv(diffuse_loc2, 1, matDiffuse2);
	GLuint specular_loc2 = glGetUniformLocation(program, "matSpecular2");
	glUniform4fv(specular_loc2, 1, matSpecular2);
	GLuint alpha_loc2 = glGetUniformLocation(program, "matAlpha2");
	glUniform1f(alpha_loc2, shininess);

	ambient_loc2 = glGetUniformLocation(program, "lightAmbient2");
	glUniform4fv(ambient_loc2, 1, lights[1].getAmbient());
	diffuse_loc2 = glGetUniformLocation(program, "lightDiffuse2");
	glUniform4fv(diffuse_loc2, 1, lights[1].getDiffuse());
	specular_loc2 = glGetUniformLocation(program, "lightSpecular2");
	glUniform4fv(specular_loc2, 1, lights[1].getSpecular());

	GLuint enabled_loc = glGetUniformLocation(program, "enabled");
	glUniform1i(enabled_loc, lights[0].getEnabled());
	GLuint enabled_loc2 = glGetUniformLocation(program, "enabled2");
	glUniform1i(enabled_loc2, lights[1].getEnabled());
	


	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

int Sphere::testColision(vec4 ray, Camera* c) {
	vec4 v1, v2, v3;
	int closestT = -1;
	int ind = -1;
	bool inside;
	for (int i = 0; i < numVertices; i+=3) {
		inside = true;

		//take the coordinates of the triangle and apply model matrix
		v1 = modelmatrix*vertexLocations[i];
		v2 = modelmatrix*vertexLocations[i + 1];
		v3 = modelmatrix*vertexLocations[i + 2];
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

		vec4 x = cross(v2-v1, (P - v1));
		float doot = dot(x, normal);
		if (doot < 0) {
			inside = false;
		}
		x = cross(v3-v2,(P - v2));//, P);
		doot = dot(x, normal);
		if (doot < 0) {
			inside = false;
		}
		x = cross(v1-v3, (P - v3));// , P);
		doot = dot(x, normal);
		if (doot < 0) {
			inside = false;
		}

		if (inside) {
			if (time < closestT || closestT==-1) {
				closestT = time;
				ind = i;
			}
		}
	}
	 
	return ind;
}

void Sphere::blackenTri(int index) {
	std::cout << index << '\n';
	vec4 black = vec4(0, 0, 0, 1);
	vertexColors[index] = black;
	vertexColors[index + 1] = black;
	vertexColors[index + 2] = black;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexLocations), sizeof(vertexColors), vertexColors);
	//GLuint vColor_loc = glGetAttribLocation(program, "vColor");
	//glEnableVertexAttribArray(vColor_loc);
	//glVertexAttribPointer(vColor_loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertexLocations)));
}

void Sphere::setMaterial(vec4 d, vec4 a, vec4 s, float sh) {
	matDiffuse = d;
	matAmbient = a;
	matSpecular = s;
	shininess = sh;
}

