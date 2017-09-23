/*
 * TowerLevel.h
 *
 *  Created on: 16/05/2015
 *      Author: rey
 */
#include "Model.h"
#include "ModelEnvironment.h"

#ifndef TOWERLEVEL_H_
#define TOWERLEVEL_H_

/**
 * Representación de un nivel de la torre.
 */

class TowerLevel {
 public:
  /**
   * Contructor de torre tomando dos modelos, un modelo con iluminación y el otro
   * con el mapeo del ambiente.
   * @param modelStruct
   * @param modelEnvironment
   */
  TowerLevel(Model * modelStruct, ModelEnvironment * modelEnvironment);
  /**
   * Setter de la matriz de transformación de un nivel.
   * @param levelTransform Matriz de transformación de un nivel.
   */
  void setLevelTransform(glm::mat4 * levelTransform) {
    this->levelTransform = levelTransform;
  }
  /**
   * Obtiene la matriz de transformación de un nivel.
   * @return Matriz de transformación de un nivel.
   */
  glm::mat4 * getLevelTransform() {
    return this->levelTransform;
  }
  /**
   * Obtiene el giro que ha dado la torre en ese instante.
   * @return Giro que ha dado la torre en ese instante.
   */
  float getGiroAct() {
    return this->giroAct;
  }
  /**
   * Setter de giro que ha dado la torre en ese instante.
   * @param giroAct Giro que ha dado la torre en ese instante.
   */
  void setGiroAct(float giroAct) {
    this->giroAct = giroAct;
  }

  /**
   * Método que renderiza el nivel de la torre.
   * @param rootTransform Transformación de su nodo padre.
   */
  void render(glm::mat4 rootTransform);

  /**
   * Obtiene el angulo máximo que puede girrar la torre.
   * @return Angulo máximo que puede girrar la torre.
   */
  float getAngleRotMax() const {
    return angleRotMax;
  }

  /**
   * Setter para el angulo maximo que puede girar.
   * @param angleRotMax Angulo máximo que puede girrar la torre.
   */
  void setAngleRotMax(float angleRotMax) {
    this->angleRotMax = angleRotMax;
  }

  /**
   * Obtiene la dirección de giro.
   * @return Dirección de giro.
   */
  float getDirection() const {
    return direction;
  }
  /**
   * Setter para la dirección de giro.
   * @param direction Direccion de giro.
   */
  void setDirection(int direction = 0) {
    this->direction = direction;
  }
  /**
   * Destructor de clase.
   */
  virtual ~TowerLevel();

 private:
  Model * modelStruct;
  ModelEnvironment * modelEnvironment;
  float giroAct = 0;
  float angleRotMax;
  int direction = 0;
  glm::mat4 * levelTransform;
};

#endif /* TOWERLEVEL_H_ */
