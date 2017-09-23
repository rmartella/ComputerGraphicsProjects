// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::vec4 ObjectPosition = glm::vec4(0, 0, 0, 0);
glm::vec3 LightPosition = glm::vec3(0, 0, 4);
//glm::vec3 LightDirection = glm::vec3(0, 0, 1);

int ObjectNormal = 1;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}
glm::vec4 getObjectPosition(){
	return ObjectPosition;
}
glm::vec3 getLightPosition(){
	return LightPosition;
}
//glm::vec3 getLightDirection(){
//	return LightDirection;
//}
int getObjectNormals() {
	return ObjectNormal;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f; //Si lo agrandas el tama�o del objeto con respecto a la verntana que lo contiene es mas peque�o

float speed = 1.0f; // 3 units / second
float mouseSpeed = 0.000005f;
float velocidad = 0.01f;



void computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
	verticalAngle += mouseSpeed * float(768 / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	//CAMERA
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	//OBJECT
	// Key_W -- UP
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		ObjectPosition.y += velocidad;
	}
	// Key_S -- DOWN
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		ObjectPosition.y -= velocidad;
	}
	// Key_A -- LEFT
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		ObjectPosition.x -= velocidad;
	}
	// Key_D -- RIGHT
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		ObjectPosition.x += velocidad;
	}
	// Key_Q -- BACK
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		ObjectPosition.z -= velocidad * 10;
	}
	// Key_E -- FRONT
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
		ObjectPosition.z += velocidad * 10;
	}

	//NORMALS
	//Key_N -- Normals
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
		if (ObjectNormal == 0){
			ObjectNormal = 1;
		}
		else{
			ObjectNormal = 0;
		}
	}

	//LIGHT_RED
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
		LightPosition.x -= velocidad * 10;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
		LightPosition.x += velocidad * 10;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
		LightPosition.y += velocidad * 10;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
		LightPosition.y -= velocidad * 10;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS){
		LightPosition.z -= velocidad * 10;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS){
		LightPosition.z += velocidad * 10;
	}

	//LIGHT_DIRECTION
	//if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
	//	LightDirection.x -= velocidad * 10;
	//}
	//if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS){
	//	LightDirection.x += velocidad * 10;
	//}
	//if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
	//	LightDirection.y += velocidad * 10;
	//}
	//if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS){
	//	LightDirection.y -= velocidad * 10;
	//}
	//if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
	//	LightDirection.z -= velocidad * 10;
	//}
	//if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS){
	//	LightDirection.z += velocidad * 10;
	//}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f); //FoV campo de visi�n: grados, relaci�n de 4:3, clipping: que tanto te puedes alejar o acercar
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}