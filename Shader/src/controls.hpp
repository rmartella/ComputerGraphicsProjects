#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::vec4 getObjectPosition();
glm::vec3 getLightPosition();
//glm::vec3 getLightDirection();
int getObjectNormals();

#endif