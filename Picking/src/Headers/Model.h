#ifndef _ModelManager_H
#define _ModelManager_H

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
using namespace glm;

/**
 * Clase que almacena los atributos de los vertices
 */
class Vertex3 {
 public:
  /**
   * Posición.
   */
  vec3 xyz;
  //float rgba[4];
  /**
   * Coordenadas de textura.
   */
  vec2 tex;
  /**
   * Vector normal.
   */
  vec3 normal;
};

/**
 * La clase modelo maneja la inicialización, renderizado y destrucción del objetos
 */
class Model {
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
   * Constructor, inicializa el modelo con un tipo de primitiva y el archivo de textura.
   * @param mode Primitva de renderización.
   * @param FileName Nombre de textura.
   */
  Model(GLenum mode, std::string FileName) {
    Scale = vec3(1.0f, 1.0f, 1.0f);
    this->mode = mode;
    this->FileName = FileName;
  }
  /**
   * Libera la memoria que utiliza el modelo.
   */
  ~Model() {
    Destroy();
  }

  /**
   * Inicialización del modelo, si antes de la inicialización se le pasa un Objeto Mesh::Mesh,
   * la inicialización la delegara a Assimp.
   * Del mismo modo si se le settea antes de la llamada a este metodo luces, valida que los shaders
   * este configurados para la iluminación.
   * @param vertices Vertices del modelo.
   * @param IndexData Indices del modelo.
   * @param length Tamaño de Vertices.
   * @param indexLength Tamaño de indices.
   * @param strVertex Nombre del archivo del vertex Shader que se asocia al modelo.
   * @param strFragment Nombre del archivo Fragment Shader que se asocia al modelo.
   */
  void Initialize(Vertex3 vertices[], GLuint IndexData[], int length,
                  int indexLength, std::string strVertex,
                  std::string strFragment);

  /**
   * Renderiza un modelo
   * @param parentModelMatrix Matrix de herencia, si no se requiere herencía
   * Se debe enviar una matriz unitaria.
   */
  void Render(mat4 parentModelMatrix);

  /**
   * Obtiene la posición del modelo.
   * @return
   */
  vec3 GetPosition() {
    return Position;
  }
  /**
   * Setter de la posición del modelo.
   * @param position Posición del modelo.
   */
  void SetPosition(vec3 position) {
    Position = position;
  }

  /**
   * Obtiene la rotación del modelo.
   * @return Rotación del modelo.
   */
  vec3 GetRotation() {
    return Rotation;
  }
  /**
   * Setter para la rotación del modelo.
   * @param rotation Rotación del modelo.
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
   * Obtiene la cámara y tener acceso a las matrices de vista y proyección.
   * @return
   */
  Camera *GetCamera() {
    return camera;
  }
  /**
   * Settea una cámara al modelo.
   * @param pCamera cámara del modelo.
   */
  void SetCamera(Camera *pCamera) {
    camera = pCamera;
  }

  /**
   * Setter de los parametros de iluminación para el objeto.
   * @param lightAtrr Atributos de iluminación.
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

  /**
   * Libera lo VBOs y VAOs que utiliza el modelo.
   */
  void Destroy();

  /**
   * Obtiene los indices asociados al modelo.
   * @return Indices del modelo.
   */
  GLuint* getIndexData() const {
    return IndexData;
  }

  /**
   * Obtiene el tamaño del arreglo de indices.
   * @return Tamaño de arreglo de indices.
   */
  GLint getIndexLength() const {
    return IndexLength;
  }

  /**
   * Obtiene los vertices del modelo.
   * @return Vertices del modelo.
   */
  const Vertex3* getVertices() const {
    return Vertices;
  }

  /**
   * Obtiene el tamaño del arreglo de vertices.
   * @return Tamaño de arreglo de vertices.
   */
  GLint getVerticesLength() const {
    return VerticesLength;
  }

  /**
   * Setter del numero de luces que estan asociados al modelo.
   * @param numLights Número de luces.
   */
  void setNumLights(int numLights) {
    this->numLights = numLights;
  }

 protected:

  Vertex3 *Vertices;
  GLuint *IndexData;
  GLint VerticesLength;
  GLint IndexLength;
  GLuint VertexBufferId;
  GLuint IndexDataId;
  GLuint VertexArrayObjectId;

  // Vectores de transformación del objeto.
  vec3 Position;
  vec3 Rotation;
  vec3 Scale;

  Shader shader;

  LightAtrr * lightAtrr = NULL;
  LightingTechnique * lightingTechnique;

  Camera *camera;

  Texture::Texture * textura = NULL;
  GLuint gSampler;

  Mesh::Mesh * mesh = NULL;

  GLenum mode;
  std::string FileName;

  int numLights;

};

#endif
