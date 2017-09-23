/*
 * Light.h
 *
 *  Created on: 18/04/2015
 *      Author: rey
 */
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifndef LIGHT_H_
#define LIGHT_H_

namespace Light {

/**
 * Estructura para representar los atributos de la iluminaci�n
 */
struct LightAtrr {
  glm::vec4 lightPosW, lightColor, materialEmissive, materialDiffuse,
      materialSpecular, ambient;
  float materialShininess;
};

/**
 * Clase para la iluminaci�n.
 */
class Light {
 public:
  /**
   * Constructor default.
   */
  Light();
  /**
   * Setter de atributos de la liluminaci�n.
   * @param lightAtrr Par�metros de iluminaci�n.
   */
  void SetLight(LightAtrr * lightAtrr);
  /**
   * Destructor de clase.
   */
  virtual ~Light();
 private:
  LightAtrr * lightAtrr;
};

} /* namespace Light */
#endif /* LIGHT_H_ */
