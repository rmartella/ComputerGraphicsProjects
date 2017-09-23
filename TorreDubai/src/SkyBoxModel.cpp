/*
 * SkyBoxModel.cpp
 *
 *  Created on: 15/05/2015
 *      Author: rey
 */

#include "Headers/SkyBoxModel.h"

/**
 * Clase encarga da crear el modelo del skybox, inicializar, renderizar y liberar memoria
 * del skybox de la escena.
 */
void SkyBoxModel::Initialize(const std::string& Directory,
                             const std::string& PosXFilename,
                             const std::string& NegXFilename,
                             const std::string& PosYFilename,
                             const std::string& NegYFilename,
                             const std::string& PosZFilename,
                             const std::string& NegZFilename,
                             std::string strVertex, std::string strFragment) {
  shader.Initialize(strVertex, strFragment);

  // Reset de errores antes de hacer alguna operación.
  GLenum ErrorCheckValue = glGetError();

  shader.TurnOn();
  GLint m_textureLocation = shader.GetVariable("gCubemapTexture");
  glUniform1i(m_textureLocation, 0);

  cubemapTexture = new CubemapTexture(Directory, PosXFilename, NegXFilename,
                                      PosYFilename, NegYFilename, PosZFilename,
                                      NegZFilename);

  if (!cubemapTexture->Load()) {
    fprintf(stderr, "ERROR: Could not create a CubeTextureMap\n");
    exit(-1);
  }

  glGenVertexArrays(1, &VertexArrayObjectId);
  glBindVertexArray(VertexArrayObjectId);
  m_pMesh = new Mesh::Mesh("Models/sphere.obj");
  m_pMesh->LoadMesh();

  // Se revisan los errores.
  ErrorCheckValue = glGetError();

  if (ErrorCheckValue != GL_NO_ERROR) {
    fprintf(stderr, "ERROR: Could not create a VAO and VBO with error Id: %d\n",
            ErrorCheckValue);
    exit(-1);
  }
  //Apagamos el shader asociado al modelo.
  shader.TurnOff();
}

/**
 * Renderiza el modelo del skybox.
 * @param parentModelMatrix Matrix de herencia, si no se requiere herencía
 * Se debe enviar una matriz unitaria.
 */
void SkyBoxModel::Render(glm::mat4 rootMatrix) {
  // Enciende el shader con el que fue creado ese modelo.
  shader.TurnOn();

  GLint OldCullFaceMode;
  glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
  GLint OldDepthFuncMode;
  glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

  glCullFace(GL_FRONT);
  glDepthFunc(GL_LEQUAL);

  // Guarda la matriz de proyección de la cámara que esta asociada al modelo.
  mat4 projectionMatrix = camera->GetProjectionMatrix();
  mat4 viewMatrix = camera->GetViewMatrix();

  // Se crea la matriz del modelo.
  mat4 modelMatrix = translate(rootMatrix, Position);

  // Se agrega la matriz de escalamiento.
  modelMatrix = scale(modelMatrix, Scale);

  // Se agrega la matriz de rotación del modelo para cada eje.
  modelMatrix = rotate(modelMatrix, Rotation.x, vec3(1, 0, 0));  // eje x
  modelMatrix = rotate(modelMatrix, Rotation.y, vec3(0, 1, 0));  // eje y
  modelMatrix = rotate(modelMatrix, Rotation.z, vec3(0, 0, 1));  // eje z

  // Obtiene los identificadores del shader para cada matriz.
  GLint modelMatrixId = shader.GetVariable("modelMatrix");
  GLint viewMatrixId = shader.GetVariable("viewMatrix");
  GLint projectionMatrixId = shader.GetVariable("projectionMatrix");

  // Se envia las matrices al shader.
  shader.SetMatrix4(modelMatrixId, 1, false, &modelMatrix[0][0]);
  shader.SetMatrix4(viewMatrixId, 1, false, &viewMatrix[0][0]);
  shader.SetMatrix4(projectionMatrixId, 1, false, &projectionMatrix[0][0]);

  // Unidad de textura.
  GLint m_textureLocation = shader.GetVariable("gCubemapTexture");
  glUniform1i(m_textureLocation, 0);

  // Bind con la unidad de textura.
  glBindVertexArray(VertexArrayObjectId);
  cubemapTexture->Bind(GL_TEXTURE0);
  m_pMesh->Render();
  glBindVertexArray(0);

  glCullFace(OldCullFaceMode);
  glDepthFunc(OldDepthFuncMode);

  //Apagamos el shader asociado al modelo.
  shader.TurnOff();
}

/**
 * Libera lo VBOs y VAOs que utiliza el modelo.
 */
void SkyBoxModel::Destroy() {
  delete cubemapTexture;

  // Libera los VAOs
  if (VertexArrayObjectId) {
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VertexArrayObjectId);
    VertexArrayObjectId = 0;
  }
  camera = nullptr;

  // Libera memoria que usan los shaders.
  shader.Destroy();
}
