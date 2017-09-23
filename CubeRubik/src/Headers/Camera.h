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
   * Obtiene la matriz de proyección.
   * @return Matriz de proyección
   */
  glm::mat4 GetProjectionMatrix() {
    return ProjectionMatrix;
  }

  /**
   * Obtiene la actual matriz de vista de acuerdo a la posición de la cámara y rotación de esta.
   * @return Matriz de vista.
   */
  glm::mat4 GetViewMatrix();

  /**
   * Setter de la posición de la cámara.
   * @param position
   */
  void SetPosition(glm::vec3 position) {
    Position = position;
  }
  /**
   * Obtiene la posición de la cámara.
   * @return
   */
  glm::vec3 GetPosition() {
    return Position;
  }

  /**
   * Método que obtiene el vector de vista de la matriz de rotación.
   * @return
   */
  glm::vec3 GetView();

  /**
   * Obtiene el angulo de rotación horizontal de la cámara.
   * @return Angulo de rotación horizontal de la cámara.
   */
  float GetYaw() {
    return Yaw;
  }
  /**
   * Obtiene el angulo de rotación horizontal de la cámara.
   * @param yaw Angulo de rotación horizontal de la cámara.
   */
  void SetYaw(float yaw) {
    Yaw = yaw;
  }
  /**
   * Obtiene el angulo de rotación vertical de la cámara.
   * @return Angulo de rotación vertical de la cámara.
   */
  float GetPitch() {
    return Pitch;
  }
  /**
   * Setter del angulo de rotación vertical de la cámara.
   * @param pitch Angulo de rotación vertical de la cámara.
   */
  void SetPitch(float pitch) {
    Pitch = pitch;
  }

  /**
   * Setter de la velocidad de los movimientos de la cámara.
   * @param speed Velocidad de la cámara.
   */
  void SetSpeed(double speed) {
    Speed = speed;
  }
  /**
   * Obtiene la velocidad de la cámara.
   * @return Velocidad de la cámara.
   */
  double GetSpeed() {
    return Speed;
  }

  /**
   * Coloca la posición, rotaciones (yaw y pitch en radianes) de la cámara.
   * @param positionX Posición en X.
   * @param positionY Posición en Y.
   * @param positionZ Posición en Z
   * @param yaw Angulo de rotación horizontal de la cámara
   * @param pitch Angulo de rotación vertical de la cámara
   */
  void PositionCamera(float positionX, float positionY, float positionZ,
                      float yaw, float pitch);

  /**
   * Obtiene la matriz de rotación de la cámara, de las rotaciones yaw y pitch.
   * @return Matriz de rotación de la cámara.
   */
  glm::mat4 GetRotationMatrix();

  /**
   * Método que mueve la cámara hacia atras o adelanate dependiendo de la velocidad de la cámara.
   * @param speed Velocidad de la cámara.
   * @param viewVector Direción de vista de la cámara.
   */
  void MoveCamera(float speed, glm::vec3 viewVector);

  /**
   * Settea los valor de pitch an yaw a la cámara y los deltas
   * para el siguiente frame.
   * @param mouseX Movimiento en X.
   * @param mouseY Movimiento en Y
   */
  void SetViewByMouse(float mouseX, float mouseY);

 protected:

  // Matriz de proyeccion.
  glm::mat4 ProjectionMatrix;
  // Matriz de posición.
  glm::vec3 Position;

  // Velocidad de la cámara.
  float Speed = 1.0;
  // Velocidad del mouse.
  double MouseSpeed = 0.001;
  // Angulo de rotación horizontal de la cámara en radiandes eje y.
  float Yaw = 0;
  // Angulo de rotación vertical de la cámara en radiandes eje x.
  float Pitch = 0;
};

#endif
