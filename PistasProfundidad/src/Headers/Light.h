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
 * Estructura para representar los atributos de la iluminación
 */
struct LightAtrr {
  glm::vec4 lightPosW, lightColor, materialEmissive, materialDiffuse,
      materialSpecular, ambient;
  float materialShininess;
};

/**
 * Clase para la iluminación.
 */
class Light {
 public:
  /**
   * Constructor default.
   */
  Light();
  /**
   * Setter de atributos de la liluminación.
   * @param lightAtrr Parámetros de iluminación.
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
