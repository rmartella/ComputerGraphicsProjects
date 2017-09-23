/*
 * ObjectCreation.h
 *
 *  Created on: 18/05/2015
 *      Author: rey
 */

#ifndef OBJECTCREATION_H_
#define OBJECTCREATION_H_

#include <vector>
#include "Model.h"

/**
 * Macros para comprar dos numeros.
 */
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

/**
 * Clase que maneja la creación de los objetos que contendra la escena.
 */
class ObjectCreation {
 public:
  /**
   * Constructor.
   */
  ObjectCreation();
  /**
   * Obtiene el tamaño de una malla.
   * @param meshEntryesPtr Apuntador al vector que contiene la malla.
   * @param sizeVertexMesh Tamaño de vertices de la malla.
   * @param sizeIndexMesh Tamaño de indices de la malla.
   */
  void getSizeMesh(std::vector<Mesh::Mesh::MeshEntry> * meshEntryesPtr,
                   int * sizeVertexMesh, int * sizeIndexMesh);
  /**
   * Convierte la malla en un formato para poder iterar sobre este.
   * @param meshEntryesPtr Apuntador al vector que contiene la malla.
   * @param sizeVertexMesh Tamaño de vertices de la malla.
   * @param sizeIndexMesh Tamaño de indices de la malla.
   * @param verticesZ Vector que almacena los nuevos vertices formato comun de los mallados.
   * @param index Indices de la malla.
   */
  void formatMesh(std::vector<Mesh::Mesh::MeshEntry> * meshEntryesPtr,
                  int sizeVertexMesh, int sizeIndexMesh, Vertex3 * verticesZ,
                  GLuint * index);
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
  Vertex3 * createModelTransform(glm::mat4 transformMatrix, Vertex3 * vertices,
                                 Vertex3 * verticesBoundingBox, int sizeVertex);
  /**
   * Genera los vetices de una esfera.
   * @param radius Radio de la esfera.
   * @param slices Revanadas horizontales.
   * @param stacks Revanadas Verticales.
   * @param vertices Apuntador donde se almacenan por referencia los vertices.
   * @param indices Apuntador donde se almacenan por referencia los indices.
   */
  void SolidSphere(float radius, int slices, int stacks, Vertex3 * vertices,
                   GLuint * indices);
  /**
   * Método que crea el modelo del cubo con vector normales apuntando al interior de la caja (Environment).
   * @param verticesEnvironmentCube Vertices del cubo de ambiente.
   * @param indexEnvironment Indicies del cubo de ambiente.
   * @param sizeVertex Tamaño de arreglo de vertices.
   * @param sizeIndex Tamaño de arreglo de indices.
   */
  void CubeEnviornment(Vertex3 * verticesEnvironmentCube,
                       GLuint * indexEnvironment, int sizeVertex,
                       int sizeIndex);
  /**
   * Método que crea el modelo del cubo.
   * @param verticesCube Vertices del cubo.
   * @param indexCube Indicies del cubo.
   * @param sizeVertex Tamaño de arreglo de vertices.
   * @param sizeIndex Tamaño de arreglo de indices.
   */
  void Cube(Vertex3 * verticesCube, GLuint * indexCube, int sizeVertex,
            int sizeIndex);
  /**
   * Método que obtiene las caras que son visibles u ocultas al observador.
   * @param vertices Vertices de objeto.
   * @param index Indices de objeto.
   * @param sizeIndex Tamaño de arreglo de indices.
   * @param camaraPosition Posición de la camara.
   * @return
   */
  bool * createCullFacesPointer(Vertex3 * vertices, GLuint * index,
                                int sizeIndex, glm::vec3 camaraPosition);
  /**
   * Destructor.
   */
  virtual ~ObjectCreation();
};

#endif /* OBJECTCREATION_H_ */
