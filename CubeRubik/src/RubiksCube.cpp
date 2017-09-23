/*
 * RubiksCube.cpp
 *
 *  Created on: 04/04/2015
 *      Author: rey
 */

#include "Headers/RubiksCube.h"

// Separación entre aristas del cubo.
const float separation = 2.005f;

// Vector de sepración de nodos.
glm::vec3 faceOffsets[] = { glm::vec3(0, -separation, 0), glm::vec3(0, 0,
                                                                    separation),
    glm::vec3(separation, 0, 0), glm::vec3(0, 0, -separation), glm::vec3(
        -separation, 0, 0), glm::vec3(0, separation, 0) };

// Arreglo de cubos que forman el rubick.
Node * cubies[26];

/**
 * Constructor del cubo de rubick.
 * @param rootNode
 */
RubiksCube::RubiksCube(Node * rootNode) {
  addCenters(rootNode);
  addEdgesAndCorners(rootNode);
  for (unsigned int i = 0; i < 26; i++)
    rootNode->addChild(cubies[i]);
}

/**
 * Constructor.
 */
RubiksCube::RubiksCube() {
}

/**
 * Destructor.
 */
RubiksCube::~RubiksCube() {
}

/**
 * Método que crea los centros de las caras del cubo.
 * @param rootNode Nodo padre.
 */
void RubiksCube::addCenters(Node * rootNode) {
  for (unsigned int i = 0; i < 6; i++) {
    Node * center = new Node(rootNode->getModel(), true);
    glm::mat4 * offset = new glm::mat4(
        glm::translate(glm::mat4(1.0f), faceOffsets[i]));
    center->setTransformMatrix(offset);
    cubies[i] = center;
    faces[i] = RubiksCubeFace(center, glm::normalize(faceOffsets[i]));
  }
}

/**
 * Se agregan los nodos para formar la cruz y las esquinas.
 * @param rootNode Nodo padre.
 */
void RubiksCube::addEdgesAndCorners(Node * rootNode) {
  for (int halfCycle = 0; halfCycle < 2; halfCycle++) {
    for (int i = 1; i < 5; i++) {
      glm::vec3 translateVec;

      // Cubos superiores e inferiores a los centro de la caras.
      {
        Node * edge = new Node(rootNode->getModel(), true);
        translateVec = faceOffsets[halfCycle * 5] + faceOffsets[i];

        glm::mat4 * translateMat = new glm::mat4(
            glm::translate(glm::mat4(1.0f), translateVec));

        edge->setTransformMatrix(translateMat);

        if (halfCycle == 0) {
          faces[0].addAdjacentToFace(edge, 4 - i);
        } else {
          faces[5].addAdjacentToFace(edge, i - 1);
        }
        faces[i].addAdjacentToFace(edge, halfCycle * 2);

        cubies[5 + i + (4 * halfCycle)] = edge;
      }

      int adjacentFace = i % 4 + 1;

      // Esquinas al centro de la cara del cubo.
      {
        translateVec = translateVec + faceOffsets[adjacentFace];
        glm::mat4 * translateMat = new glm::mat4(
            glm::translate(glm::mat4(1.0f), translateVec));

        Node * corner = new Node(rootNode->getModel(), true);
        corner->setTransformMatrix(translateMat);

        if (halfCycle == 0) {
          faces[0].addCornerToFace(corner, 4 - i);
        } else {
          faces[5].addCornerToFace(corner, i - 1);
        }

        faces[i].addCornerToFace(corner, halfCycle);
        faces[adjacentFace].addCornerToFace(corner, 3 - halfCycle);

        cubies[13 + i + (4 * halfCycle)] = corner;
      }

      // Cubos izquierdo y derecho a los centros de las caras.
      {
        if (halfCycle == 0) {

          mat4 * translateMat = new mat4(
              glm::translate(glm::mat4(1.0f),
                             faceOffsets[i] + faceOffsets[adjacentFace]));

          Node * sideEdge = new Node(rootNode->getModel(), true);
          sideEdge->setTransformMatrix(translateMat);

          faces[i].addAdjacentToFace(sideEdge, 1);
          faces[adjacentFace].addAdjacentToFace(sideEdge, 3);

          cubies[21 + i] = sideEdge;
        }
      }
    }
  }
}

void RubiksCube::rotateFaceAuto(float turnRatio, bool armar, bool * complete) {
  std::vector<Movimiento*> * movimientos = rotationCubeManager
      ->GetMovimientosSave();
  int indice;
  if (indexArray == 0) {
    indexArray = rotationCubeManager->GetMovimientosSave()->size();
    if (armar)
      indice = indexArray - 1;
    else
      indice = rotationCubeManager->GetMovimientosSave()->size() - indexArray;
    currDirection = (*rotationCubeManager->GetMovimientosSave())[indice]
        ->direction;
    if (currDirection == 0 && armar)
      currDirection = 1;
    else if (currDirection == 1 && armar)
      currDirection = 0;
    currFace = (*rotationCubeManager->GetMovimientosSave())[indice]->face;
  } else if (turnRatio < currTurnRatio) {
    faces[currFace].rotate(1, currDirection, currTurnPortion);
    faces[currFace].flipFacesMatrix(currDirection);

    if (indexArray > 1) {
      indexArray--;
      if (armar)
        indice = indexArray - 1;
      else
        indice = rotationCubeManager->GetMovimientosSave()->size() - indexArray;
      currDirection = (*movimientos)[indice]->direction;
      if (currDirection == 0 && armar)
        currDirection = 1;
      else if (currDirection == 1 && armar)
        currDirection = 0;
      currFace = (*movimientos)[indice]->face;
    } else {
      *complete = true;
      indexArray = 0;
      currFace = 0;
      currDirection = 0;
    }

    turnRatio = 0;
  }

  faces[currFace].rotate(turnRatio, currDirection, currTurnPortion);
  currTurnRatio = turnRatio;
}

void RubiksCube::rotateFace(float turnRatio, bool armar, bool * ready) {
  currDirection = rotationCubeManager->getCurrentRotation()->direction;
  currFace = rotationCubeManager->getCurrentRotation()->face;

  if (turnRatio < currTurnRatio) {
    faces[currFace].rotate(1, currDirection, currTurnPortion);
    faces[currFace].flipFacesMatrix(currDirection);

    rotationCubeManager->deleteCurrentCompleteRotation();
    Movimiento * movPtr = (Movimiento *) malloc(sizeof(Movimiento));
    movPtr->direction = currDirection;
    movPtr->face = currFace;
    rotationCubeManager->GetMovimientosSave()->push_back(movPtr);

    if (armar && !*ready) {
      while (rotationCubeManager->getCurrentRotation() != NULL) {
        rotationCubeManager->deleteCurrentCompleteRotation();
      }
      *ready = true;
    } else if (rotationCubeManager->getCurrentRotation() != NULL) {
      currDirection = rotationCubeManager->getCurrentRotation()->direction;
      currFace = rotationCubeManager->getCurrentRotation()->face;
    }
    turnRatio = 0;
  }

  faces[currFace].rotate(turnRatio, currDirection, currTurnPortion);
  currTurnRatio = turnRatio;
}
