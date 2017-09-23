/*
 * Node.h
 *
 *  Created on: 04/04/2015
 *      Author: rey
 */

#include <vector>
#include "Model.h"
#ifndef NODE_H_
#define NODE_H_

class Node {
 public:
  /**
   * Constructor defualt.
   */
  Node();
  /**
   * Constructor de un cubo de rubick con un modelo e indica si es el cubo raíz es invisible o no.
   * @param model Modelo del Rubick.
   * @param isRoot Bandera si es el nodo padre para no pintarlo.
   */
  Node(Model * model, bool isRoot);
  /**
   * Setter de la matriz de transformación del nodo prinicipal del cubo.
   * @param transform Matriz de transformación de un nodo del cubo.
   */
  void setTransformMatrix(glm::mat4 * transform);
  /**
   * Obtiene la matriz de transformación de un nodo del cubo.
   * @return Matriz de transformación de un nodo del cubo.
   */
  glm::mat4 * getTransformMatrix();
  /**
   * Dibuja la estructura del cubo de rubick.
   */
  void renderNode();
  /**
   * Se obtiene el modelo asociado al nodo.
   * @return Modelo del nodo.
   */
  Model * getModel();
  /**
   * Agrega un hijo al nodo para emparentar las dependencias.
   * @param child Nodo hijo.
   */
  void addChild(Node * child);
  /**
   * Destructor.
   */
  virtual ~Node();
 private:
  std::vector<Node*> * nodeList = new std::vector<Node*>();
  glm::mat4 * transformMatrix;
  Model * model;
  bool isRoot;
};

#endif /* NODE_H_ */
