#ifndef _Main_H
#define _Main_H

#include "WindowManager.h"
#include "Model.h"
#include "TimeManager.h"

#include "Camera.h"
#include "InputManager.h"
#include "RubiksCube.h"

/**
 * Clase principal de la aplicacion, maneja la inicialización de la aplicación, loop de la aplicación
 * y limpia los recursos utilizados.
 */
class GLApplication {
 public:

  /**
   *  El ancho y alto de la ventana.
   */
  int ScreenWidth = 1024;
  int ScreenHeight = 700;

  /**
   * Constructor default.
   */
  GLApplication() {
    // Inicialización del cueternion de giro de los objetos.
    glm::quat rotX = glm::angleAxis<float>(glm::radians(22.5f),
                                           glm::vec3(1, 0, 0));
    glm::quat rotY = glm::angleAxis<float>(glm::radians(22.5f),
                                           glm::vec3(0, 1, 0));
    // Rotación inicial.
    gRotation = rotX * rotY;

    rotationCubeManager = new RotationCubeManager::RotationCubeManager();
  }
  /**
   * Destructor.
   */
  ~GLApplication() {
    Destroy();
  }

  /**
   * Funcion principal de la aplicación, debe de ser llamada en la función WindowManager.
   * @return Termino del programa.
   */
  int GLMain();

  /**
   * Método que obtiene el manejador de ventanas de la aplicación.
   * Este debe ser creada antes de crear la aplicación.
   * @return Manejador de ventanas.
   */
  WindowManager *GetWindowManager() {
    return windowManager;
  }
  /**
   * Método que setter del manejador de ventanas de la aplicación.
   * Este debe ser setteada antes de inicializar el loop.
   * @param pWindowManager Manejador de ventanas que usará la aplicación.
   */
  void SetWindowManager(WindowManager *pWindowManager) {
    windowManager = pWindowManager;
  }

  /**
   * Obtiene la cámara de la aplicación.
   * @return Camar.
   */
  Camera *GetCamera() {
    return camera;
  }
  /**
   * Setter para la cámara de la aplicación.
   * @param pCamera cámara que usará aplicación.
   */
  void SetCamera(Camera *pCamera) {
    camera = pCamera;
  }

  /**
   * Setter para la rotación de la escena.
   * @param gRotation Cuaternion de rotación de la escena
   */
  void SetGRotation(glm::quat gRotation) {
    this->gRotation = gRotation;
  }

  /**
   * Obtiene el Cuaternion actual de la escena.
   * @return Cuaternion de la aplicación.
   */
  glm::quat GetGRotation() {
    return gRotation;
  }

  /**
   * Setter para armar el cubo.
   * @param armar Bander para armar.
   */
  void SetArmar(bool armar) {
    this->armar = armar;
  }

  /**
   * Setter para el manejador de rotaciones.
   * @param rotationCubeManager Manejador de rotaciones de las caras del cubo.
   */
  void SetRotationCubeManager(
      RotationCubeManager::RotationCubeManager * rotationCubeManager) {
    this->rotationCubeManager = rotationCubeManager;
  }
  /**
   * Obtiene el manejador de rotaciones.
   * @return Manejador de rotaciones de las caras del cubo.
   */
  RotationCubeManager::RotationCubeManager * GetRotationCubeManager() {
    return this->rotationCubeManager;
  }

  /**
   * Setter para la velocidad del rotación de la cara.
   * @param speedRotattionFaces Velocidad del rotación de la cara.
   */
  void SetSpeedRotattionFaces(float speedRotattionFaces) {
    this->speedRotattionFaces = speedRotattionFaces;
  }

  /**
   * Inicialización de la aplicación.
   */
  void Initialize();

  /**
   * Loop de la aplicación, solo se sale de esta función si el usuario
   * pulsa Esc o cierra la ventana.
   */
  void ApplicationLoop();

  /**
   * Método que hace limpieza de los recursos utilizados por la aplicación.
   */
  void Destroy();

  glm::mat4 eRotation;
 protected:

  // Abstracción del manejador de ventanas, crea ventanas y maneja entradas.
  // Ninguna clase debe manejar ventanas ni eventos, es a travez de tener
  // un atributo WindowManager que sea la misma para todas la clases que
  // utilice el manejadro de ventanas.
  WindowManager *windowManager;
  Camera *camera;
  //Cuaternion para girar los modelo.
  glm::quat gRotation;

  RotationCubeManager::RotationCubeManager * rotationCubeManager;

  float speedRotattionFaces;

  bool armar;

};

#endif
