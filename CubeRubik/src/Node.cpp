/*
 * Node.cpp
 *
 *  Created on: 04/04/2015
 *      Author: rey
 */

#include "Headers/Node.h"

/**
 * Constructor defualt.
 */
Node::Node() {
  this->isRoot = false;
}

/**
 * Constructor de un cubo de rubick con un modelo e indica si es el cubo raíz es invisible o no.
 * @param model Modelo del Rubick.
 * @param isRoot Bandera si es el nodo padre para no pintarlo.
 */
Node::Node(Model * model, bool isRoot) {
  this->isRoot = isRoot;
  this->model = model;
}

/**
 * Destructor.
 */
Node::~Node() {
  // TODO Auto-generated destructor stub
}

/**
 * Setter de la matriz de transformación del nodo prinicipal del cubo.
 * @param transform Matriz de transformación de un nodo del cubo.
 */
void Node::setTransformMatrix(glm::mat4 * transform) {
  this->transformMatrix = transform;
}

/**
 * Obtiene la matriz de transformación de un nodo del cubo.
 * @return Matriz de transformación de un nodo del cubo.
 */
glm::mat4 * Node::getTransformMatrix() {
  return this->transformMatrix;
}

/**
 * Se obtiene el modelo asociado al nodo.
 * @return Modelo del nodo.
 */
Model * Node::getModel() {
  return this->model;
}

/**
 * Agrega un hijo al nodo para emparentar las dependencias.
 * @param child Nodo hijo.
 */
void Node::addChild(Node * child) {
  this->nodeList->push_back(child);
}
/**
 * Dibuja la estructura del cubo de rubick.
 */
void Node::renderNode() {
  // Iteración sobre los nodos del cubo de rubick.
  for (unsigned int i = 0; i < this->nodeList->size(); i++) {
    glm::mat4 parentMat = *this->transformMatrix;
    glm::mat4 childMat = *this->nodeList[0][i]->transformMatrix;
    // Se emparentan las transformaciones.
    glm::mat4 * finalTransform = new glm::mat4(parentMat * childMat);
    // Renderiza el modelo con el enparentamiento de las transformaciones.
    this->nodeList[0][i]->model->Render(*finalTransform);
  }
}

