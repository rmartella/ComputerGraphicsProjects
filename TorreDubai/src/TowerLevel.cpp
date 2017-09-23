/*
 * TowerLevel.cpp
 *
 *  Created on: 16/05/2015
 *      Author: rey
 */

#include "Headers/TowerLevel.h"

/**
 * Contructor de torre tomando dos modelos, un modelo con iluminación y el otro
 * con el mapeo del ambiente.
 * @param modelStruct
 * @param modelEnvironment
 */
TowerLevel::TowerLevel(Model * modelStruct,
                       ModelEnvironment * modelEnvironment) {
  this->modelStruct = modelStruct;
  this->modelEnvironment = modelEnvironment;
}
/**
 * Método que renderiza el nivel de la torre.
 * @param rootTransform Transformación de su nodo padre.
 */
void TowerLevel::render(glm::mat4 rootTransform) {
  this->modelStruct->Render(rootTransform);
  this->modelEnvironment->Render(rootTransform);
}
/**
 * Destructor de clase.
 */
TowerLevel::~TowerLevel() {
}
