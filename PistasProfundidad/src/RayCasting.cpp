/*
 * Picking.cpp
 *
 *  Created on: 19/04/2015
 *      Author: rey
 */

#include "Headers/RayCasting.h"
#define EPSILON (double)0.000000000001

namespace RayCasting {

RayCasting::RayCasting() {
}

/**
 * Método que obtiene las corrdenadas del origen y destino del rayo,
 * cooresponden a las coordenadas del rayo en el plano de recorte,
 * plano cercano y plano lejano, los vetores esta en el espacio del mundo,
 * es decir lo que esta viendo el observador.
 * @param x Coordenada en x, en el espacio de la pantalla.
 * @param y Coordenada en y, en el espacio de la pantalla.
 * @param ScreenWidth Ancho de ventana.
 * @param ScreenHeight Altura de ventana.
 * @param v1 Apuntador a la coordenada del origen del rayo (Near Plane).
 * @param v2 Apuntador a la coordenada del destino del rayo (Far Plane).
 */
void RayCasting::Get3DRayUnderScrean(float x, float y, int ScreenWidth,
                                     int ScreenHeight, glm::vec3* v1,
                                     glm::vec3* v2) {
  // Se obtiene el rayo en coordenadas del mundo.
  glm::vec4 viewport = glm::vec4(0.0, 0.0, ScreenWidth, ScreenHeight);
  *v1 = glm::unProject(glm::vec3(x, y, 0.0f), camera->GetViewMatrix(),
                       camera->GetProjectionMatrix(), viewport);
  *v2 = glm::unProject(glm::vec3(x, y, 1.0f), camera->GetViewMatrix(),
                       camera->GetProjectionMatrix(), viewport);
}

/**
 * Setter para la cámara, se utiliza al momento de obtener el rayo,
 * y que este este en coordenadas del modelo.
 * @param camera
 */
bool RayCasting::RayTriangleIntersect(glm::vec3 * orig, glm::vec3 * dir,
                                      glm::vec3 *vert0, glm::vec3 * vert1,
                                      glm::vec3 * vert2, double *t, double *u,
                                      double *v) {
  glm::vec3 edge1, edge2, tvec, pvec, qvec;
  double det, inv_det;
  // Se encuentran los vectores de dos aristas que contengan a ver0
  edge1 = glm::vec3(*vert1 - *vert0);
  edge2 = glm::vec3(*vert2 - *vert0);
  // Se calcula el determiante - se usa para calcular el parametro U.
  pvec = glm::cross(*dir, edge2);
  // Si el determinante es muy cercano a cero, rayo cae en el plano
  // que contiene al triángulo.
  det = glm::dot(edge1, pvec);
  if (det > -EPSILON && det < EPSILON)
    return false;
  inv_det = 1.0 / det;
  // Distancia entre el vert0 al origen del rayo.
  tvec = glm::vec3(*orig - *vert0);
  // Calcula el parametro u y prueba su valor.
  *u = glm::dot(tvec, pvec) * inv_det;
  if (*u < 0.0 || *u > 1.0)
    return false;
  qvec = glm::cross(tvec, edge1);
  // Calcula el parametro v y prueba que este dentro del rango.
  *v = glm::dot(*dir, qvec) * inv_det;
  if (*v < 0.0 || *u + *v > 1.0)
    return false;
  // Calcula t, interseccion del rayo con el triángulo
  *t = glm::dot(edge2, qvec) * inv_det;
  return true;
}

/**
 * Método que asigna la escala de grises de los vertices de un triangulo.
 * @param t distancia a la que se encuentra el objeto.
 * @param vertice Apuntador a vertice de la cara.
 * @param index Apuntador a indice de la cara.
 */
void RayCasting::assignGrayScaleValue(double t, Vertex3 * vertice, int index) {

  double normalize;
  // Obtiene el parametro de escala de grises.
  if (contrast)
    normalize =
        t
            / (scale * (GLApplication::farPlane - GLApplication::nearPlane)
                + offset);
  else
    normalize = 1
        - t
            / (scale * (GLApplication::farPlane - GLApplication::nearPlane)
                + offset);

  glm::vec3 directionScale = glm::normalize(glm::vec3(1.0, 1.0, 1.0));

  // Calcula el color en escala de grises.
  vertice[index] = Vertex3 { vertice[index].xyz, glm::vec4(
      normalize * directionScale.x, normalize * directionScale.y,
      normalize * directionScale.z, 1.0) };

}

/**
 * Método que obtiene al intersección de un rayo con el triangulo.
 * @param origRay Origen.
 * @param direction Dirección.
 * @param VP ViewMatriz * ProyectionMatriz.
 * @param vertices Vertices.
 * @param verticesTransform Vertices transformados.
 * @param index Indices.
 * @param sizeIndex Tamaño de indices.
 * @param cullFacesPointer Apuntador a las caras.
 */
void RayCasting::iterateObjectTriangle(glm::vec3 * origRay,
                                       glm::vec3 * direction, glm::mat4 * VP,
                                       Vertex3 * vertices,
                                       Vertex3 * verticesTransform,
                                       GLuint * index, int sizeIndex,
                                       bool * cullFacesPointer) {
  int indexCullFace = 0;
  bool hit;
  glm::vec4 result0;
  glm::vec4 result1;
  glm::vec4 result2;
  // Por cada cara del mallado.
  for (int i = 0; i < sizeIndex; i += 3) {
    if (cullFacesPointer != NULL) {
      // Cara visible.
      if (cullFacesPointer[indexCullFace]) {
        // Valida si ya se puso el color de esos vertices para evitar calcular dobles triangulos.
        if (vertices[index[i]].rgba.w < 0.0
            || vertices[index[i + 1]].rgba.w < 0.0
            || vertices[index[i + 2]].rgba.w < 0.0) {

          glm::vec3 * vertice0 = &verticesTransform[index[i]].xyz;
          glm::vec3 * vertice1 = &verticesTransform[index[i + 1]].xyz;
          glm::vec3 * vertice2 = &verticesTransform[index[i + 2]].xyz;

          // Valida el triangulo de la cara.
          double t, u, v;
          hit = this->RayTriangleIntersect(origRay, direction, vertice0,
                                           vertice1, vertice2, &t, &u, &v);

          // Pega el rayo con el triangulo.
          if (hit) {
            // Se obtienen las proyecciónes inversas de los vertices del triangulo.
            result0 = *VP * glm::vec4(*vertice0, 1.0f);
            result1 = *VP * glm::vec4(*vertice1, 1.0f);
            result2 = *VP * glm::vec4(*vertice2, 1.0f);

            // Asigna el color al rayo.
            this->assignGrayScaleValue(result0.z, vertices, index[i]);
            this->assignGrayScaleValue(result1.z, vertices, index[i + 1]);
            this->assignGrayScaleValue(result2.z, vertices, index[i + 2]);
          }
        }
      }
      indexCullFace++;
    }
  }
}

/**
 * Método que prueba si el objeto puede ser alcanzado por el rayo
 * utilizando el bounding box.
 * @param origRay Origen del rayo.
 * @param destRay Punto final de rayo.
 * @param direction Dirección de rayo.
 * @param testObjectHit  Objeto para probar
 * @return Bandera que indica si el rayo alcanza al objeto.
 */
bool RayCasting::hitObjectTest(glm::vec3 * origRay, glm::vec3 * destRay,
                               glm::vec3 * direction,
                               TestObjectHit * testObjectHit) {
  bool hit = false;
  // Tipo de objeto envolvente igual a caja.
  if (testObjectHit->typeObjectBox == Box) {
    // Prueba los triangulos que forman la caja, si da hit entonces termina y regresa
    // que puede intersecar los triangulos del objeto.
    for (int i = 0; i < testObjectHit->sizeIndexBB && !hit; i += 3) {
      double t, u, v;
      hit = this->RayTriangleIntersect(
          origRay, direction,
          &testObjectHit->vertexBB[testObjectHit->indexBB[i]].xyz,
          &testObjectHit->vertexBB[testObjectHit->indexBB[i + 1]].xyz,
          &testObjectHit->vertexBB[testObjectHit->indexBB[i + 2]].xyz, &t, &u,
          &v);
    }
  } else if (testObjectHit->typeObjectBox == Sphere) {
    // Prueba si existe intersección del rayo con la esfera.
    hit = this->RaySphereIntersect(origRay, destRay, direction,
                                   testObjectHit->vSphereCenter,
                                   testObjectHit->sphereRadius);
  }
  return hit;
}

/**
 * Encapsula el objeto a probar con el rayo en una estructura, para no pasarle
 * cada cosa por separado al método de intersección.
 * @param vertexBB Vertices el Bounding Box.
 * @param indexBB Indices del Boundig Box.
 * @param sizeIndexBB Tamaño de Indices del Boundig Box.
 * @param typeObjectBox Tupo de Boundig Box.
 * @param vSphereCenter Centro de la esfera si es una Sphere Box
 * @param sphereRadius Radio de la esfera si es una Sphere Box
 * @return Objeto para porbar intersección.
 */
TestObjectHit * RayCasting::createTestObjectHit(Vertex3 * vertexBB,
                                                GLuint * indexBB,
                                                int sizeIndexBB,
                                                TypeObjectBox typeObjectBox,
                                                glm::vec3 * vSphereCenter,
                                                float sphereRadius) {
  TestObjectHit * test = (TestObjectHit *) malloc(sizeof(TestObjectHit));
  test->vSphereCenter = vSphereCenter;
  test->indexBB = indexBB;
  test->vertexBB = vertexBB;
  test->sizeIndexBB = sizeIndexBB;
  test->typeObjectBox = typeObjectBox;
  test->sphereRadius = sphereRadius;
  return test;
}

/**
 * Método que prueba si una linea interseca con una esfera.
 * @param orig Origen del rayo.
 * @param dest Punto final del rayo.
 * @param dir Dirección del rayo.
 * @param vSphereCenter Centro de la esfera.
 * @param sphereRadius Radio de la esfera.
 * @return Bandera que indica si existe intersección de rayo con la esfera,
 * true existe la intersección false no existe la intersección.
 */
bool RayCasting::RaySphereIntersect(glm::vec3 * orig, glm::vec3 * dest,
                                    glm::vec3 * dir, glm::vec3 * vSphereCenter,
                                    float sphereRadius) {
  // Vector del Origen del rayo al centro de la esfera.
  glm::vec3 vDirToSphere = *vSphereCenter - *orig;

  // Distancia del origen al destino del rayo.
  float fLineLength = glm::distance(*orig, *dest);

  // Proyección escalar de vDirToSphere sobre la direccion del rayo.
  float t = glm::dot(vDirToSphere, *dir);

  glm::vec3 vClosestPoint;
  // Si la distancia proyectada del origen es menor o igual que cero
  // Significa que el punto mas cercano al centro es el origen.
  if (t <= 0.0f)
    vClosestPoint = *orig;
  // Si la proyección escalar del origen es mayor a distancia del origen
  // al destino, el punto mas cercano es el destino.
  else if (t >= fLineLength)
    vClosestPoint = *dest;
  // En caso contrario de calcula el punto sobre la linea usando t.
  else
    vClosestPoint = *orig + *dir * (t);

  // Se pureba si el punto mas cercao esta contenido en el radio de la esfera.
  return glm::distance(*vSphereCenter, vClosestPoint) <= sphereRadius;
}

/**
 * Método que encuentra la profundida que se encuentra un objeto.
 * @param mousePosition Posición del mouse espacio de la ventana.
 * @param vertices Arreglo de vertices transformados al modelo de la escena.
 * @param index Indices de los vertices.
 * @param sizeIndex Tamaño de indices.
 * @param ScreenWidth Ancho de ventana.
 * @param ScreenHeight Alto de ventana.
 * @param VP ViewMatriz * ProyectionMatriz.
 * @param testHit Encamsulamiento de objeto para hacer la prueba con el Bounding Box.
 * @param cullFacesPointer Apuntador a caras visibles y no visibles.
 * @return Profunidad minima a la que se encuentra el objeto, regresa -1 si no encuentra intersección.
 */
float RayCasting::getDistanceScaleValue(glm::ivec2 * mousePosition,
                                        Vertex3 * vertices, GLuint * index,
                                        int sizeIndex, int ScreenWidth,
                                        int ScreenHeight, glm::mat4 * VP,
                                        TestObjectHit * testHit,
                                        bool * cullFacesPointer) {
  int indexCullFace = 0;
  // Valores en coordenadas de los pixeles en cordenadas de la pantalla.
  int px = mousePosition->x;
  int py = ScreenHeight - mousePosition->y;
  glm::vec3 * v1 = new glm::vec3(), *v2 = new glm::vec3();
  // Obtiene el rayo que pasa por la posición del mouse.
  this->Get3DRayUnderScrean(px, py, ScreenWidth, ScreenHeight, v1, v2);
  glm::vec3 direction = glm::normalize(*v2 - *v1);
  // Pureba con su bounding Box.
  if (this->hitObjectTest(v1, v2, &direction, testHit)) {
    bool hit = false;
    bool hitMin;
    float distMin = -1;
    vec3 * intersection = new vec3(0.0f);
    // Por cada cara se calcula se prueba si hay intersección.
    for (int i = 0; i < sizeIndex; i += 3) {
      if (cullFacesPointer != NULL && cullFacesPointer[indexCullFace]) {
        double t, u, v;
        hitMin = this->RayTriangleIntersect(v1, &direction,
                                            &vertices[index[i]].xyz,
                                            &vertices[index[i + 1]].xyz,
                                            &vertices[index[i + 2]].xyz, &t, &u,
                                            &v);
        // Asigane el la minima distancia.
        if (t >= 0 && hitMin
            && (distMin == -1 || (distMin > -1 && distMin > t))) {
          distMin = t;
          float factor = 1 - u - v;
          glm::vec3 minInt = glm::vec3(factor * vertices[index[i]].xyz.x,
                                       factor * vertices[index[i]].xyz.y,
                                       factor * vertices[index[i]].xyz.z)
              + glm::vec3(u * vertices[index[i + 1]].xyz.x,
                          u * vertices[index[i + 1]].xyz.y,
                          u * vertices[index[i + 1]].xyz.z)
              + glm::vec3(v * vertices[index[i + 2]].xyz.x,
                          v * vertices[index[i + 2]].xyz.y,
                          v * vertices[index[i + 2]].xyz.z);
          intersection->x = minInt.x;
          intersection->y = minInt.y;
          intersection->z = minInt.z;
          hit = true;
        }
      }
      indexCullFace++;
    }
    if (hit) {
      // Profundid del rayo.
      glm::vec4 result0 = *VP * glm::vec4(*intersection, 1.0f);
      return result0.z;
    }
  }
  return -1;
}

/**
 * Destructor de clase.
 */
RayCasting::~RayCasting() {
}

} /* namespace Picking */
