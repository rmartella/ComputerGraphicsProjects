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
    /**
     * Inicializaci�n de atributos.
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
   * Funcion principal de la aplicaci�n, debe de ser llamada en la funci�n WindowManager.
   * @return Termino del programa.
   */
  int GLMain();

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
   * Setter que maneja el Picking de la aplicaci�n.
   * @param picking Picking de la aplicaci�n.
   */
  void SetPicking(Picking::Picking * picking) {
    this->picking = picking;
  }

  /**
   * Setter que habilita el trazado de rayos gr�fico que ha realizado el Picking.
   * @param enableRayCastingGL Bandera de encendido o apagado.
   */
  void setEnableRayCastingGL(bool enableRayCastingGL) {
    this->enableRayCastingGL = enableRayCastingGL;
  }

  /**
   * Obtiene del estado de la bandera de  encendido o apagado del trasado de
   * rayos gr�fico-.
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
   * Setter de la posici�n del mouse en coordenadas de la ventana.
   * @param x Coordenada en x de la venatana.
   * @param y Coordenada en x de la venatana.
   */
  void SetMousePos(int x, int y) {
    this->mousePos->x = x;
    this->mousePos->y = y;
  }

  /**
   * Obtiene el tama�o de los landmarks.
   * @return Tama�o del landmark.
   */
  float getSizeLandMark() const {
    return sizeLandMark;
  }

  /**
   * Setter para el tama�o del landmark.
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
   * Obtiene el n�mero de landmarks seleccionadas.
   * @return N�mero de landmarks que ha selecci�nado el usuario.
   */
  int getNumMarks() const {
    return numMarks;
  }

  /**
   * Setter del N�mero de landmarks marcadas para obtener su distancia.
   * @param numMarks N�mero de landmarks marcadas para obtener su distancia.
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
   * Setter para la bandera de eliminaci�n de landmark.
   * @param deleteEnable
   */
  void setDeleteEnable(bool deleteEnable) {
    this->deleteEnable = deleteEnable;
  }

 protected:
  void SolidSphere(float radius, int slices, int stacks, Vertex3 * vertices,
                   GLuint * indices);
 protected:

  // Abstracci�n del manejador de ventanas, crea ventanas y maneja entradas.
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
