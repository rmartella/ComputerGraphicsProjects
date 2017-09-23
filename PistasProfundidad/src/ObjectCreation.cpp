/*
 * ObjectCreation.cpp
 *
 *  Created on: 18/05/2015
 *      Author: rey
 */

#include "Headers/ObjectCreation.h"

/**
 * Constructor.
 */
ObjectCreation::ObjectCreation() {

}

/**
 * Destructor.
 */
ObjectCreation::~ObjectCreation() {
}

/**
 * Obtiene el tamaño de una malla.
 * @param meshEntryesPtr Apuntador al vector que contiene la malla.
 * @param sizeVertexMesh Tamaño de vertices de la malla.
 * @param sizeIndexMesh Tamaño de indices de la malla.
 */
void ObjectCreation::getSizeMesh(
    std::vector<Mesh::Mesh::MeshEntry> * meshEntryesPtr, int * sizeVertexMesh,
    int * sizeIndexMesh) {
  for (unsigned int k = 0; k < meshEntryesPtr->size(); k++) {
    *sizeVertexMesh += (*meshEntryesPtr)[k].NumIndices;
    *sizeIndexMesh += (*meshEntryesPtr)[k].NumIndices;
  }
}

/**
 * Convierte la malla en un formato para poder iterar sobre este.
 * @param meshEntryesPtr Apuntador al vector que contiene la malla.
 * @param sizeVertexMesh Tamaño de vertices de la malla.
 * @param sizeIndexMesh Tamaño de indices de la malla.
 * @param verticesZ Vector que almacena los nuevos vertices formato comun de los mallados.
 * @param index Indices de la malla.
 */
void ObjectCreation::formatMesh(
    std::vector<Mesh::Mesh::MeshEntry> * meshEntryesPtr, int sizeVertexMesh,
    int sizeIndexMesh, Vertex3 * verticesZ, GLuint * index) {

  for (unsigned int k = 0; k < meshEntryesPtr->size(); k++) {
    for (unsigned int l = 0; l < (*meshEntryesPtr)[k].NumIndices; l += 3) {
      index[l] = (*meshEntryesPtr)[k].Indices[l];
      verticesZ[index[l]].xyz =
          (*meshEntryesPtr)[k].Vertices[(*meshEntryesPtr)[k].Indices[l]].m_pos;

      index[l + 1] = (*meshEntryesPtr)[k].Indices[l + 1];
      verticesZ[index[l + 1]].xyz =
          (*meshEntryesPtr)[k].Vertices[(*meshEntryesPtr)[k].Indices[l + 1]]
              .m_pos;

      index[l + 2] = (*meshEntryesPtr)[k].Indices[l + 2];
      verticesZ[index[l + 2]].xyz =
          (*meshEntryesPtr)[k].Vertices[(*meshEntryesPtr)[k].Indices[l + 2]]
              .m_pos;
    }
  }
}

/**
 * Transforma los vertices de una malla para manejar los objetos en su propio espacio,
 * tambien crea su forma envolvente.
 * @param transformMatrix Matrix de transformación.
 * @param vertices Vertices de la malla.
 * @param verticesBoundingBox Vertices del mallado de la forma envolvente, si se le pasa un
 * apuntador nullo no crea la caja envolvente (Esferas).
 * @param sizeVertex Tamaño del arreglo de vertices.
 * @return
 */
Vertex3 * ObjectCreation::createModelTransform(glm::mat4 transformMatrix,
                                               Vertex3 * vertices,
                                               Vertex3 * verticesBoundingBox,
                                               int sizeVertex) {
  Vertex3 * modelTransform = (Vertex3 *) malloc(sizeof(Vertex3) * sizeVertex);
  float zmin, zmax, ymin, ymax, xmin, xmax;
  for (int i = 0; i < sizeVertex; i++) {
    modelTransform[i].xyz = glm::vec3(
        transformMatrix * glm::vec4(vertices[i].xyz, 1.0f));
    if (i == 0) {
      xmin = modelTransform[0].xyz.x;
      xmax = modelTransform[0].xyz.x;
      ymin = modelTransform[0].xyz.y;
      ymax = modelTransform[0].xyz.y;
      zmin = modelTransform[0].xyz.z;
      zmax = modelTransform[0].xyz.z;
    }
    xmin = aisgl_min(xmin, modelTransform[i].xyz.x);
    xmax = aisgl_max(xmax, modelTransform[i].xyz.x);
    ymin = aisgl_min(ymin, modelTransform[i].xyz.y);
    ymax = aisgl_max(ymax, modelTransform[i].xyz.y);
    zmin = aisgl_min(zmin, modelTransform[i].xyz.z);
    zmax = aisgl_max(zmax, modelTransform[i].xyz.z);
    vertices[i].rgba = glm::vec4(-1.0);
  }
  /*printf("xmin %f xmax %f\n", xmin, xmax);
   printf("ymin %f ymax %f\n", ymin, ymax);
   printf("zmin %f zmax %f\n", zmin, zmax);*/
  if (verticesBoundingBox != nullptr) {
    verticesBoundingBox[0] = Vertex3 { glm::vec3(xmin, ymax, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[1] = Vertex3 { glm::vec3(xmin, ymin, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[2] = Vertex3 { glm::vec3(xmax, ymin, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[3] = Vertex3 { glm::vec3(xmax, ymax, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };

    verticesBoundingBox[4] = Vertex3 { glm::vec3(xmax, ymax, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[5] = Vertex3 { glm::vec3(xmax, ymin, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[6] = Vertex3 { glm::vec3(xmax, ymin, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[7] = Vertex3 { glm::vec3(xmax, ymax, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };

    verticesBoundingBox[8] = Vertex3 { glm::vec3(xmin, ymax, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[9] = Vertex3 { glm::vec3(xmin, ymax, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[10] = Vertex3 { glm::vec3(xmin, ymin, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[11] = Vertex3 { glm::vec3(xmin, ymin, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };

    verticesBoundingBox[12] = Vertex3 { glm::vec3(xmin, ymax, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[13] = Vertex3 { glm::vec3(xmax, ymax, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[14] = Vertex3 { glm::vec3(xmax, ymax, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[15] = Vertex3 { glm::vec3(xmin, ymax, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };

    verticesBoundingBox[16] = Vertex3 { glm::vec3(xmin, ymax, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[17] = Vertex3 { glm::vec3(xmax, ymax, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[18] = Vertex3 { glm::vec3(xmax, ymin, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[19] = Vertex3 { glm::vec3(xmin, ymin, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };

    verticesBoundingBox[20] = Vertex3 { glm::vec3(xmin, ymin, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[21] = Vertex3 { glm::vec3(xmin, ymin, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[22] = Vertex3 { glm::vec3(xmax, ymin, zmin), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
    verticesBoundingBox[23] = Vertex3 { glm::vec3(xmax, ymin, zmax), glm::vec4(
        0.4, 0.7, 0.8, 0.0) };
  }

  return modelTransform;
}
/**
 * Método que obtiene las caras que son visibles u ocultas al observador.
 * @param vertices Vertices de objeto.
 * @param index Indices de objeto.
 * @param sizeIndex Tamaño de arreglo de indices.
 * @param camaraPosition Posición de la camara.
 * @return
 */
bool * ObjectCreation::createCullFacesPointer(Vertex3 * vertices,
                                              GLuint * index, int sizeIndex,
                                              glm::vec3 camaraPosition) {
  int sizeCullFacesPointer = sizeIndex / 3;
  int indexFace = 0;
  bool * cullFacesPtr = new bool[sizeCullFacesPointer];
  for (int i = 0; i < sizeIndex; i += 3) {
    glm::vec3 v1 = vertices[index[i + 1]].xyz - vertices[index[i]].xyz;
    glm::vec3 v2 = vertices[index[i + 2]].xyz - vertices[index[i]].xyz;
    glm::vec3 normal = glm::cross(v1, v2);
    normal = glm::normalize(normal);
    if (glm::dot(normal, camaraPosition - vertices[index[i]].xyz) > 0)
      cullFacesPtr[indexFace] = true;
    else
      cullFacesPtr[indexFace] = false;
    indexFace++;
  }
  return cullFacesPtr;
}
/**
 * Genera los vetices de una esfera.
 * @param radius Radio de la esfera.
 * @param slices Revanadas horizontales.
 * @param stacks Revanadas Verticales.
 * @param vertices Apuntador donde se almacenan por referencia los vertices.
 * @param indices Apuntador donde se almacenan por referencia los indices.
 */
void ObjectCreation::SolidSphere(float radius, int slices, int stacks,
                                 Vertex3 * vertices, GLuint * indices) {
  // Uso local de name spaces.
  using namespace glm;
  using namespace std;

  const float pi = 3.1415926535897932384626433832795f;
  const float _2pi = 2.0f * pi;

  vector<vec3> positions;
  vector<vec3> normals;
  vector<vec2> textureCoords;

  for (int i = 0; i <= stacks; ++i) {
    // Coordenadas V de la textura.
    float V = i / (float) stacks;
    float phi = V * pi;

    for (int j = 0; j <= slices; ++j) {
      // Coordenadas U de la textura.
      float U = j / (float) slices;
      float theta = U * _2pi;

      float X = cos(theta) * sin(phi);
      float Y = cos(phi);
      float Z = sin(theta) * sin(phi);
      // Se calculan los vertices, UV, normales de la esfera.
      *(vertices + i * (slices + 1) + j) = {vec3(X, Y, Z) * radius, vec4(0, 0 , 0, 0), vec3(X, Y, Z)};
    }
  }

  // Generaicón de indices.
  for (int i = 0; i < slices * stacks + slices; ++i) {
    *(indices + i * 6) = i;
    *(indices + i * 6 + 1) = i + slices + 1;
    *(indices + i * 6 + 2) = i + slices;
    *(indices + i * 6 + 3) = i + slices + 1;
    *(indices + i * 6 + 4) = i;
    *(indices + i * 6 + 5) = i + 1;
  }
}
/**
 * Método que crea el modelo del cubo con vector normales apuntando al interior de la caja (Environment).
 * @param verticesEnvironmentCube Vertices del cubo de ambiente.
 * @param indexEnvironment Indicies del cubo de ambiente.
 * @param sizeVertex Tamaño de arreglo de vertices.
 * @param sizeIndex Tamaño de arreglo de indices.
 */
void ObjectCreation::CubeEnviornment(Vertex3 * verticesEnvironmentCube,
                                     GLuint * indexEnvironment, int sizeVertex,
                                     int sizeIndex) {
  Vertex3 verticesEnvironmentCubeTmp[] =
      {
          //Bottom
          Vertex3 { glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                              1.0f) },
          Vertex3 { glm::vec3(-5.0f, -5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) }, Vertex3 {
              glm::vec3(5.0f, -5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          Vertex3 { glm::vec3(5.0f, -5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) },
          //Back
          Vertex3 { glm::vec3(-5.0f, 5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) },
          Vertex3 { glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                              1.0f) },
          Vertex3 { glm::vec3(5.0f, -5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) }, Vertex3 {
              glm::vec3(5.0f, 5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          //Left
          Vertex3 { glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                              1.0f) },
          Vertex3 { glm::vec3(-5.0f, 5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) }, Vertex3 {
              glm::vec3(-5.0f, 5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          Vertex3 { glm::vec3(-5.0f, -5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) },
          //Right
          Vertex3 { glm::vec3(5.0f, -5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) }, Vertex3 {
              glm::vec3(5.0f, -5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          Vertex3 { glm::vec3(5.0f, 5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                           1.0f) }, Vertex3 {
              glm::vec3(5.0f, 5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          //TOP
          Vertex3 { glm::vec3(-5.0f, 5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) }, Vertex3 {
              glm::vec3(5.0f, 5.0f, -5.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          Vertex3 { glm::vec3(5.0, 5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                          1.0f) }, Vertex3 {
              glm::vec3(-5.0f, 5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          //TOP
          Vertex3 { glm::vec3(5.0f, 5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                           1.0f) }, Vertex3 {
              glm::vec3(5.0f, -5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          Vertex3 { glm::vec3(-5.0f, -5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) }, Vertex3 {
              glm::vec3(-5.0f, 5.0f, 5.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) } };

  GLuint indexEnvironmentTmp[] { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10,
      10, 11, 8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22,
      23, 20 };
  memcpy(verticesEnvironmentCube, verticesEnvironmentCubeTmp,
         sizeof(Vertex3) * sizeVertex);
  memcpy(indexEnvironment, indexEnvironmentTmp, sizeof(GLuint) * sizeIndex);
}
/**
 * Método que crea el modelo del cubo.
 * @param verticesCube Vertices del cubo.
 * @param indexCube Indicies del cubo.
 * @param sizeVertex Tamaño de arreglo de vertices.
 * @param sizeIndex Tamaño de arreglo de indices.
 */
void ObjectCreation::Cube(Vertex3 * verticesCube, GLuint * indexCube,
                          int sizeVertex, int sizeIndex) {
  Vertex3 verticesCubeTmp[] =
      {
          //Front face
          Vertex3 { glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                           1.0f) }, Vertex3 {
              glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          Vertex3 { glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) }, Vertex3 {
              glm::vec3(1.0, -1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          //BackFace
          Vertex3 { glm::vec3(1.0f, 1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                            1.0f) },
          Vertex3 { glm::vec3(1.0f, -1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) },
          Vertex3 { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                              1.0f) }, Vertex3 {
              glm::vec3(-1.0, 1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          // Right Face
          Vertex3 { glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                           1.0f) }, Vertex3 {
              glm::vec3(1.0f, -1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          Vertex3 { glm::vec3(1.0f, -1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) }, Vertex3 {
              glm::vec3(1.0, 1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          // Left Face
          Vertex3 { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                            1.0f) },
          Vertex3 { glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) },
          Vertex3 { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                              1.0f) }, Vertex3 {
              glm::vec3(-1.0, -1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          // Up Face
          Vertex3 { glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                           1.0f) }, Vertex3 {
              glm::vec3(1.0f, 1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          Vertex3 { glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) }, Vertex3 {
              glm::vec3(-1.0, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) },
          // Down Face
          Vertex3 { glm::vec3(1.0f, -1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                            1.0f) },
          Vertex3 { glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                             1.0f) },
          Vertex3 { glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f,
                                                              1.0f) }, Vertex3 {
              glm::vec3(1.0, -1.0f, -1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) } };
  GLuint indexCubeTmp[] { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11,
      8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20 };
  memcpy(verticesCube, verticesCubeTmp, sizeof(Vertex3) * sizeVertex);
  memcpy(indexCube, indexCubeTmp, sizeof(GLuint) * sizeIndex);
}

