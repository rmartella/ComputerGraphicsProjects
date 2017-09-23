#ifndef _Camera_H
#define _Camera_H

#define PI 3.14159265358979323846264338327950288
#define GLM_FORCE_RADIANS
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

/**
 * Clase que maneja los eventos del usuario, en nuestra escena 3D.
 */
class Camera {
 public:

  /**
   *Constructor defualt.
   */
  Camera();

  /**
   * Setter de la matriz de perspectiva esta es construida de el fiel of fiew, aspect ratio y planos near/far.
   * @param fov Fiel of fiew.
   * @param aspectRatio Aspect ratio.
   * @param nearPlane Near plane.
   * @param farPlane Far plnae.
   * @return
   */
  glm::mat4 SetPerspective(float fov, float aspectRatio, float nearPlane,
                           float farPlane);

  /**
   * Obtiene la matriz de proyecci�n.
   * @return Matriz de proyecci�n
   */
  glm::mat4 GetProjectionMatrix() {
    return ProjectionMatrix;
  }

  /**
   * Obtiene la actual matriz de vista de acuerdo a la posici�n de la c�mara y rotaci�n de esta.
   * @return Matriz de vista.
   */
  glm::mat4 GetViewMatrix();

  /**
   * Setter de la posici�n de la c�mara.
   * @param position
   */
  void SetPosition(glm::vec3 position) {
    Position = position;
  }
  /**
   * Obtiene la posici�n de la c�mara.
   * @return
   */
  glm::vec3 GetPosition() {
    return Position;
  }

  /**
   * M�todo que obtiene el vector de vista de la matriz de rotaci�n.
   * @return
   */
  glm::vec3 GetView();

  /**
   * Obtiene el angulo de rotaci�n horizontal de la c�mara.
   * @return Angulo de rotaci�n horizontal de la c�mara.
   */
  float GetYaw() {
    return Yaw;
  }
  /**
   * Obtiene el angulo de rotaci�n horizontal de la c�mara.
   * @param yaw Angulo de rotaci�n horizontal de la c�mara.
   */
  void SetYaw(float yaw) {
    Yaw = yaw;
  }
  /**
   * Obtiene el angulo de rotaci�n vertical de la c�mara.
   * @return Angulo de rotaci�n vertical de la c�mara.
   */
  float GetPitch() {
    return Pitch;
  }
  /**
   * Setter del angulo de rotaci�n vertical de la c�mara.
   * @param pitch Angulo de rotaci�n vertical de la c�mara.
   */
  void SetPitch(float pitch) {
    Pitch = pitch;
  }

  /**
   * Setter de la velocidad de los movimientos de la c�mara.
   * @param speed Velocidad de la c�mara.
   */
  void SetSpeed(double speed) {
    Speed = speed;
  }
  /**
   * Obtiene la velocidad de la c�mara.
   * @return Velocidad de la c�mara.
   */
  double GetSpeed() {
    return Speed;
  }

  /**
   * Coloca la posici�n, rotaciones (yaw y pitch en radianes) de la c�mara.
   * @param positionX Posici�n en X.
   * @param positionY Posici�n en Y.
   * @param positionZ Posici�n en Z
   * @param yaw Angulo de rotaci�n horizontal de la c�mara
   * @param pitch Angulo de rotaci�n vertical de la c�mara
   */
  void PositionCamera(float positionX, float positionY, float positionZ,
                      float yaw, float pitch);

  /**
   * Obtiene la matriz de rotaci�n de la c�mara, de las rotaciones yaw y pitch.
   * @return Matriz de rotaci�n de la c�mara.
   */
  glm::mat4 GetRotationMatrix();

  /**
   * M�todo que mueve la c�mara hacia atras o adelanate dependiendo de la velocidad de la c�mara.
   * @param speed Velocidad de la c�mara.
   * @param viewVector Direci�n de vista de la c�mara.
   */
  void MoveCamera(float speed, glm::vec3 viewVector);

  /**
   * Settea los valor de pitch an yaw a la c�mara y los deltas
   * para el siguiente frame.
   * @param mouseX Movimiento en X.
   * @param mouseY Movimiento en Y
   */
  void SetViewByMouse(float mouseX, float mouseY);

 protected:

  // Matriz de proyeccion.
  glm::mat4 ProjectionMatrix;
  // Matriz de posici�n.
  glm::vec3 Position;

  // Velocidad de la c�mara.
  float Speed = 1.0;
  // Velocidad del mouse.
  double MouseSpeed = 0.001;
  // Angulo de rotaci�n horizontal de la c�mara en radiandes eje y.
  float Yaw = 0;
  // Angulo de rotaci�n vertical de la c�mara en radiandes eje x.
  float Pitch = 0;
};

#endif
