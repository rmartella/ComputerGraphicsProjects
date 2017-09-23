#include "Headers/LightingTechnique.h"

/**
 * Constructor.
 * @param numLights Número de luces.
 */
LightingTechnique::LightingTechnique(int numLights) {
  this->numLights = numLights;
  lightsPtr = new Light[numLights];
}

/**
 * Método que inicializa la iluminación.
 */
void LightingTechnique::Init() {

  m_gSampler = shader->GetVariable("gSampler");
  eyePosW = shader->GetVariable("EyePosW");
  gNumLights = shader->GetVariable("gNumLights");
  if (m_gSampler == 0xFFFFFFFF || gNumLights == 0xFFFFFFFF) {
    std::cout << "No se puede inicializar la luz" << std::endl;
    exit(-1);
  }
  for (int i = 0; i < numLights; i++) {
    char Name[128];
    memset(Name, 0, sizeof(Name));
    snprintf(Name, sizeof(Name), "gLight[%d].LightPosW", i);
    lightsPtr[i].LightPosW = shader->GetVariable(Name);
    snprintf(Name, sizeof(Name), "gLight[%d].LightColor", i);
    lightsPtr[i].LightColor = shader->GetVariable(Name);
    snprintf(Name, sizeof(Name), "gLight[%d].MaterialEmissive", i);
    lightsPtr[i].MaterialEmissive = shader->GetVariable(Name);
    snprintf(Name, sizeof(Name), "gLight[%d].MaterialDiffuse", i);
    lightsPtr[i].MaterialDiffuse = shader->GetVariable(Name);
    snprintf(Name, sizeof(Name), "gLight[%d].MaterialSpecular", i);
    lightsPtr[i].MaterialSpecular = shader->GetVariable(Name);
    snprintf(Name, sizeof(Name), "gLight[%d].MaterialShininess", i);
    lightsPtr[i].MaterialShininess = shader->GetVariable(Name);
    snprintf(Name, sizeof(Name), "gLight[%d].Ambient", i);
    lightsPtr[i].Ambient = shader->GetVariable(Name);

    if (lightsPtr[i].LightPosW == 0xFFFFFFFF
        || lightsPtr[i].LightColor == 0xFFFFFFFF
        || lightsPtr[i].MaterialEmissive == 0xFFFFFFFF
        || lightsPtr[i].MaterialDiffuse == 0xFFFFFFFF
        || lightsPtr[i].MaterialSpecular == 0xFFFFFFFF
        || lightsPtr[i].MaterialShininess == 0xFFFFFFFF
        || lightsPtr[i].Ambient == 0xFFFFFFFF) {
      std::cout << "No se puede inicializar la luz" << std::endl;
      exit(-1);
    }
  }
}

/**
 * Envia la unidad de textura.
 * @param TextureUnit Unidad de textura.
 */
void LightingTechnique::SetTextureUnit(unsigned int TextureUnit) {
  glUniform1i(m_gSampler, TextureUnit);
}

/**
 * Método que envia los atributos de la iluminación.
 * @param Light
 */
void LightingTechnique::SetAttrLight(LightAtrr * Light) {
  glUniform1i(gNumLights, numLights);
  for (int i = 0; i < numLights; i++) {
    glUniform4fv(lightsPtr[i].LightPosW, 1, glm::value_ptr(Light[i].lightPosW));
    glUniform4fv(lightsPtr[i].LightColor, 1,
                 glm::value_ptr(Light[i].lightColor));
    glUniform4fv(lightsPtr[i].MaterialEmissive, 1,
                 glm::value_ptr(Light[i].materialEmissive));
    glUniform4fv(lightsPtr[i].MaterialDiffuse, 1,
                 glm::value_ptr(Light[i].materialDiffuse));
    glUniform4fv(lightsPtr[i].MaterialSpecular, 1,
                 glm::value_ptr(Light[i].materialSpecular));
    glUniform1f(lightsPtr[i].MaterialShininess, Light[i].materialShininess);
    glUniform4fv(lightsPtr[i].Ambient, 1, glm::value_ptr(Light[i].ambient));
  }
}

/**
 * Setter para colocar el shader a utilizar y así poder enviarle la información.
 * @param shader
 */
void LightingTechnique::SetShader(Shader * shader) {
  this->shader = shader;
}

/**
 * Método que envia la posicion del punto de vista (Posición de la cámara).
 * @param EyeWorldPos
 */
void LightingTechnique::SetEyeWorldPos(const glm::vec3& EyeWorldPos) {
  glUniform4fv(eyePosW, 1, glm::value_ptr(glm::vec4(EyeWorldPos, 1)));
}
