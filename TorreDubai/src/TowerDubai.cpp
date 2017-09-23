/*
 * TowerRoot.cpp
 *
 *  Created on: 16/05/2015
 *      Author: rey
 */

#include "Headers/TowerDubai.h"

#define offset (float)0.025

/**
 * Constructor de torre.
 * @param numeLevels Número de niveles.
 * @param modelStruct Modelo que no refleja.
 * @param modelEnvironment Modelo que refleja el ambiente.
 */
TowerDubai::TowerDubai(int numeLevels, Model * modelStruct,
                       ModelEnvironment * modelEnvironment) {
  this->numLevels = numeLevels;
  this->modelStruct = modelStruct;
  this->modelEnvironment = modelEnvironment;
  listLevelsTower = new std::vector<TowerLevel*>();
}
/**
 * Obtiene la trnaslación que debe tener la torre en cada nivel.
 * @param meshEntryesPtr
 * @return
 */
float TowerDubai::getDistanceForTranslate(
    std::vector<Mesh::Mesh::MeshEntry> * meshEntryesPtr) {
  // Iteración para obtener la longitud del desplazamiento del nivel.
  // Se itera sobre el modelo .obj.
  float ymin, ymax;
  for (unsigned int i = 0; i < meshEntryesPtr->size(); i++) {
    for (unsigned int j = 0; j < (*meshEntryesPtr)[i].NumIndices; j++) {
      if (i == 0) {
        ymin = (*meshEntryesPtr)[i].Vertices[(*meshEntryesPtr)[i].Indices[j]]
            .m_pos.y;
        ymax = (*meshEntryesPtr)[i].Vertices[(*meshEntryesPtr)[i].Indices[j]]
            .m_pos.y;
      }
      ymin =
          aisgl_min(
              ymin,
              (*meshEntryesPtr)[i].Vertices[(*meshEntryesPtr)[i].Indices[j]]
                  .m_pos.y);
      ymax =
          aisgl_max(
              ymax,
              (*meshEntryesPtr)[i].Vertices[(*meshEntryesPtr)[i].Indices[j]]
                  .m_pos.y);
    }
  }
  return (ymax - ymin);
}

/**
 * Método que crea e inicializa proceduralmente los niveles de la torre.
 */
void TowerDubai::createLevelsTower() {
  float yTranslate = this->heightLevel;

  // Agulo que que tanto va a girar cada nivel de la torre para que cambie de dirección.
  int angle = 360;

  // Iteración del número de niveles.
  for (int i = 0; i < numLevels; i++) {
    // Escala de cada nivel.
    float scale = (numLevels - i) / (float) numLevels;
    // Se crea un nivel.
    TowerLevel * level = new TowerLevel(this->modelStruct,
                                        this->modelEnvironment);
    //Declaración de la matriz de transformación de cada nivel.
    glm::mat4 levelTransform = glm::mat4(glm::mat4(1.0));
    // Translacción hacia arriba del nivel.
    levelTransform = glm::translate(
        levelTransform,
        glm::vec3(0.0f, (float) i * (yTranslate + offset ), 0.0f));
    // Se escala el nivel dependiendo en que nivel este.
    levelTransform = glm::scale(
        levelTransform,
        vec3(1.0 + (numLevels - i) / ((float) numLevels), 1.0, 1.0));
    // Inicialización de la rotación del nivel.
    levelTransform = glm::rotate(levelTransform,
                                 -i * glm::radians((float) angle / numLevels),
                                 vec3(0.0, 1.0, 0.0));
    // Se le indica que ha rotado lo anterior
    level->setGiroAct(
        glm::radians(((float) angle) / 2)
            - i * glm::radians((float) angle / numLevels));
    // Se indica el angulo maximo de rotación.
    // Este parametro es la mitad del angulo de giro
    // Debido a que se quiere que la oscilación en una dirección empiece
    // en el pizo de abajo mientras en el de arriba termina un dirección mas.
    level->setAngleRotMax(glm::radians(((float) angle) / 2));

    level->setLevelTransform(new glm::mat4(levelTransform));

    listLevelsTower->push_back(level);
    listLevelsTower[0][i]->setDirection(1);
  }
}

/**
 * Método que renderiza todos lo niveles de la torre.
 */
void TowerDubai::render() {
  // Render de cada nivel.
  for (int i = 0; i < this->numLevels; i++) {
    listLevelsTower[0][i]->render(
        *towerTransform
            * glm::mat4(*listLevelsTower[0][i]->getLevelTransform()));
  }
}
/**
 * Rota un nivel un epsiolon.
 * @param giro Incremento.
 * @param level Nivel.
 */
void TowerDubai::rotateLevel(float giro, int level) {
  glm::mat4 * transformL = new glm::mat4(1.0);
  float angle = 0.0f;
  // Revisa que dirección se encuetra el giro del nivel.
  // Gira dependiendo del sentido.
  if (listLevelsTower[0][level]->getDirection() == 1) {
    angle = listLevelsTower[0][level]->getGiroAct() + giro;
    *transformL = glm::rotate(*transformL, giro, glm::vec3(0.0f, 1.0f, 0.0f));
  } else {
    angle = listLevelsTower[0][level]->getGiroAct() - giro;
    *transformL = glm::rotate(*transformL, -giro, glm::vec3(0.0f, 1.0f, 0.0f));
  }
  *transformL = *listLevelsTower[0][level]->getLevelTransform() * (*transformL);
  listLevelsTower[0][level]->setLevelTransform(transformL);
  listLevelsTower[0][level]->setGiroAct(angle);
}
/**
 * Método que coordina las rotaciones de los niveles.
 * @param rotInc Incremento que debe hacer a la rotación.
 */
void TowerDubai::cordinateRotation(float rotInc) {
  // Giro para cada nivel.
  for (int i = 0; i < numLevels; i++) {
    TowerLevel* levelPtr = listLevelsTower[0][i];
    // Revisa cuando la torre va a cambiar de dirección.
    // Y actualiza el giro maximo que debe de dar.
    if (levelPtr->getDirection() != 0) {
      if (levelPtr->getDirection() == 1
          && levelPtr->getGiroAct() < levelPtr->getAngleRotMax()) {
        levelPtr->setDirection(1);
      } else if (levelPtr->getDirection() == -1
          && levelPtr->getGiroAct() < -levelPtr->getAngleRotMax()) {
        levelPtr->setDirection(1);
        levelPtr->setGiroAct(-levelPtr->getAngleRotMax());
      } else if (levelPtr->getDirection() == 1
          && levelPtr->getGiroAct() > levelPtr->getAngleRotMax()) {
        levelPtr->setDirection(-1);
        levelPtr->setGiroAct(levelPtr->getAngleRotMax());
      } else
        levelPtr->setDirection(-1);
    }
    rotateLevel(rotInc, i);
  }
}
/**
 * Destructor de clase.
 */
TowerDubai::~TowerDubai() {
  this->modelEnvironment->Destroy();
  this->modelStruct->Destroy();
  free(this->listLevelsTower);
}

;
