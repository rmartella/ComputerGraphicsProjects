// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <math.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"
#include "texture.hpp"
#include "controls.hpp"
#include "objloader.hpp"
#include "vboindexer.hpp"

float PI = 3.14159265358979323846;

int main(void) {
	// Initialise GLFW
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Tutorial - Shaders", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetCursorPos(window, 1024 / 2, 768 / 2);
	glfwSwapInterval(1);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("src/StandardShading.vertexshader.glsl",
			"src/StandardShading.fragmentshader.glsl");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Load the texture
	GLuint Texture = loadDDS("src/uvmap.DDS");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> normals_1;
	std::vector<int> vertexB;
	bool res = loadOBJ("src/suzanne.obj", vertices, uvs, normals);
	normals_1 = normals;
	for (unsigned int i = 0; i < normals_1.size(); i++) {
		normals_1[i].x = normals_1[i].x * -1.0f;
		normals_1[i].y = normals_1[i].y * -1.0f;
		normals_1[i].z = normals_1[i].z * -1.0f;
		if (i > normals_1.size() / 2)
			vertexB.push_back(0);
		else
			vertexB.push_back(1);
	}
	//Encontrar el m�ximo valor de vertices en z

	// Load it into a VBO

	GLuint vertexbuffer; //Array Buffer
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
			&vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0],
			GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
			&normals[0], GL_STATIC_DRAW);

	GLuint normalbuffer_1;
	glGenBuffers(1, &normalbuffer_1);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_1);
	glBufferData(GL_ARRAY_BUFFER, normals_1.size() * sizeof(glm::vec3),
			&normals_1[0], GL_STATIC_DRAW);

	GLuint vertexB_buffer;
	glGenBuffers(1, &vertexB_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexB_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertexB.size() * sizeof(int), &vertexB[0],
			GL_STATIC_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID,
			"LightPosition_worldspace");

	//LightPosition2
	glUseProgram(programID);
	GLuint LightID_2 = glGetUniformLocation(programID,
			"LightPosition_worldspace_2");

	glUseProgram(programID);
	GLuint LightID_3 = glGetUniformLocation(programID,
			"LightPosition_SpotDirection");

	//___________________________MOVIMIENTO___________________________________________________________________
	glUseProgram(programID);
	GLuint ObjectPositionID = glGetUniformLocation(programID, "MovePosition");
	//GLfloat ObjMoveFreq = 0.05,  ObjPosX = 0.0, ObjPosY = 0.0;

	// New variables
	glUseProgram(programID);
	GLfloat waveTime = 0.0, waveWidth = 0.1, waveHeight = 3.0, waveFreq = 0.1;

	GLint waveTimeLoc = glGetUniformLocation(programID, "waveTime");
	GLint waveWidthLoc = glGetUniformLocation(programID, "waveWidth");
	GLint waveHeightLoc = glGetUniformLocation(programID, "waveHeight");

	float RotateSpeed = 0.01;
	float RotateAngle = 0.0;
	float Radio = 5;
	float X, Z = 0;

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0); //La declara como matriz de identidad
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		//Projection - Lente (que ve la camara) -- View - Posici�n Camara -- Model - Como se modifica el modelo
		//glm::mat4 MVP = ProjectionMatrix * ModelMatrix;

		//printf("%f", &ModelMatrix[0][0]);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		//Point Light
		X = Radio * (cos(RotateAngle * PI));
		Z = Radio * (sin(RotateAngle * PI));
		glm::vec3 lightPos = glm::vec3(X, 0, Z);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
		RotateAngle += RotateSpeed;

		//SpotLight
		glm::vec3 lightPos_2 = glm::vec3(getLightPosition().x,
				getLightPosition().y, getLightPosition().z);
		glUniform3f(LightID_2, lightPos_2.x, lightPos_2.y, lightPos_2.z);
		glm::vec3 lightDir = glm::vec3(0, 0, 1);
		glUniform3f(LightID_3, lightDir.x, lightDir.y, lightDir.z);

		//------------New----------------
		/*ObjPosX += ObjMoveFreq;
		 ObjPosY += ObjMoveFreq;*/
		glm::vec4 ObjPos = glm::vec4(getObjectPosition().x,
				getObjectPosition().y, getObjectPosition().z, 0);
		glUniform4f(ObjectPositionID, ObjPos.x, ObjPos.y, ObjPos.z, ObjPos.w);

		// New variables
		waveTime += waveFreq;
		glUniform1f(waveTimeLoc, waveTime);
		glUniform1f(waveWidthLoc, waveWidth);
		glUniform1f(waveHeightLoc, waveHeight);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		//Movement

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0,             // attribute (es el atributo no. 0)
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride (saltos de memoria)
				(void*) 0            // array buffer offset
				);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1,                                // attribute
				2,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*) 0                          // array buffer offset
				);

		if (getObjectNormals()) {
			// 3rd attribute buffer : normals
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
			glVertexAttribPointer(2,                                // attribute
					3,                                // size
					GL_FLOAT,                         // type
					GL_FALSE,                         // normalized?
					0,                                // stride
					(void*) 0                          // array buffer offset
					);
		} else {
			// 3rd attribute buffer : normals
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, normalbuffer_1);
			glVertexAttribPointer(2,                                // attribute
					3,                                // size
					GL_FLOAT,                         // type
					GL_FALSE,                         // normalized?
					0,                                // stride
					(void*) 0                          // array buffer offset
					);
		}

		// 3rd attribute buffer : Bool
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, vertexB_buffer);
		glVertexAttribPointer(3,                                // attribute
				1,                                // size
				GL_INT,                           // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*) 0                          // array buffer offset
				);

		// Draw the triangles !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
			&& glfwWindowShouldClose(window) == 0);
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
