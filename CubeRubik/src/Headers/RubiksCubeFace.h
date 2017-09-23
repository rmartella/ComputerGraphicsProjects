/*
 * RubiksCubeFace.h
 *
 *  Created on: 04/04/2015
 *      Author: rey
 */

#ifndef RUBIKSCUBEFACE_H_
#define RUBIKSCUBEFACE_H_
#include "../Headers/Node.h"

/**
 * Clase que representa una cara del cubo de rubick.
 */
class RubiksCubeFace {
 public:

  /**
   * Constructor de una cara del cubo.
   * @param centerCubie Cubo centrico.
   * @param axis Eje de giro.
   */
  RubiksCubeFace(Node * centerCubie, glm::vec3 axis);
  /**
   * Constructor defualt.
   */
  RubiksCubeFace();
  /**
   * Destructor.
   */
  virtual ~RubiksCubeFace();
  /**
   * Método que agrega un cubo adjacente a la cara (Forma de cruz del cara.)
   * @param edge Nuevo nodo.
   * @param id Indice en el arreglo de nodos.
   */
  void addAdjacentToFace(Node * edge, int id);
  /**
   * Agrega los extremos de una cara.
   * @param corner Nodo esquina.
   * @param id Indice en el arreglo de nodos.
   */
  void addCornerToFace(Node * corner, int id);
  /**
   * Se incrementa el giro de la cara.
   * @param angleFlip Incremento en la rotación de giro de cara.
   * @param direction Dirección de giro.
   * @param portionTurn Porción de giro.
   */
  void rotate(float angleFlip, bool direction, float portionTurn);
  /**
   * Método que realiza el re-acomodo en las aristas al completar un giro.
   * @param lastTurnDirection Dirección de giro que se completo.
   */
  void flipFacesMatrix(bool lastTurnDirection);
 private:
  Node * cubosCentricos;
  Node * cubosAdjacentes[4];
  Node * cubosEsquinas[4];
  glm::vec3 ejeGiro;
  void rotateFace(Node * cubie, float incAngle);
};

#endif /* RUBIKSCUBEFACE_H_ */
