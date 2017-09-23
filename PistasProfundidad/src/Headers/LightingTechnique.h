#include <GL/glew.h>
#include "Shader.h"
#include "Light.h"
#include <iostream>
#ifdef _WIN32
#define snprintf _snprintf_s
#endif

#ifndef LIGHTING_TECHNIQUE_H
#define    LIGHTING_TECHNIQUE_H

using namespace Light;

/**
 * Clase que maneja la técnica de iluminación.
 */
class LightingTechnique {
 public:

  /**
   * Constructor.
   * @param numLights Número de luces.
   */
  LightingTechnique(int numLights);

  /**
   * Método que inicializa la iluminación.
   */
  virtual void Init();

  /**
   * Envia la unidad de textura a utilizar.
   * @param TextureUnit Unidad de textura.
   */
  void SetTextureUnit(unsigned int TextureUnit);
  /**
   * Método que envia los atributos de la iluminación.
   * @param Light
   */
  void SetAttrLight(LightAtrr * Light);
  /**
   * Método que envia la posicion del punto de vista (Posición de la cámara).
   * @param EyeWorldPos
   */
  void SetEyeWorldPos(const glm::vec3& EyeWorldPos);

  /**
   * Setter para colocar el shader a utilizar y así poder enviarle la información.
   * @param shader
   */
  void SetShader(Shader * shader);

 private:
  Shader * shader;

  GLuint m_gSampler;
  GLuint eyePosW;
  GLuint gNumLights;

  /**
   * Estructura de identificadores para iluminación de las variables del shader.
   */
  typedef struct m_light {
    GLuint LightPosW;
    GLuint LightColor;
    GLuint MaterialEmissive;
    GLuint MaterialDiffuse;
    GLuint MaterialSpecular;
    GLuint MaterialShininess;
    GLuint Ambient;
  } Light;

  /**
   * Número de luces.
   */
  int numLights;
  /**
   * Apuntador a os atributos de la iluminación.
   */
  Light * lightsPtr;
};

#endif    /* LIGHTING_TECHNIQUE_H */
