/*
 * RayTracing.h
 *
 *  Created on: 19/04/2015
 *      Author: rey
 */
#include <vector>
#include "Model.h"
#include "Main.h"
#include "Util.h"
#include <iostream>

#ifndef PICKING_H_
#define PICKING_H_

/**
 * Enum para el typo de boundig box que envuelce al objeto.
 */
enum TypeObjectBox {
  Box,  //!< Box
  Sphere  //!< Sphere
};

/**
 * Estructura que sirve para encapsular una prueba de intersecci�n.
 */
struct TestObjectHit {
  Vertex3 * vertexBB;
  GLuint * indexBB;
  int sizeIndexBB;
  TypeObjectBox typeObjectBox;
  glm::vec3 * vSphereCenter;
  float sphereRadius;
};

namespace RayCasting {

/**
 * Clase para manejar el trazado de rayos, tiene funciones como obtener rayos, intersecci�n con esferas, triangulos
 * Ademas asigna la escala de gris dependiendo de la profundidad.
 */
class RayCasting {
 public:
  /**
   * Constructor default.
   */
  RayCasting();
  /**
   * Setter para la c�mara, se utiliza al momento de obtener el rayo,
   * y que este este en coordenadas del modelo.
   * @param camera
   */
  void SetCamera(Camera * camera) {
    this->camera = camera;
  }
  /**
   * M�todo que obtiene las corrdenadas del origen y destino del rayo,
   * cooresponden a las coordenadas del rayo en el plano de recorte,
   * plano cercano y plano lejano, las coordenadas en el WorldSpace
   * es decir lo que esta viendo el observador.
   * @param x Coordenada en x, en el espacio de la pantalla.
   * @param y Coordenada en y, en el espacio de la pantalla.
   * @param ScreenWidth Ancho de ventana.
   * @param ScreenHeight Altura de ventana.
   * @param v1 Apuntador a la coordenada del origen del rayo (Near Plane).
   * @param v2 Apuntador a la coordenada del destino del rayo (Far Plane).
   */
  void Get3DRayUnderScrean(float x, float y, int ScreenWidth, int ScreenHeight,
                           glm::vec3* v1, glm::vec3* v2);
  /**
   *M�todo que obtiene la intersecci�n del rayo con un tri�ngulo,
   *Se utiliza el algortimo de Tomas Moller.
   *  Representaci�n del rayo en coordenadas parametricas.
   *    R ( t ) = O + tD  ------- (1)
   *  Un punto de un tri�ngulo esta dado por las coordenadas varicentricas,
   *  del triangulo.
   *    T ( u v ) = (1 ; u ; v ) V 0 + uV 1 + vV 2  ------- (2)
   * @param orig Origen del rayo.
   * @param dir Direcci�n del rayo.
   * @param vert0 Vertice 1 del triangulo a probar.
   * @param vert1 Vertice 2 del triangulo a probar.
   * @param vert2 Vertice 3 del triangulo a probar.
   * @param t Valor de ecuaci�n de (1)
   * @param u Coordenada u varicentrica del tri�ngulo (2).
   * @param v Coordenada v varicentrica del tri�ngulo (2).
   * @return Bandera que indica si hay intersecci�n o no, true existe la intersecci�n
   * false no existe la intersecci�n.
   */
  bool RayTriangleIntersect(glm::vec3 * orig, glm::vec3 * dir,
                            glm::vec3 * vert0, glm::vec3 *vert1,
                            glm::vec3 * vert2, double *t, double *u, double *v);
  /**
   * M�todo que prueba si una linea interseca con una esfera.
   * @param orig Origen del rayo.
   * @param dest Punto final del rayo.
   * @param dir Direcci�n del rayo.
   * @param vSphereCenter Centro de la esfera.
   * @param sphereRadius Radio de la esfera.
   * @return Bandera que indica si existe intersecci�n de rayo con la esfera,
   * true existe la intersecci�n false no existe la intersecci�n.
   */
  bool RaySphereIntersect(glm::vec3 * orig, glm::vec3 * dest, glm::vec3 * dir,
                          glm::vec3 * vSphereCenter, float sphereRadius);
  /**
   * M�todo que asigna la escala de grises de los vertices de un triangulo.
   * @param t distancia a la que se encuentra el objeto.
   * @param vertice Apuntador a vertice de la cara.
   * @param index Apuntador a indice de la cara.
   */
  void assignGrayScaleValue(double t, Vertex3 * vertice, int index);
  /**
   * M�todo que obtiene al intersecci�n de un rayo con el triangulo.
   * @param origRay Origen.
   * @param direction Direcci�n.
   * @param VP ViewMatriz * ProyectionMatriz.
   * @param vertices Vertices.
   * @param verticesTransform Vertices transformados.
   * @param index Indices.
   * @param sizeIndex Tama�o de indices.
   * @param cullFacesPointer Apuntador a las caras.
   */
  void iterateObjectTriangle(glm::vec3 * origRay, glm::vec3 * direction,
                             glm::mat4 * VP, Vertex3 * vertices,
                             Vertex3 * verticesTransform, GLuint * index,
                             int sizeIndex, bool * cullFacesPointer);
  /**
   * M�todo que prueba si el objeto puede ser alcanzado por el rayo.
   * utiliza el bounding box.
   * @param origRay Origen del rayo.
   * @param destRay Punto final de rayo.
   * @param direction Direcci�n de rayo.
   * @param testObjectHit  Objeto para probar
   * @return Bandera que indica si el rayo alcanza al objeto.
   */
  bool hitObjectTest(glm::vec3 * origRay, glm::vec3 * destRay,
                     glm::vec3 * direction, TestObjectHit * testObjectHit);
  /**
   * Encapsula el objeto a probar con el rayo en una estructura, para no pasarle
   * cada cosa por separado al m�todo de intersecci�n.
   * @param vertexBB Vertices el Bounding Box.
   * @param indexBB Indices del Boundig Box.
   * @param sizeIndexBB Tama�o de Indices del Boundig Box.
   * @param typeObjectBox Tupo de Boundig Box.
   * @param vSphereCenter Centro de la esfera si es una Sphere Box
   * @param sphereRadius Radio de la esfera si es una Sphere Box
   * @return Objeto para porbar intersecci�n.
   */
  TestObjectHit * createTestObjectHit(Vertex3 * vertexBB, GLuint * indexBB,
                                      int sizeIndexBB,
                                      TypeObjectBox typeObjectBox,
                                      glm::vec3 * vSphereCenter,
                                      float sphereRadius);
  /**
   * M�todo que encuentra la profundida que se encuentra un objeto.
   * @param mousePosition Posici�n del mouse espacio de la ventana.
   * @param vertices Arreglo de vertices transformados al modelo de la escena.
   * @param index Indices de los vertices.
   * @param sizeIndex Tama�o de indices.
   * @param ScreenWidth Ancho de ventana.
   * @param ScreenHeight Alto de ventana.
   * @param VP ViewMatriz * ProyectionMatriz.
   * @param testHit Encamsulamiento de objeto para hacer la prueba con el Bounding Box.
   * @param cullFacesPointer Apuntador a caras visibles y no visibles.
   * @return Profunidad minima a la que se encuentra el objeto, regresa -1 si no encuentra intersecci�n.
   */
  float getDistanceScaleValue(glm::ivec2 * mousePosition, Vertex3 * vertices,
                              GLuint * index, int sizeIndex, int ScreenWidth,
                              int ScreenHeight, glm::mat4 * VP,
                              TestObjectHit * testHit, bool * cullFacesPointer);

  // Coordenadas iniciales de la ventana en pixeles.
  int xBottom, yBottom, xTop, yTop;
  /**
   * Destructor de clase.
   */
  virtual ~RayCasting();

  /**
   * Obtiene el offset de color.
   * @return Offset.
   */
  float getOffset() const {
    return offset;
  }

  /**
   * Setter del desplazamiento.
   * @param offset Desplazamiento.
   */
  void setOffset(float offset) {
    this->offset = offset;
  }

  /**
   * Obiene la escala de intensidad.
   * @return Escala de intensidad.
   */
  float getScale() const {
    return scale;
  }

  /**
   * Setter para la escala de intensidad.
   * @return Escala de intensidad.
   */
  void setScale(float scale) {
    this->scale = scale;
  }

  bool isContrast() const {
    return contrast;
  }

  void setContrast(bool contrast) {
    this->contrast = contrast;
  }

 private:
  Camera * camera;
  bool contrast;
  float offset;
  float scale;
};

} /* namespace Picking */
#endif /* PICKING_H_ */
