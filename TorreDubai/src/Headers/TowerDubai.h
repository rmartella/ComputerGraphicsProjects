/*
 * TowerRoot.h
 *
 *  Created on: 16/05/2015
 *      Author: rey
 */
#include "Model.h"
#include "ModelEnvironment.h"
#include "TowerLevel.h"
#include "Util.h"
#include <vector>
#include <math.h>

#ifndef TOWERROOT_H_
#define TOWERROOT_H_

/**
 * Clase que construye y proporciona las rotaciones de la torre.
 */
class TowerDubai {

 public:

  /**
   * Constructor de torre.
   * @param numeLevels Número de niveles.
   * @param modelStruct Modelo que no refleja.
   * @param modelEnvironment Modelo que refleja el ambiente.
   */
  TowerDubai(int numeLevels, Model * modelStruct,
             ModelEnvironment * modelEnvironment);
  /**
   * Obtiene la trnaslación que debe tener la torre en cada nivel.
   * @param meshEntryesPtr
   * @return
   */
  float getDistanceForTranslate(
      std::vector<Mesh::Mesh::MeshEntry> * meshEntryesPtr);
  /**
   * Método que crea e inicializa proceduralmente los niveles de la torre.
   */
  void createLevelsTower();
  /**
   * Método que renderiza todos lo niveles de la torre.
   */
  void render();
  /**
   * Setter para la matriz de transformación de toda la torre.
   * @param towerTransform Matriz de transformación de toda la torre.
   */
  void setTowerTransform(glm::mat4 * towerTransform) {
    this->towerTransform = towerTransform;
  }
  /**
   * Rota un nivel un epsiolon.
   * @param giro Incremento.
   * @param level Nivel.
   */
  void rotateLevel(float giro, int level);
  /**
   * Método que coordina las rotaciones de los niveles.
   * @param rotInc Incremento que debe hacer a la rotación.
   */
  void cordinateRotation(float rotInc);

  /**
   * Destructor de clase.
   */
  virtual ~TowerDubai();

  /**
   * Obtiene la altura del nivel.
   * @return Altura del nivel.
   */
  float getHeightLevel() const {
    return heightLevel;
  }

  /**
   * Setter para la altura del nivel.
   * @param heightLevel Altura del nivel.
   */
  void setHeightLevel(float heightLevel) {
    this->heightLevel = heightLevel;
  }

 protected:
 private:
  int numLevels;
  float heightLevel;
  Model * modelStruct;
  ModelEnvironment * modelEnvironment;
  glm::mat4 * towerTransform;
  std::vector<TowerLevel*> * listLevelsTower;
};

#endif /* TOWERROOT_H_ */
