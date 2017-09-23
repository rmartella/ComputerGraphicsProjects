#ifndef _Main_H
#define _Main_H

#include "WindowManager.h"
#include "Model.h"
#include "ModelEnvironment.h"
#include "TimeManager.h"

#include "Camera.h"
#include "InputManager.h"
#include "LightingTechnique.h"
#include "Util.h"
#include "SkyBoxModel.h"
#include "TowerDubai.h"

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
   * Radio de precisión para los Landmarks.
   */
  const float ratioSphere = 1.0;

  /**
   * Constructor default.
   */
  GLApplication() {
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

 protected:
  // Abstracción del manejador de ventanas, crea ventanas y maneja entradas.
  // Ninguna clase debe manejar ventanas ni eventos, es a travez de tener
  // un atributo WindowManager que sea la misma para todas la clases que
  // utilice el manejadro de ventanas.
  WindowManager *windowManager;
  Camera *camera;
  SkyBoxModel * skyBoxModel;
  Model * modelSolid;
  ModelEnvironment * modelEnvironment;
  Model * planeModel;

};

#endif
