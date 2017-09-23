#ifndef _Main_H
#define _Main_H

#include "WindowManager.h"
#include "Model.h"
#include "TimeManager.h"

#include "Camera.h"
#include "InputManager.h"
#include "LightingTechnique.h"
#include "Util.h"
#include "FontTypeRendering.h"
#include <math.h>
#include <sstream>
#include "RayCasting.h"
#include "ObjectCreation.h"

/**
 * Clase principal de la aplicacion, maneja la inicializaci�n de la aplicaci�n, loop de la aplicaci�n
 * y limpia los recursos utilizados.
 */
class GLApplication {
 public:

  /**
   *  El ancho y alto de la ventana.
   */
  int ScreenWidth = 600;
  int ScreenHeight = 400;

  static const float nearPlane;
  static const float farPlane;

  /**
   * Constructor default.
   */
  GLApplication() {
    // Inicializaci�n del cueternion de giro de los objetos.
    glm::quat rotX = glm::angleAxis<float>(glm::radians(0.0f),
                                           glm::vec3(1, 0, 0));
    glm::quat rotY = glm::angleAxis<float>(glm::radians(0.0f),
                                           glm::vec3(0, 1, 0));
    // Rotaci�n inicial.
    gRotation = rotX * rotY;
    // Posici�n inicial.
    mousePos = new glm::ivec2(0);
    // Modelo de renderizado de texeles.
    modelText = new FontTypeRendering::FontTypeRendering(ScreenWidth,
                                                         ScreenHeight);
  }
  /**
   * Destructor.
   */
  ~GLApplication() {
    Destroy();
  }

  /**
   * Funcion principal de la aplicaci�n, debe de ser llamada en la funci�n WindowManager.
   * @return Termino del programa.
   */
  int GLMain();

  /**
   * M�todo que obtiene el manejador de ventanas de la aplicaci�n.
   * Este debe ser creada antes de crear la aplicaci�n.
   * @return Manejador de ventanas.
   */
  WindowManager *GetWindowManager() {
    return windowManager;
  }
  /**
   * M�todo que setter del manejador de ventanas de la aplicaci�n.
   * Este debe ser setteada antes de inicializar el loop.
   * @param pWindowManager Manejador de ventanas que usar� la aplicaci�n.
   */
  void SetWindowManager(WindowManager *pWindowManager) {
    windowManager = pWindowManager;
  }

  /**
   * Obtiene la c�mara de la aplicaci�n.
   * @return Camar.
   */
  Camera *GetCamera() {
    return camera;
  }
  /**
   * Setter para la c�mara de la apliciaci�n.
   * @param pCamera c�mara que usar� aplicaici�n.
   */
  void SetCamera(Camera *pCamera) {
    camera = pCamera;
  }

  /**
   * Setter para la rotaci�n de la escena.
   * @param gRotation Cuaternion de rotaci�n de la escena
   */
  void SetGRotation(glm::quat gRotation) {
    this->gRotation = gRotation;
  }

  /**
   * Obtiene el Cuaternion actual de la escena.
   * @return Cuaternion de la aplicaci�n.
   */
  glm::quat GetGRotation() {
    return gRotation;
  }

  /**
   * Setter que maneja el trazado de rayos de la aplicaci�n.
   * @param picking Picking de la aplicaci�n.
   */
  void SetRayCasting(RayCasting::RayCasting * rayCasting) {
    this->rayCasting = rayCasting;
  }

  /**
   * Setter de la posici�n del mouse en coordenadas de la ventana.
   * @param x Coordenada en x de la venatana.
   * @param y Coordenada en x de la venatana.
   */
  void SetMousePos(int x, int y) {
    this->mousePos->x = x;
    this->mousePos->y = y;
  }

  /**
   * Inicializaci�n de la aplicaci�n.
   */
  void Initialize();

  /**
   * Loop de la aplicaci�n, solo se sale de esta funci�n si el usuario
   * pulsa Esc o cierra la ventana.
   */
  void ApplicationLoop();

  /**
   * M�todo que hace limpieza de los recursos utilizados por la aplicaci�n.
   */
  void Destroy();

 protected:

  // Abstracci�n del manejador de ventanas, crea ventanas y maneja entradas.
  // Ninguna clase debe manejar ventanas ni eventos, es a travez de tener
  // un atributo WindowManager que sea la misma para todas la clases que
  // utilice el manejadro de ventanas.
  WindowManager *windowManager;
  Camera *camera;
  //Cuaternion para girar los modelo.
  glm::quat gRotation;
  RayCasting::RayCasting * rayCasting;
  FontTypeRendering::FontTypeRendering *modelText;
  glm::ivec2 * mousePos;
  ObjectCreation objectCreation;
};

#endif
