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
 * Clase que maneja la t�cnica de iluminaci�n.
 */
class LightingTechnique {
 public:

  /**
   * Constructor.
   * @param numLights N�mero de luces.
   */
  LightingTechnique(int numLights);

  /**
   * M�todo que inicializa la iluminaci�n.
   */
  virtual void Init();

  /**
   * Envia la unidad de textura a utilizar.
   * @param TextureUnit Unidad de textura.
   */
  void SetTextureUnit(unsigned int TextureUnit);
  /**
   * M�todo que envia los atributos de la iluminaci�n.
   * @param Light
   */
  void SetAttrLight(LightAtrr * Light);
  /**
   * M�todo que envia la posicion del punto de vista (Posici�n de la c�mara).
   * @param EyeWorldPos
   */
  void SetEyeWorldPos(const glm::vec3& EyeWorldPos);

  /**
   * Setter para colocar el shader a utilizar y as� poder enviarle la informaci�n.
   * @param shader
   */
  void SetShader(Shader * shader);

 private:
  Shader * shader;

  GLuint m_gSampler;
  GLuint eyePosW;
  GLuint gNumLights;

  /**
   * Estructura de identificadores para iluminaci�n de las variables del shader.
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
   * N�mero de luces.
   */
  int numLights;
  /**
   * Apuntador a os atributos de la iluminaci�n.
   */
  Light * lightsPtr;
};

#endif    /* LIGHTING_TECHNIQUE_H */
