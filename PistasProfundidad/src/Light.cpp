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
 * Setter de atributos de la liluminaci�n.
 * @param lightAtrr Par�metros de iluminaci�n.
 */
void Light::SetLight(LightAtrr *lightAtrr) {
  this->lightAtrr = lightAtrr;
}

} /* namespace Light */
