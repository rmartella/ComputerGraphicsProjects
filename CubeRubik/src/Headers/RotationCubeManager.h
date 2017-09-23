/*
 * RotationCubeManager.h
 *
 *  Created on: 05/05/2015
 *      Author: rey
 */
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtx/quaternion.hpp>

#ifndef ROTATIONCUBEMANAGER_H_
#define ROTATIONCUBEMANAGER_H_

#define EPSILON 0.000001

// Representaci�n de movimeitnos cara y direcci�n.
struct Movimiento {
  int face, direction;
};

namespace RotationCubeManager {

/**
 * Clase que maneja y registra las rotaciones que realiza el cubo.
 */
class RotationCubeManager {
 public:
  /**
   * Constructor.
   */
  RotationCubeManager();
  /**
   * Metodo que agrega una rotaci�n
   * @param face Cara que se agrega.
   * @param direction Direcci�n de la cara.
   */
  void addRotation(int face, int direction);
  /**
   * M�todo que elimina la rotaci�n de una cara al completar su giro.
   */
  void deleteCurrentCompleteRotation();
  /**
   * Metodo que obtiene el ultimo movimiento que esta realizando el rubick.
   */
  Movimiento * getCurrentRotation();
  /**
   * M�todo que actualiza las orientaci�n de las caras del rubick.
   */
  void updateOrientation();
  /**
   * M�todo que mapea las caras reales con las que fue contruido el rubick y con las caras que estan
   * De frente al observador.
   * @param faceAxisX Cara que esta viendo el observador.
   * @param faceAxisY Cara que esta viendo una camara invisible sobre el eje y.
   */
  void updateFaceMap(int faceAxisX, float faceAxisY);
  /**
   *M�todo que obtiene la intersecci�n del rayo con un tri�ngulo,
   *Se utiliza el algortimo de Tomas Moller.
   *  Representaci�n del rayo en coordenadas parametricas.
   *    R ( t ) = O + tD  ------- (1)
   *  Un punto de un tri�ngulo esta dado por las coordenadas varicentricas,
   *  del triangulo, u >= 0, v >= 0 y u + v <= 1.
   *    T ( u v ) = (1 ; u ; v ) V 0 + uV 1 + vV 2  ------- (2)
   * @param orig Origen del rayo.
   * @param dir Direcci�n del rayo.
   * @param vert0 Vertice 1 del triangulo a probar.
   * @param vert1 Vertice 2 del triangulo a probar.
   * @param vert2 Vertice 3 del triangulo a probar.
   * @param t Valor de ecuaci�n de (1)
   * @param u Coordenada u varicentrica del tri�ngulo (2).
   * @param v Coordenada v varicentrica del tri�ngulo (2).
   * @return Bandera que indica si hay intersecci�n o no, true existe la intersecci�n
   * false no existe la intersecci�n.
   */
  bool RayTriangleIntersect(glm::vec3 orig, glm::vec3 dir, glm::vec3 vert0,
                            glm::vec3 vert1, glm::vec3 vert2, double *t,
                            double *u, double *v);
  /**
   * M�todo que guarda el estado del rubick al cerrar la ventana.
   */
  void saveRotations();
  /**
   * M�tdo que carga las rotaciones al iniciar sesi�n.
   */
  void loadRotations();
  /**
   * Setter de los vertices del cubo.
   * @param verticesCube
   */
  void SetVerticesCube(Vertex3 * verticesCube) {
    this->verticesCube = verticesCube;
  }
  /**
   * Setter de indicies del cubo.
   * @param indexCube Indices del cubo.
   */
  void SetIndexCube(GLubyte * indexCube) {
    this->indexCube = indexCube;
  }
  /**
   * Setter de la camara.
   * @param camera Camara.
   */
  void SetCamera(Camera * camera) {
    this->camera = camera;
  }
  /**
   * Setter de la matriz del rubick.
   * @param WorldMatrix Matriz rubick.
   */
  void SetWorldMatrix(glm::mat4 WorldMatrix) {
    this->WorldMatrix = WorldMatrix;
  }
  /**
   * M�todo que obtiene los movimientos que se han realizado sobre el Cubo de Rubick.
   * @return
   */
  std::vector<Movimiento*> * GetMovimientosSave() {
    return this->movimientosSave;
  }
  /**
   * Destructor.
   */
  virtual ~RotationCubeManager();

 private:
  int faceMap[6];
  float axisXAngleAct = 0.7852;
  std::vector<Movimiento*> * movimientos = new std::vector<Movimiento*>();
  std::vector<Movimiento*> * movimientosSave = new std::vector<Movimiento*>();
  Vertex3 * verticesCube;
  GLubyte * indexCube;
  Camera * camera;
  glm::mat4 WorldMatrix;
};

} /* namespace RotationCubeManager */
#endif /* ROTATIONCUBEMANAGER_H_ */
