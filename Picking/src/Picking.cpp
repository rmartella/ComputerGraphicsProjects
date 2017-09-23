/*
 * Picking.cpp
 *
 *  Created on: 19/04/2015
 *      Author: rey
 */

#include "Headers/Picking.h"

#define EPSILON 0.000001

namespace Picking {

/**
 * Constructor default.
 */
Picking::Picking() {
}

/**
 * Método que obtiene las corrdenadas del origen y destino del rayo,
 * cooresponden a las coordenadas del rayo en el plano de recorte,
 * plano cercano y plano lejano, las coordenadas ya estan en espació del modelo,
 * al realizar las transformaciónes inversas de proyeción al modelo.
 * @param modelMatrix Matrix del modelo.
 * @param x Coordenada en x, en el espacio de la pantalla.
 * @param y Coordenada en y, en el espacio de la pantalla.
 * @param ScreenWidth Ancho de ventana.
 * @param ScreenHeight Altura de ventana.
 * @param v1 Apuntador a la coordenada del origen del rayo (Near Plane).
 * @param v2 Apuntador a la coordenada del destino del rayo (Far Plane).
 */
void Picking::Get3DRayUnderMouse(glm::mat4 modelMatrix, float x, float y,
                                 int ScreenWidth, int ScreenHeight,
                                 glm::vec3* v1, glm::vec3* v2) {
  glm::vec4 viewport = glm::vec4(0.0f, 0.0f, ScreenWidth, ScreenHeight);
  *v1 = glm::unProject(glm::vec3(x, y, 0.0f),
                       camera->GetViewMatrix() * modelMatrix,
                       camera->GetProjectionMatrix(), viewport);
  *v2 = glm::unProject(glm::vec3(x, y, 1.0f),
                       camera->GetViewMatrix() * modelMatrix,
                       camera->GetProjectionMatrix(), viewport);
}

/**
 *Método que obtiene la intersección del rayo con un triángulo,
 *Se utiliza el algortimo de Tomas Moller.
 *  Representación del rayo en coordenadas parametricas.
 *    R ( t ) = O + tD  ------- (1)
 *  Un punto de un triángulo esta dado por las coordenadas varicentricas,
 *  del triangulo, u >= 0, v >= 0 y u + v <= 1.
 *    T ( u v ) = (1 ; u ; v ) V 0 + uV 1 + vV 2  ------- (2)
 * @param orig Origen del rayo.
 * @param dir Dirección del rayo.
 * @param vert0 Vertice 1 del triangulo a probar.
 * @param vert1 Vertice 2 del triangulo a probar.
 * @param vert2 Vertice 3 del triangulo a probar.
 * @param t Valor de ecuación de (1)
 * @param u Coordenada u varicentrica del triángulo (2).
 * @param v Coordenada v varicentrica del triángulo (2).
 * @return Bandera que indica si hay intersección o no, true existe la intersección
 * false no existe la intersección.
 */
bool Picking::RayTriangleIntersect(glm::vec3 orig, glm::vec3 dir,
                                   glm::vec3 vert0, glm::vec3 vert1,
                                   glm::vec3 vert2, double *t, double *u,
                                   double *v) {
  glm::vec3 edge1, edge2, tvec, pvec, qvec;
  double det, inv_det;
  // Se encuentran los vectores de dos aristas que contengan a ver0
  edge1 = glm::vec3(vert1 - vert0);
  edge2 = glm::vec3(vert2 - vert0);
  // Se calcula el determiante - se usa para calcular el parametro U.
  pvec = glm::cross(dir, edge2);
  // Si el determinante es muy cercano a cero, rayo cae en el plano
  // que contiene al triángulo.
  det = glm::dot(edge1, pvec);
  if (det > -EPSILON && det < EPSILON)
    return false;
  inv_det = 1.0 / det;
  // Distancia entre el vert0 al origen del rayo.
  tvec = glm::vec3(orig - vert0);
  // Calcula el parametro u y prueba su valor.
  *u = glm::dot(tvec, pvec) * inv_det;
  if (*u < 0.0 || *u > 1.0)
    return false;
  qvec = glm::cross(tvec, edge1);
  // Calcula el parametro v y prueba que este dentro del rango.
  *v = glm::dot(dir, qvec) * inv_det;
  if (*v < 0.0 || *u + *v > 1.0)
    return false;
  // Calcula t, interseccion del rayo con el triángulo
  *t = glm::dot(edge2, qvec) * inv_det;
  return true;
}

/**
 * Método que prueba si una linea interseca con una esfera.
 * @param orig Origen del rayo.
 * @param dest Punto final del rayo.
 * @param dir Dirección del rayo.
 * @param vSphereCenter Centro de la esfera.
 * @param sphereRadius Radio de la esfera.
 * @param t Indica la distancia que existe entre el orign de la esfera
 * y el punto de intersección.
 * @return Bandera que indica si existe intersección de rayo con la esfera,
 * true existe la intersección false no existe la intersección.
 */
bool Picking::RaySphereIntersect(glm::vec3 * orig, glm::vec3 * dest,
                                 glm::vec3 * dir, glm::vec3 * vSphereCenter,
                                 float sphereRadius, float * t) {

  // Vector del Origen del rayo al centro de la esfera.
  glm::vec3 vDirToSphere = *vSphereCenter - *orig;

  // Distancia del origen al destino del rayo.
  float fLineLength = glm::distance(*orig, *dest);

  // Proyección escalar de vDirToSphere sobre la direccion del rayo.
  *t = glm::dot(vDirToSphere, *dir);

  glm::vec3 vClosestPoint;
  // Si la distancia proyectada del origen es menor o igual que cero
  // Significa que el punto mas cercano al centro es el origen.
  if (*t <= 0.0f)
    vClosestPoint = *orig;
  // Si la proyección escalar del origen es mayor a distancia del origen
  // al destino, el punto mas cercano es el destino.
  else if (*t >= fLineLength)
    vClosestPoint = *dest;
  // En caso contrario de calcula el punto sobre la linea usando t.
  else
    vClosestPoint = *orig + *dir * (*t);

  // Se pureba si el punto mas cercao esta contenido en el radio de la esfera.
  return glm::distance(*vSphereCenter, vClosestPoint) <= sphereRadius;
}

/**
 * Método que valida la superposición de marcas y evitar tener muchas marcas,
 * en un miso sitio, la condición de una marca es que el radio de tolerancia
 * de la marca no sobrepase dos veces el radio.
 * @param marks Lista actual de marcas agregadas.
 * @param sphereRadius  Radio de la esfera.
 * @param mark Marca que se desea validar.
 * @param markHit Bandera que indica que es una marca ya existente.
 * @param idLandMark Identificador de la marca con la que pego.
 * @return Bandera que indica si es valida o no la marca.
 */
bool Picking::ValidateMark(std::vector<Mark*> marks, float sphereRadius,
                           Mark * mark, Mark * markHit, int * idLandMark) {
  bool validMark = true;
  // Iterador de las marcas ya agregadas.
  for (unsigned int i = 0; i < marks.size() && validMark; i++) {
    Mark * currentMark = marks[i];
    float distanceMark = glm::distance(*currentMark->getPickPoints3D(),
                                       *mark->getPickPoints3D());
    // Valida la distancia entre la marca a ingresar complegidad O(n).
    if (distanceMark < 2.0f * sphereRadius) {
      validMark = false;
      *markHit = *currentMark;
      *idLandMark = i;
    }
  }
  return validMark;
}

/**
 * Destructor de clase.
 */
Picking::~Picking() {
}

} /* namespace Picking */
