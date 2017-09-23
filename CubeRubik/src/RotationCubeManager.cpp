/*
 * RotationCubeManager.cpp
 *
 *  Created on: 05/05/2015
 *      Author: rey
 */

#include "Headers/RotationCubeManager.h"

namespace RotationCubeManager {

/**
 * Constructor.
 */
RotationCubeManager::RotationCubeManager() {
  for (int i = 0; i < 6; i++) {
    this->faceMap[i] = i;
  }
}

/**
 * Destructor.
 */
RotationCubeManager::~RotationCubeManager() {
}

/**
 * Metodo que agrega una rotación
 * @param face Cara que se agrega.
 * @param direction Dirección de la cara.
 */
void RotationCubeManager::addRotation(int face, int direction) {
  Movimiento * mov = (Movimiento *) malloc(sizeof(Movimiento));
  mov->direction = direction;
  mov->face = faceMap[face];
  movimientos->push_back(mov);
}
/**
 * Método que elimina la rotación de una cara al completar su giro.
 */
void RotationCubeManager::deleteCurrentCompleteRotation() {
  movimientos->erase(movimientos->begin());
}
/**
 * Metodo que obtiene el ultimo movimiento que esta realizando el rubick.
 */
Movimiento * RotationCubeManager::getCurrentRotation() {
  if (movimientos->size() > 0)
    return movimientos->front();
  return NULL;
}
/**
 * Método que actualiza las orientación de las caras del rubick.
 */
void RotationCubeManager::updateOrientation() {

  glm::mat4 WorldMatrixVertex = glm::scale(WorldMatrix, glm::vec3(3, 3, 3));

  glm::vec4 origenWorldX = camera->GetViewMatrix() * WorldMatrix
      * glm::vec4(0, 0, 0, 1.0);

  // Se obtiene la cara que esta mas cerca del observador, se obtiene mediante
  // un rayo que va desde el punto del observador al centro de cubo, esto
  // se debe hacer en el espacio del mundo, para ello primero se construye
  // la posición del rayo e iterar sobre las corrdenadas de los triangulos
  // del cubo en el espacio en el WorldSpace.
  float minD = 0;
  int faceX = -1;
  for (int i = 0; i < 72; i += 3) {
    double t, u, v;
    glm::vec4 vertex0 = camera->GetViewMatrix() * WorldMatrixVertex
        * glm::vec4(verticesCube[indexCube[i]].xyz[0],
                    verticesCube[indexCube[i]].xyz[1],
                    verticesCube[indexCube[i]].xyz[2], 1.0);
    glm::vec4 vertex1 = camera->GetViewMatrix() * WorldMatrixVertex
        * glm::vec4(verticesCube[indexCube[i + 1]].xyz[0],
                    verticesCube[indexCube[i + 1]].xyz[1],
                    verticesCube[indexCube[i + 1]].xyz[2], 1.0);
    glm::vec4 vertex2 = camera->GetViewMatrix() * WorldMatrixVertex
        * glm::vec4(verticesCube[indexCube[i + 2]].xyz[0],
                    verticesCube[indexCube[i + 2]].xyz[1],
                    verticesCube[indexCube[i + 2]].xyz[2], 1.0);
    bool intersect = RayTriangleIntersect(
        camera->GetPosition(),
        glm::normalize(
            glm::vec3(origenWorldX.x, origenWorldX.y, origenWorldX.z)
                - camera->GetPosition()),
        glm::vec3(vertex0.x, vertex0.y, vertex0.z),
        glm::vec3(vertex1.x, vertex1.y, vertex1.z),
        glm::vec3(vertex2.x, vertex2.y, vertex2.z), &t, &u, &v);
    if (intersect) {
      if (minD == 0 || (minD > 0 && t < minD)) {
        minD = t;
        faceX = i / 12;
      }
    }
  }

  // Para determinar que cara se esta viendo si se tuviera un punto de vista desde
  // arriba del cubo (eje y) se crea el punto como si la camara estuviera en la misma
  // profundidad del cubo y misma coordenada del eje z de la camara.
  int faceY = -1;
  for (int i = 0; i < 72; i += 3) {
    double t, u, v;
    glm::vec4 vertex0 = camera->GetViewMatrix() * WorldMatrixVertex
        * glm::vec4(verticesCube[indexCube[i]].xyz[0],
                    verticesCube[indexCube[i]].xyz[1],
                    verticesCube[indexCube[i]].xyz[2], 1.0);
    glm::vec4 vertex1 = camera->GetViewMatrix() * WorldMatrixVertex
        * glm::vec4(verticesCube[indexCube[i + 1]].xyz[0],
                    verticesCube[indexCube[i + 1]].xyz[1],
                    verticesCube[indexCube[i + 1]].xyz[2], 1.0);
    glm::vec4 vertex2 = camera->GetViewMatrix() * WorldMatrixVertex
        * glm::vec4(verticesCube[indexCube[i + 2]].xyz[0],
                    verticesCube[indexCube[i + 2]].xyz[1],
                    verticesCube[indexCube[i + 2]].xyz[2], 1.0);
    bool intersect = RayTriangleIntersect(
        glm::vec3(0, camera->GetPosition().z, origenWorldX.z),
        glm::normalize(
            glm::vec3(origenWorldX.x, origenWorldX.y, origenWorldX.z)
                - glm::vec3(0, camera->GetPosition().z, origenWorldX.z)),
        glm::vec3(vertex0.x, vertex0.y, vertex0.z),
        glm::vec3(vertex1.x, vertex1.y, vertex1.z),
        glm::vec3(vertex2.x, vertex2.y, vertex2.z), &t, &u, &v);
    if (intersect) {
      if (minD == 0 || (minD > 0 && t < minD)) {
        minD = t;
        faceY = i / 12;
      }
    }
  }

  // Se actualizan las caras que representa lo que esta viendo el observador.
  updateFaceMap(faceX, faceY);
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
bool RotationCubeManager::RayTriangleIntersect(glm::vec3 orig, glm::vec3 dir,
                                               glm::vec3 vert0, glm::vec3 vert1,
                                               glm::vec3 vert2, double *t,
                                               double *u, double *v) {
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
 * Método que mapea las caras reales con las que fue contruido el rubick y con las caras que estan
 * De frente al observador.
 * @param faceAxisX Cara que esta viendo el observador.
 * @param faceAxisY Cara que esta viendo una camara invisible sobre el eje y.
 */
void RotationCubeManager::updateFaceMap(int faceAxisX, float faceAxisY) {
  if (faceAxisX == 0 && faceAxisY == 1) {
    faceMap[0] = 0;
    faceMap[1] = 1;
    faceMap[2] = 2;
    faceMap[3] = 3;
    faceMap[4] = 4;
    faceMap[5] = 5;
  } else if (faceAxisX == 0 && faceAxisY == 2) {
    faceMap[0] = 4;
    faceMap[1] = 1;
    faceMap[2] = 0;
    faceMap[3] = 3;
    faceMap[4] = 5;
    faceMap[5] = 2;
  } else if (faceAxisX == 0 && faceAxisY == 4) {
    faceMap[0] = 5;
    faceMap[1] = 1;
    faceMap[2] = 4;
    faceMap[3] = 3;
    faceMap[4] = 2;
    faceMap[5] = 0;
  } else if (faceAxisX == 0 && faceAxisY == 5) {
    faceMap[0] = 2;
    faceMap[1] = 1;
    faceMap[2] = 5;
    faceMap[3] = 3;
    faceMap[4] = 0;
    faceMap[5] = 4;
  } else if (faceAxisX == 1 && faceAxisY == 3) {
    faceMap[0] = 1;
    faceMap[1] = 5;
    faceMap[2] = 2;
    faceMap[3] = 0;
    faceMap[4] = 4;
    faceMap[5] = 3;
  } else if (faceAxisX == 1 && faceAxisY == 2) {
    faceMap[0] = 4;
    faceMap[1] = 5;
    faceMap[2] = 1;
    faceMap[3] = 0;
    faceMap[4] = 3;
    faceMap[5] = 2;
  } else if (faceAxisX == 1 && faceAxisY == 0) {
    faceMap[0] = 3;
    faceMap[1] = 5;
    faceMap[2] = 4;
    faceMap[3] = 0;
    faceMap[4] = 2;
    faceMap[5] = 1;
  } else if (faceAxisX == 1 && faceAxisY == 5) {
    faceMap[0] = 2;
    faceMap[1] = 5;
    faceMap[2] = 3;
    faceMap[3] = 0;
    faceMap[4] = 1;
    faceMap[5] = 4;
  } else if (faceAxisX == 3 && faceAxisY == 4) {
    faceMap[0] = 5;
    faceMap[1] = 3;
    faceMap[2] = 2;
    faceMap[3] = 1;
    faceMap[4] = 4;
    faceMap[5] = 0;
  } else if (faceAxisX == 3 && faceAxisY == 2) {
    faceMap[0] = 4;
    faceMap[1] = 3;  //
    faceMap[2] = 5;
    faceMap[3] = 1;  //
    faceMap[4] = 0;
    faceMap[5] = 2;
  } else if (faceAxisX == 3 && faceAxisY == 1) {
    faceMap[0] = 0;
    faceMap[1] = 3;  //
    faceMap[2] = 4;
    faceMap[3] = 1;  //
    faceMap[4] = 2;
    faceMap[5] = 5;
  } else if (faceAxisX == 3 && faceAxisY == 5) {
    faceMap[0] = 2;
    faceMap[1] = 3;  //
    faceMap[2] = 0;
    faceMap[3] = 1;  //
    faceMap[4] = 5;
    faceMap[5] = 4;
  } else if (faceAxisX == 4 && faceAxisY == 0) {
    faceMap[0] = 3;
    faceMap[1] = 0;  //
    faceMap[2] = 2;
    faceMap[3] = 5;  //
    faceMap[4] = 4;
    faceMap[5] = 1;
  } else if (faceAxisX == 4 && faceAxisY == 2) {
    faceMap[0] = 4;
    faceMap[1] = 0;  //
    faceMap[2] = 3;
    faceMap[3] = 5;  //
    faceMap[4] = 1;
    faceMap[5] = 2;
  } else if (faceAxisX == 4 && faceAxisY == 3) {
    faceMap[0] = 1;
    faceMap[1] = 0;  //
    faceMap[2] = 4;
    faceMap[3] = 5;  //
    faceMap[4] = 2;
    faceMap[5] = 3;
  } else if (faceAxisX == 4 && faceAxisY == 5) {
    faceMap[0] = 2;
    faceMap[1] = 0;  //
    faceMap[2] = 1;
    faceMap[3] = 5;  //
    faceMap[4] = 3;
    faceMap[5] = 4;
  } else if (faceAxisX == 5 && faceAxisY == 1) {
    faceMap[0] = 0;
    faceMap[1] = 4;  //
    faceMap[2] = 1;
    faceMap[3] = 2;  //
    faceMap[4] = 3;
    faceMap[5] = 5;
  } else if (faceAxisX == 5 && faceAxisY == 0) {
    faceMap[0] = 3;
    faceMap[1] = 4;  //
    faceMap[2] = 0;
    faceMap[3] = 2;  //
    faceMap[4] = 5;
    faceMap[5] = 1;
  } else if (faceAxisX == 5 && faceAxisY == 4) {
    faceMap[0] = 5;
    faceMap[1] = 4;  //
    faceMap[2] = 3;
    faceMap[3] = 2;  //
    faceMap[4] = 1;
    faceMap[5] = 0;
  } else if (faceAxisX == 5 && faceAxisY == 3) {
    faceMap[0] = 1;
    faceMap[1] = 4;  //
    faceMap[2] = 5;
    faceMap[3] = 2;  //
    faceMap[4] = 0;
    faceMap[5] = 3;
  } else if (faceAxisX == 2 && faceAxisY == 1) {
    faceMap[0] = 0;
    faceMap[1] = 2;  //
    faceMap[2] = 3;
    faceMap[3] = 4;  //
    faceMap[4] = 1;
    faceMap[5] = 5;
  } else if (faceAxisX == 2 && faceAxisY == 3) {
    faceMap[0] = 1;
    faceMap[1] = 2;  //
    faceMap[2] = 0;
    faceMap[3] = 4;  //
    faceMap[4] = 5;
    faceMap[5] = 3;
  } else if (faceAxisX == 2 && faceAxisY == 4) {
    faceMap[0] = 5;
    faceMap[1] = 2;  //
    faceMap[2] = 1;
    faceMap[3] = 4;  //
    faceMap[4] = 3;
    faceMap[5] = 0;
  } else if (faceAxisX == 2 && faceAxisY == 0) {
    faceMap[0] = 3;
    faceMap[1] = 2;  //
    faceMap[2] = 5;
    faceMap[3] = 4;  //
    faceMap[4] = 0;
    faceMap[5] = 1;
  }
}
/**
 * Método que guarda el estado del rubick al cerrar la ventana.
 */
void RotationCubeManager::saveRotations() {
  std::ofstream fileSave;
  fileSave.open("src/session.txt");
  if (fileSave.is_open()) {
    for (int i = 0; i < movimientosSave->size(); i++) {
      /* ok, proceed with output */
      fileSave << (*movimientosSave)[i]->face << "|"
               << (*movimientosSave)[i]->direction << ",";
    }
  }
  fileSave.close();
}

/**
 * Métdo que carga las rotaciones al iniciar sesión.
 */
void RotationCubeManager::loadRotations() {
  std::ifstream fileOpen;
  std::string line;
  fileOpen.open("src/session.txt");
  if (fileOpen.is_open()) {
    while (getline(fileOpen, line)) {
      std::istringstream splitM(line);
      for (std::string eachM; std::getline(splitM, eachM, ',');) {
        //printf("splitM %s\n", eachM.c_str());
        std::string face;
        std::string direction;
        std::istringstream split(eachM);
        std::getline(split, face, '|');
        std::getline(split, direction, '|');
        /*printf("face %s direction %s\n", face.c_str(),
         direction.c_str());*/
        Movimiento * movimiento = (Movimiento *) malloc(sizeof(Movimiento));
        movimiento->direction = atoi(direction.c_str());
        movimiento->face = atoi(face.c_str());
        movimientosSave->push_back(movimiento);
      }
      //cout << line << '\n';
    }
    fileOpen.close();
  } else
    printf("Unable to open file\n");
}

} /* namespace RotationCubeManager */
