#include "Shape.h"
Shape::Shape() {
	modelmatrix = Translate(0,1,-4);
	//load the shader and get the location of it's variables
	program = InitShader("vshader01_v150.glsl", "fshader01_v150.glsl");
	build();
}



void Shape::build(){
		glGenBuffers(1,&VBO);
		glGenVertexArrays(1,&VAO);
		buildTriangle();  //build the triangle (set the vertex locations and normals)

		//move the data onto the buffer
		glBindVertexArray(VAO);
		glBindBuffer( GL_ARRAY_BUFFER, VBO);
		glBufferData( GL_ARRAY_BUFFER, sizeof(vertexLocations),NULL, GL_STATIC_DRAW );
		glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(vertexLocations),vertexLocations);

		//link the vertex attributes with the buffer
		GLuint vPosition = glGetAttribLocation( program, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		
}

//on destroy, delete the buffer (cleanup)
Shape::~Shape(){
	glDeleteBuffers(1,&VBO);
	glDeleteVertexArrays(1, &VAO);
}


void Shape::buildTriangle(){
	//set up the vertices
	vertexLocations[0] = vec4(-1,0,-1,1);
	vertexLocations[1] = vec4(1,0,-1,1);
	vertexLocations[2] = vec4(1,0,1,1);
	vertexLocations[3] = vec4(-1, 0, 1, 1);
}

void Shape::draw(Camera *c, GLuint p) {

	glBindVertexArray(VAO);
	glUseProgram(p);

	//set the transformation matrices
	GLuint model_loc = glGetUniformLocation(p, "model_matrix");
	glUniformMatrix3fv(model_loc, 1, GL_FALSE, modelmatrix);

	GLuint vColor = glGetUniformLocation(p, "vColor");
	glUniform4fv(vColor, 1, vec4(1, 1, 1, 1));


	GLuint viewMatrix_loc = glGetUniformLocation(p, "view_matrix");
	glUniformMatrix4fv(viewMatrix_loc, 1, GL_TRUE, c->viewMatrix);

	GLuint projMatrix_loc = glGetUniformLocation(p, "projection_matrix");
	glUniformMatrix4fv(projMatrix_loc, 1, GL_TRUE, c->proMatrix);

	//draw!
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

void Shape::draw(Camera *c){
	
	glBindVertexArray(VAO);
	glUseProgram(program);

	//set the transformation matrices
	GLuint model_loc = glGetUniformLocation(program,"model_matrix");
	glUniformMatrix3fv(model_loc,1,GL_FALSE,modelmatrix);

	GLuint vColor = glGetUniformLocation(program, "vColor");
	glUniform4fv(vColor, 1, vec4(1, 1, 0, 1));


	GLuint viewMatrix_loc = glGetUniformLocation(program, "view_matrix");
	glUniformMatrix4fv(viewMatrix_loc, 1, GL_TRUE, c->viewMatrix);

	GLuint projMatrix_loc = glGetUniformLocation(program, "projection_matrix");
	glUniformMatrix4fv(projMatrix_loc, 1, GL_TRUE, c->proMatrix);
	
	//draw!
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);

}

int Shape::testColision(vec4 x, Camera* c) {
	return -1;
}


