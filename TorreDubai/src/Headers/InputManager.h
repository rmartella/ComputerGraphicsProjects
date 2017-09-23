#ifndef _InputManager_H
#define _InputManager_H

#define GLM_FORCE_RADIANS// Aseguramos que GLM use radianes en lugar de grados
#include <glm/glm.hpp>//Include para la librería GLM
#include <glm/gtc/matrix_transform.hpp>//Include para la librería GLM
#include <glm/gtx/transform2.hpp>//Se incluye para manejar con GLM las transformaciones
#include <glm/gtx/quaternion.hpp>//Se incluye para utilizar quterniones en ciertos casos
#include "Camera.h"// Se include para enviarle los comandos de movimientos a la cámara

// Estandarización de GLFW para el manejo de codigos estandarizados de entradas del teclado
enum InputCodes {
  kEscape = 27,
  Space = 32,
  Left = 37,
  Up = 38,
  Right = 39,
  Down = 40,
  a = 97,
  A = 65,
  b = 98,
  B = 66,
  c = 99,
  C = 67,
  d = 100,
  D = 68,
  e = 101,
  E = 69,
  f = 102,
  F = 70,
  g = 103,
  G = 71,
  h = 104,
  H = 72,
  i = 105,
  I = 73,
  j = 106,
  J = 74,
  k = 107,
  K = 75,
  l = 108,
  L = 76,
  m = 109,
  M = 77,
  n = 110,
  N = 78,
  o = 111,
  O = 79,
  p = 112,
  P = 80,
  q = 113,
  Q = 81,
  r = 114,
  R = 82,
  s = 115,
  S = 83,
  t = 116,
  T = 84,
  u = 117,
  U = 85,
  v = 118,
  V = 86,
  w = 119,
  W = 87,
  x = 120,
  X = 88,
  y = 121,
  Y = 89,
  z = 122,
  Z = 90,
};

/**
 * Clase que maneja las entradas del usuario y se lo envia a la cámara.
 */
class InputManager {
 public:

  /**
   * Envia a la cámara el tipo de entrada para que la cámara se mueva.
   * @param code Codigos de entrada.
   */
  void KeyPressed(InputCodes code);

  /**
   * Setter de la cámara.
   * @param pCamera cámara del manejador.
   */
  void SetCamera(Camera *pCamera) {
    camera = pCamera;
  }
  /**
   * Obtiene la cámara que usa el manejador de entradas.
   * @return
   */
  Camera *GetCamera() {
    return camera;
  }

  /**
   * Setter de la posición de la cámara.
   * @param gMousePos Posición del mouse.
   */
  void SetGMousePos(glm::ivec2 * gMousePos) {
    this->gMousePos = gMousePos;
  }

  /**
   * Obtiene la posición actual del mouse que tiene el manejador.
   * @return Posición del mouse.
   */
  glm::ivec2 * getGMousePos() {
    return gMousePos;
  }

  /**
   * Método que maneja los moviemientos del mouse.
   * @param mouseX Posición del mouse en X.
   * @param mouseY Posición del mouse en Y
   */
  void MouseMoved(float mouseX, float mouseY);

 protected:
  Camera *camera;
  glm::ivec2 * gMousePos = NULL;
};

#endif
