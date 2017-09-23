/*
 * RubiksCube.h
 *
 *  Created on: 04/04/2015
 *      Author: rey
 */

#ifndef RUBIKSCUBE_H_
#define RUBIKSCUBE_H_

#include "RubiksCubeFace.h"
#include "RotationCubeManager.h"

class RubiksCube {
 public:
  /**
   * Constructor.
   */
  RubiksCube();
  /**
   * Destructor.
   */
  RubiksCube(Node * rootNode);
  /**
   * Método que crea los centros de las caras del cubo.
   * @param rootNode Nodo padre.
   */
  virtual ~RubiksCube();
  /**
   * Método que rota automaticamente las caras, es decir para restarurar las sesiones y
   * restatura el estado del rubick
   * @param turnRatio Porción que debe girar.
   * @param armar Indica que es un armado de cara, realiza la obtención de arriba hacia abajo de la cola de giros
   * y en dirección contraría.
   * @param complete Se completaron los giros, se puede volver a girar las caras manualmente.
   */
  void rotateFaceAuto(float turnRatio, bool armar, bool * complete);
  /**
   * Rota una cara y se detiene cuando ha acumulado un giro de 90°
   * @param turnRatio
   * @param armar Bandera que indica que al ultimo giro de la cara del rubick debe de armarse.
   * @param ready Bandera que indica que ya puede armarse completo el ultimo giro.
   */
  void rotateFace(float turnRatio, bool armar, bool * ready);
  /**
   * Setter para el manejadro de rotaciones de caras del cubo.
   * @param rotationCubeManager Manejadro de rotaciones de caras del cubo.
   */
  void SetRotationCubeManager(
      RotationCubeManager::RotationCubeManager * rotationCubeManager) {
    this->rotationCubeManager = rotationCubeManager;
  }
  /**
   * Obtiene para el manejadro de rotaciones de caras del cubo.
   * @param rotationCubeManager Manejadro de rotaciones de caras del cubo.
   */
  RotationCubeManager::RotationCubeManager * GetRotationCubeManager() {
    return this->rotationCubeManager;
  }
 private:
  RubiksCubeFace faces[6];
  void addCenters(Node * rootNode);
  void addEdgesAndCorners(Node * rootNode);

  float currTurnRatio = 0;
  int currFace = 0;
  int indexArray = 0;
  bool currDirection = false;
  float currTurnPortion = 1 / 4.0f;
  RotationCubeManager::RotationCubeManager * rotationCubeManager = NULL;
};

#endif /* RUBIKSCUBE_H_ */
