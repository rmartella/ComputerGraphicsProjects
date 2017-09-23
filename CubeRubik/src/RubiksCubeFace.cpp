/*
 * RubiksCubeFace.cpp
 *
 *  Created on: 04/04/2015
 *      Author: rey
 */

#include "Headers/RubiksCubeFace.h"

float currAngle;

/**
 * Constructor de una cara del cubo.
 * @param centerCubie Cubo centrico.
 * @param axis Eje de giro.
 */
RubiksCubeFace::RubiksCubeFace(Node * centerCubie, glm::vec3 axis) {
  this->cubosCentricos = centerCubie;
  this->ejeGiro = axis;
}

/**
 * Constructor defualt.
 */
RubiksCubeFace::RubiksCubeFace() {
}
/**
 * Destructor.
 */
RubiksCubeFace::~RubiksCubeFace() {
}

/**
 * Método que agrega una arista al nodo central, para formar una cruz con el cubo.
 * @param edge Nuevo nodo.
 * @param id Indice en el arreglo de nodos.
 */
void RubiksCubeFace::addAdjacentToFace(Node * edge, int id) {
  cubosAdjacentes[id] = edge;
}
/**
 * Agrega los extremos de una cara.
 * @param corner Nodo esquina.
 * @param id Indice en el arreglo de nodos.
 */
void RubiksCubeFace::addCornerToFace(Node * corner, int id) {
  this->cubosEsquinas[id] = corner;
}

/**
 * Se incrementa el giro de la cara.
 * @param angleFlip Incremento en la rotación de giro de cara.
 * @param direction Dirección de giro.
 * @param portionTurn Porción de giro.
 */
void RubiksCubeFace::rotate(float angleFlip, bool direction,
                            float turnPortion) {
  // Se calcula el angulo que debe girar, para completar el giro indicado
  float incrementAngle = (angleFlip - currAngle) * glm::radians(360.0f)
      * turnPortion;
  currAngle = fmod(angleFlip, 1);
  // Determina si es angulo positivo o negativo depende de la dirección de giro.
  incrementAngle = direction ? incrementAngle : -incrementAngle;
  // Se giran primero los cubos centricos de la cara.
  rotateFace(cubosCentricos, incrementAngle);
  for (int i = 0; i < 4; i++) {
    // Se giran los cubos adjacentes de la cara.
    rotateFace(cubosAdjacentes[i], incrementAngle);
    // Se giran los cubos de las esquinas de la cara.
    rotateFace(cubosEsquinas[i], incrementAngle);
  }
}

/**
 * Se incrementa el giro de la cara.
 * @param ratio Incremento en la rotación de giro de cara.
 * @param direction Dirección de giro.
 * @param portionTurn Porción de giro.
 */
void RubiksCubeFace::rotateFace(Node * cubie, float incAngle) {
  glm::mat4 rotateMat(1.0f);
  glm::mat4 oldMat = *cubie->getTransformMatrix();
  rotateMat = glm::rotate(rotateMat, incAngle, this->ejeGiro);
  // Se agrega giro a la tranfromación acumulada.
  glm::mat4 * finalTransformation = new glm::mat4(rotateMat * oldMat);
  cubie->setTransformMatrix(finalTransformation);
}
/**
 * Método que realiza el re-acomodo en las aristas al completar un giro.
 * @param lastTurnDirection Dirección de giro que se completo.
 */
void RubiksCubeFace::flipFacesMatrix(bool lastTurnDirection) {
  // Giro positivo se intercambian esquinas y cubos adjacentes.
  if (lastTurnDirection) {
    glm::mat4 * temp = cubosAdjacentes[3]->getTransformMatrix();
    for (int i = 3; i > 0; i--) {
      cubosAdjacentes[i]->setTransformMatrix(
          cubosAdjacentes[i - 1]->getTransformMatrix());
    }
    cubosAdjacentes[0]->setTransformMatrix(temp);
    temp = cubosEsquinas[3]->getTransformMatrix();
    for (int i = 3; i > 0; i--) {
      cubosEsquinas[i]->setTransformMatrix(
          cubosEsquinas[i - 1]->getTransformMatrix());
    }
    cubosEsquinas[0]->setTransformMatrix(temp);
  } else {
    // Giro negativo se intercambian esquinas y cubos adjacentes.
    glm::mat4 * temp = cubosAdjacentes[0]->getTransformMatrix();
    for (int i = 0; i < 3; i++) {
      cubosAdjacentes[i]->setTransformMatrix(
          cubosAdjacentes[i + 1]->getTransformMatrix());
    }
    cubosAdjacentes[3]->setTransformMatrix(temp);
    temp = cubosEsquinas[0]->getTransformMatrix();
    for (int i = 0; i < 3; i++) {
      cubosEsquinas[i]->setTransformMatrix(
          cubosEsquinas[i + 1]->getTransformMatrix());
    }
    cubosEsquinas[3]->setTransformMatrix(temp);
  }
}
