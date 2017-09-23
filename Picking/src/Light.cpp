/*
 * Light.cpp
 *
 *  Created on: 18/04/2015
 *      Author: rey
 */

#include "Headers/Light.h"

namespace Light {

/**
 * Constructor default.
 */
Light::Light() {
}

Light::~Light() {
}

/**
 * Setter de atributos de la liluminación.
 * @param lightAtrr Parámetros de iluminación.
 */
void Light::SetLight(LightAtrr *lightAtrr) {
  this->lightAtrr = lightAtrr;
}

} /* namespace Light */
