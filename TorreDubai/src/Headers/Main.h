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
 * Clase principal de la aplicacion, maneja la inicializaci�n de la aplicaci�n, loop de la aplicaci�n
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
   * Radio de precisi�n para los Landmarks.
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
   * Setter para la c�mara de la aplicaci�n.
   * @param pCamera c�mara que usar� aplicaci�n.
   */
  void SetCamera(Camera *pCamera) {
    camera = pCamera;
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
  SkyBoxModel * skyBoxModel;
  Model * modelSolid;
  ModelEnvironment * modelEnvironment;
  Model * planeModel;

};

#endif
