#ifndef _ModelEnironmentManager_H
#define _ModelEnironmentManager_H

#define GLM_FORCE_RADIANS
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include "LightingTechnique.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "CubemapTexture.h"
#include "Model.h"
using namespace glm;

/**
 * Clase que representa el environment.
 */
class ModelEnvironment {
 public:

  /**
   *  Indice para vertices.
   */
  static const GLuint kVertexIndex = 0;
  /**
   * Indices para los colores.
   */
  static const GLuint kColorIndex = 1;
  /**
   * Indices para las texturas.
   */
  static const GLuint kTextureIndex = 2;
  /**
   * Indices para las normales.
   */
  static const GLuint kNormalIndex = 3;

  /**
   * Constructor.
   * @param mode Primitva de renderizaci�n.
   * @param FileName Nombre de textura.
   */
  ModelEnvironment() {
    Scale = vec3(1.0f, 1.0f, 1.0f);
  }
  /**
   * Libera la memoria que utiliza el modelo.
   */
  ~ModelEnvironment() {
    Destroy();
  }

  /**
   * Inicializaci�n del modelo, si antes de la inicializaci�n se le pasa un Objeto Mesh::Mesh,
   * la inicializaci�n la delegara a Assimp.
   * Del mismo modo si se le settea antes de la llamada a este metodo luces, valida que los shaders
   * este configurados para la iluminaci�n.
   * @param vertices Vertices del modelo.
   * @param IndexData Indices del modelo.
   * @param length Tama�o de Vertices.
   * @param indexLength Tama�o de indices.
   * @param strVertex Nombre del archivo del vertex Shader que se asocia al modelo.
   * @param strFragment Nombre del archivo Fragment Shader que se asocia al modelo.
   */
  void Initialize(Vertex3 vertices[], GLuint IndexData[], int length,
                  int indexLength, std::string strVertex,
                  std::string strFragment);

  /**
   * Renderiza un modelo
   * @param parentModelMatrix Matrix de herencia, si no se requiere herenc�a
   * Se debe enviar una matriz unitaria.
   */
  void Render(mat4 parentModelMatrix);

  /**
   * Obtiene la posici�n del modelo.
   * @return
   */
  vec3 GetPosition() {
    return Position;
  }
  /**
   * Setter de la posici�n del modelo.
   * @param position Posici�n del modelo.
   */
  void SetPosition(vec3 position) {
    Position = position;
  }

  /**
   * Obtiene la rotaci�n del modelo.
   * @return Rotaci�n del modelo.
   */
  vec3 GetRotation() {
    return Rotation;
  }
  /**
   * Setter para la rotaci�n del modelo.
   * @param rotation Rotaci�n del modelo.
   */
  void SetRotation(vec3 rotation) {
    Rotation = rotation;
  }

  /**
   * Obtiene la escala del objeto.
   * @return Escala de objeto.
   */
  vec3 GetScale() {
    return Scale;
  }
  /**
   * Setter de la escala del objeto.
   * @param Escala de objeto.
   */
  void SetScale(vec3 scale) {
    Scale = scale;
  }

  /**
   * Obtiene la c�mara y tener acceso a las matrices de vista y proyecci�n.
   * @return
   */
  Camera *GetCamera() {
    return camera;
  }
  /**
   * Settea una c�mara al modelo.
   * @param pCamera c�mara del modelo.
   */
  void SetCamera(Camera *pCamera) {
    camera = pCamera;
  }

  /**
   * Setter de los parametros de iluminaci�n para el objeto.
   * @param lightAtrr Atributos de iluminaci�n.
   */
  void SetLightAtrr(LightAtrr * lightAtrr) {
    this->lightAtrr = lightAtrr;
  }

  /**
   * Obtiene una variable uniform del shader que esta asociado al modelo.
   * @param strVariable Nombre de variable.
   * @return  Indice de la variable.
   */
  GLint GetVariable(std::string strVariable);

  /**
   * Libera lo VBOs y VAOs que utiliza el modelo.
   */
  void Destroy();

  /**
   * Setter del numero de luces que estan asociados al modelo.
   * @param numLights N�mero de luces.
   */
  void setNumLights(int numLights) {
    this->numLights = numLights;
  }

  /**
   * Setter Cube Texture.
   * @param cubemapTexture
   */
  void SetCubemapTexture(CubemapTexture * cubemapTexture) {
    this->cubemapTexture = cubemapTexture;
  }

  /**
   * Setter para la malla de un modelo.
   * @param mesh
   */
  void SetMesh(Mesh::Mesh * mesh) {
    this->mesh = mesh;
  }

  /**
   * Obtiene la malla que esta asociada al modelo.
   * @return Malla del modelo.
   */
  Mesh::Mesh * GetMesh() {
    return this->mesh;
  }

 protected:

  Vertex3 *Vertices;
  GLuint *IndexData;
  GLint VerticesLength;
  GLint IndexLength;
  GLuint VertexBufferId;
  GLuint IndexDataId;
  GLuint VertexArrayObjectId;

  // Vectores de transformaci�n del objeto.
  vec3 Position;
  vec3 Rotation;
  vec3 Scale;

  Shader shader;

  LightAtrr * lightAtrr = NULL;
  LightingTechnique * lightingTechnique;

  Camera *camera;
  Mesh::Mesh * mesh = NULL;
  CubemapTexture * cubemapTexture;
  int numLights;
};

#endif

