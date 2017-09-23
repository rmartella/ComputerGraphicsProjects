/*
 * SkyBoxModel.h
 *
 *  Created on: 15/05/2015
 *      Author: rey
 */

#ifndef SKYBOXMODEL_H_
#define SKYBOXMODEL_H_

#include <string.h>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "CubemapTexture.h"

using namespace glm;

/**
 * Clase encarga da crear el modelo del skybox, inicializar, renderizar y liberar memoria
 * del skybox de la escena.
 */
class SkyBoxModel {
 public:
  /**
   * Constructor, inicializa el modelo con un tipo de primitiva y el archivo de textura.
   * @param mode Primitva de renderización.
   * @param FileName Nombre de textura.
   */
  SkyBoxModel() {
  }
  /**
   * Libera la memoria que utiliza el modelo.
   */
  ~SkyBoxModel() {
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
  void Initialize(const std::string& Directory, const std::string& PosXFilename,
                  const std::string& NegXFilename,
                  const std::string& PosYFilename,
                  const std::string& NegYFilename,
                  const std::string& PosZFilename,
                  const std::string& NegZFilename, std::string strVertex,
                  std::string strFragment);

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
   * Renderiza el modelo del skybox.
   * @param parentModelMatrix Matrix de herencia, si no se requiere herencía
   * Se debe enviar una matriz unitaria.
   */
  void Render(glm::mat4 rootMatrix);

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
   * Obtiene la textura con la que se creo el skybox.
   * @return
   */
  CubemapTexture * GetCubeTextureMap() {
    return this->cubemapTexture;
  }

  /**
   * Libera lo VBOs y VAOs que utiliza el modelo.
   */
  void Destroy();

 private:
  GLuint VertexArrayObjectId;
  // Vectores de transformación del objeto.
  vec3 Position;
  vec3 Rotation;
  vec3 Scale;
  Mesh::Mesh* m_pMesh;
  Shader shader;
  Camera *camera;
  CubemapTexture * cubemapTexture;
};

#endif /* SKYBOXMODEL_H_ */
