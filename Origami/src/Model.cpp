#include "Headers/Model.h"

/**
 *
 * @param vertices Vertices del modelo.
 * @param length Tamaño del modelo.
 * @param strVertex Nombre del archivo del vertex Shader que se asocia al modelo.
 * @param strFragment Nombre del archivo Fragment Shader que se asocia al modelo.
 */
void Model::Initialize(Vertex3 vertices[], int length, std::string strVertex,
                       std::string strFragment) {

  shader.Initialize(strVertex, strFragment);

  Vertices = vertices;
  VerticesLength = length;

  // Reset de errores antes de hacer alguna operación.
  GLenum ErrorCheckValue = glGetError();

  glGenVertexArrays(1, &VertexArrayObjectId);
  glBindVertexArray(VertexArrayObjectId);

  glGenBuffers(1, &VertexBufferId);
  glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);

  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * length, Vertices,
  GL_STATIC_DRAW);

  glVertexAttribPointer(kVertexIndex, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertices[0]), 0);
  glVertexAttribPointer(kTextureIndex, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertices[0]), (GLvoid*) sizeof(Vertices[0].xyz));
  glVertexAttribPointer(
      kNormalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]),
      (GLvoid*) (sizeof(Vertices[0].xyz) + sizeof(Vertices[0].tex)));

  // Hardcord de la textura a utiizar.
  textura = new Texture::Texture(GL_TEXTURE_2D, "Texture/textura1.jpg");

  if (!textura->Load()) {
    fprintf(stderr, "ERROR: Could not load the texture: \n");
    exit(-1);
  }

  // Se revisan los errores.
  ErrorCheckValue = glGetError();

  if (ErrorCheckValue != GL_NO_ERROR) {
    fprintf(stderr, "ERROR: Could not create a VAO and VBO with error Id: %d\n",
            ErrorCheckValue);
    exit(-1);
  }

  if (lightAtrr != NULL) {
    lightingTechnique = new LightingTechnique(numLights);
    lightingTechnique->SetShader(&shader);
    lightingTechnique->Init();
  }

}

/**
 * Renderiza un modelo
 * @param parentModelMatrix Matrix de herencia, si no se requiere herencía
 * Se debe enviar una matriz unitaria.
 */
void Model::Render(mat4 parentModelMatrix) {
  // Enciende el shader con el que fue creado ese modelo.
  shader.TurnOn();

  // Guarda la matriz de proyección de la cámara que esta asociada al modelo.
  mat4 projectionMatrix = camera->GetProjectionMatrix();
  mat4 viewMatrix = camera->GetViewMatrix();

  // Se crea la matriz del modelo.
  mat4 modelMatrix = translate(parentModelMatrix, Position);

  // Se agrega la matriz de escalamiento.
  modelMatrix = scale(modelMatrix, Scale);

  // Se agrega la matriz de rotación del modelo para cada eje.
  modelMatrix = rotate(modelMatrix, Rotation.x, vec3(1, 0, 0));  // x-axis
  modelMatrix = rotate(modelMatrix, Rotation.y, vec3(0, 1, 0));  // y-axis
  modelMatrix = rotate(modelMatrix, Rotation.z, vec3(0, 0, 1));  // z-axis

  // Obtiene los identificadores del shader para cada matriz.
  GLint modelMatrixId = shader.GetVariable("modelMatrix");
  GLint viewMatrixId = shader.GetVariable("viewMatrix");
  GLint projectionMatrixId = shader.GetVariable("projectionMatrix");

  // Se envia las matrices al shader.
  shader.SetMatrix4(modelMatrixId, 1, false, &modelMatrix[0][0]);
  shader.SetMatrix4(viewMatrixId, 1, false, &viewMatrix[0][0]);
  shader.SetMatrix4(projectionMatrixId, 1, false, &projectionMatrix[0][0]);

  // Se envian los atributo de ilumnación.
  if (lightAtrr != NULL) {
    lightingTechnique->SetAttrLight(lightAtrr);
    lightingTechnique->SetEyeWorldPos(camera->GetPosition());
  }

  textura->Bind(GL_TEXTURE0);

  // Se le indica a OpenGL con que VAO se van a enlazar los datos.
  glBindVertexArray(VertexArrayObjectId);

  // Se habilitan la información de los vertices en el shader.
  glEnableVertexAttribArray(kVertexIndex);

  // Se habilitan la información de lo color en el shader.
  glEnableVertexAttribArray(kTextureIndex);

  // Se habilitan la información de las normales en el shader.
  glEnableVertexAttribArray(kNormalIndex);

  /*gSampler = GetVariable("gSampler");
   glUniform1i(gSampler, 0);*/

  // Dibuja la primitiva del VBO.
  glDrawArrays(GL_TRIANGLES, 0, VerticesLength);

  // Se deshabilitan los atributos de vertices, textura y normales.
  glDisableVertexAttribArray(kColorIndex);
  glDisableVertexAttribArray(kTextureIndex);
  glDisableVertexAttribArray(kNormalIndex);

  // Reset del VAO enlazado a los datos.
  glBindVertexArray(0);

  //Apagamos el shader asociado al modelo.
  shader.TurnOff();
}

/**
 * Obtiene una variable uniform del shader que esta asociado al modelo.
 * @param strVariable Nombre de variable.
 * @return  Indice de la variable.
 */
GLint Model::GetVariable(std::string strVariable) {
  return shader.GetVariable(strVariable);
}

/**
 * Libera lo VBOs y VAOs que utiliza el modelo.
 */
void Model::Destroy() {
  // Libera el VBO de vertices.
  if (VertexBufferId) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VertexBufferId);
    VertexBufferId = 0;
  }

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
