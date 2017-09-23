#include "Headers/Camera.h"

// Se inicializa la c�mara que tenga una posici�n en el origen.
Camera::Camera() {
  Position = glm::vec3(0.0, 0.0, 0.0);
}

/**
 * Setter de la matriz de perspectiva esta es construida del fiel of fiew, aspect ratio y planos near/far.
 * @param fov Fiel of fiew.
 * @param aspectRatio Aspect ratio.
 * @param nearPlane Near plane.
 * @param farPlane Far plnae.
 * @return
 */
glm::mat4 Camera::SetPerspective(float fov, float aspectRatio, float nearPlane,
                                 float farPlane) {

  ProjectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
  return ProjectionMatrix;
}

/**
 * Coloca la posici�n, rotaciones (yaw y pitch en radianes) de la c�mara.
 * @param positionX Posici�n en X.
 * @param positionY Posici�n en Y.
 * @param positionZ Posici�n en Z
 * @param yaw Angulo de rotaci�n horizontal de la c�mara
 * @param pitch Angulo de rotaci�n vertical de la c�mara
 */
void Camera::PositionCamera(float positionX, float positionY, float positionZ,
                            float yaw, float pitch) {
  Position = glm::vec3(positionX, positionY, positionZ);
  Yaw = yaw;
  Pitch = pitch;
}

/**
 * Obtiene la matriz de rotaci�n de la c�mara, a travez de las rotaciones yaw y pitch.
 * @return Matriz de rotaci�n de la c�mara.
 */
glm::mat4 Camera::GetRotationMatrix() {
  glm::mat4 rotationMatrix(1.0f);
  rotationMatrix = glm::rotate(rotationMatrix, Pitch, glm::vec3(1, 0, 0));
  rotationMatrix = glm::rotate(rotationMatrix, Yaw, glm::vec3(0, 1, 0));
  return rotationMatrix;
}

/**
 * Obtiene la actual matriz de vista de acuerdo a la posici�n de la c�mara y rotaci�n de esta.
 * @return Matriz de vista.
 */
glm::mat4 Camera::GetViewMatrix() {
  return GetRotationMatrix()
      * glm::inverse(glm::translate(glm::mat4(), Position));
}

/**
 * Metodo que obtiene el vector de vista de la matriz de rotaci�n.
 * @return
 */
glm::vec3 Camera::GetView() {
  return glm::vec3(0, 0, -1);
}

/**
 * M�todo que mueve la c�mara hacia atras, adelanate, izquierda, derecha, arriba y abajo
 * dependiendo de la velocidad de la c�mara y el vector de movimiento.
 * @param speed Velocidad de la c�mara.
 * @param viewVector Direcci�n de vista de la c�mara.
 */
void Camera::MoveCamera(float speed, glm::vec3 viewVector) {
  Position.x += viewVector.x * speed;
  Position.y += viewVector.y * speed;
  Position.z += viewVector.z * speed;
}

/**
 * Setter de los valor pitch an yaw de la c�mara y los deltas
 * para el siguiente frame.
 * @param mouseX Movimiento en X.
 * @param mouseY Movimiento en Y
 */
void Camera::SetViewByMouse(float xOffset, float yOffset) {
  Yaw += xOffset * MouseSpeed;
  Pitch += yOffset * MouseSpeed;
  if (Yaw > 2 * PI)
    Yaw = 0;
  if (Yaw < -2 * PI)
    Yaw = 2 * PI;
  if (Pitch > glm::radians(360.0f))
    Pitch = glm::radians(360.0f);
  if (Pitch < glm::radians(-360.0f))
    Pitch = glm::radians(-360.0f);
}
