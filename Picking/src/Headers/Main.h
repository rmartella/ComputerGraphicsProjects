#ifndef _Main_H
#define _Main_H

#include "WindowManager.h"
#include "Model.h"
#include "TimeManager.h"

#include "Camera.h"
#include "InputManager.h"
#include "LightingTechnique.h"
#include "Util.h"
#include "Picking.h"
#include "FontTypeRendering.h"
#include <sstream>

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
    /**
     * Inicialización de atributos.
     */
    modelText = new FontTypeRendering::FontTypeRendering(ScreenWidth,
                                                         ScreenHeight);
    mousePos = new glm::ivec2(0);
    sizeLandMark = 0.05;
    numMarks = 0;
    marksToDistance = new std::vector<Picking::Mark*>();
    rays = new std::vector<Model *>();
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
   * Setter que maneja el Picking de la aplicación.
   * @param picking Picking de la aplicación.
   */
  void SetPicking(Picking::Picking * picking) {
    this->picking = picking;
  }

  /**
   * Setter que habilita el trazado de rayos gráfico que ha realizado el Picking.
   * @param enableRayCastingGL Bandera de encendido o apagado.
   */
  void setEnableRayCastingGL(bool enableRayCastingGL) {
    this->enableRayCastingGL = enableRayCastingGL;
  }

  /**
   * Obtiene del estado de la bandera de  encendido o apagado del trasado de
   * rayos gráfico-.
   * @return
   */
  bool getEnableRayCastingGL() {
    return enableRayCastingGL;
  }

  /**
   * Setter del vector de marcas para el picking.
   * @param marks Vector de marcas.
   */
  void setMarks(const std::vector<Picking::Mark*>& marks) {
    this->marks = marks;
  }

  /**
   * Setter de la posición del mouse en coordenadas de la ventana.
   * @param x Coordenada en x de la venatana.
   * @param y Coordenada en x de la venatana.
   */
  void SetMousePos(int x, int y) {
    this->mousePos->x = x;
    this->mousePos->y = y;
  }

  /**
   * Obtiene el tamaño de los landmarks.
   * @return Tamaño del landmark.
   */
  float getSizeLandMark() const {
    return sizeLandMark;
  }

  /**
   * Setter para el tamaño del landmark.
   * @param sizeLendMark Radio de landmark.
   */
  void setSizeLandMark(float sizeLendMark) {
    this->sizeLandMark = sizeLendMark;
  }

  /**
   * Obtiene si esta habilitada la distancia entre landmarks.
   * @return Bandera de encendido o apagado.
   */
  bool isDistanceEnable() const {
    return distanceEnable;
  }

  /**
   * Setter de la bandera que indica si esta encendido o apagado el landmark.
   * @param distanceEnable Enecendido o apagado.
   */
  void setDistanceEnable(bool distanceEnable) {
    this->distanceEnable = distanceEnable;
  }

  /**
   * Obtiene el número de landmarks seleccionadas.
   * @return Número de landmarks que ha selecciónado el usuario.
   */
  int getNumMarks() const {
    return numMarks;
  }

  /**
   * Setter del Número de landmarks marcadas para obtener su distancia.
   * @param numMarks Número de landmarks marcadas para obtener su distancia.
   */
  void setNumMarks(int numMarks) {
    this->numMarks = numMarks;
  }

  /**
   * Obtiene las marcas que han sido seleccionadas para obtener su distancia.
   * @return Vector de marcas.
   */
  std::vector<Picking::Mark*>*getMarksToDistance() {
    return marksToDistance;
  }

  /**
   * Setter para la bandera de eliminación de landmark.
   * @param deleteEnable
   */
  void setDeleteEnable(bool deleteEnable) {
    this->deleteEnable = deleteEnable;
  }

 protected:
  void SolidSphere(float radius, int slices, int stacks, Vertex3 * vertices,
                   GLuint * indices);
 protected:

  // Abstracción del manejador de ventanas, crea ventanas y maneja entradas.
  // Ninguna clase debe manejar ventanas ni eventos, es a travez de tener
  // un atributo WindowManager que sea la misma para todas la clases que
  // utilice el manejadro de ventanas.
  WindowManager *windowManager;
  Camera *camera;
  //Cuaternion para girar los modelo.
  glm::quat gRotation;
  Picking::Picking * picking;
  bool enableRayCastingGL = false;
  std::vector<Picking::Mark*> marks;
  glm::ivec2 * mousePos;
  FontTypeRendering::FontTypeRendering * modelText;
  Model * sphere;
  Model * humanHead;
  float sizeLandMark;
  bool distanceEnable;
  bool deleteEnable;
  std::vector<Picking::Mark*> * marksToDistance;
  int numMarks;
  std::vector<Model*> * rays;
};

#endif
