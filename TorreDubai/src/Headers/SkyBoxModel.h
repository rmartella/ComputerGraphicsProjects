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
   * @param mode Primitva de renderizaci�n.
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
  void Initialize(const std::string& Directory, const std::string& PosXFilename,
                  const std::string& NegXFilename,
                  const std::string& PosYFilename,
                  const std::string& NegYFilename,
                  const std::string& PosZFilename,
                  const std::string& NegZFilename, std::string strVertex,
                  std::string strFragment);

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
   * Renderiza el modelo del skybox.
   * @param parentModelMatrix Matrix de herencia, si no se requiere herenc�a
   * Se debe enviar una matriz unitaria.
   */
  void Render(glm::mat4 rootMatrix);

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
  // Vectores de transformaci�n del objeto.
  vec3 Position;
  vec3 Rotation;
  vec3 Scale;
  Mesh::Mesh* m_pMesh;
  Shader shader;
  Camera *camera;
  CubemapTexture * cubemapTexture;
};

#endif /* SKYBOXMODEL_H_ */
