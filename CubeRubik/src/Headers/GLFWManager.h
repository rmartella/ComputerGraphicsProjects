#ifndef _GLFWManager_H
#define _GLFWManager_H

#include <string>
#include <fstream>
#include <GLFW/glfw3.h>
#include "WindowManager.h"
#include <iostream>

/**
 * Implementación de la clase abstracta WindowManager, utliza la liberia GLFW para crear ventanas,
 * manejar eventos de entrada y crear contextos de OpenGL.
 */
class GLFWManager : public WindowManager {
 public:

  /**
   * Constructor por default.
   */
  GLFWManager() {
  }
  /**
   * Destruye la ventana y el contexto de OpenGL.
   */
  ~GLFWManager() {
    Destroy();
  }

  /**
   * Inicializa la ventana y crea el contexto de OpenGL.
   * @param width Ancho de ventana.
   * @param height Altura de ventana.
   * @param strTitle Titulo de ventana.
   * @param bFullScreen Indica si se requiere ventana completa o no.
   * @return Bandera que indica la inicialización correcta de la ventana y el contexto de OpenGL.
   */
  virtual int Initialize(int width, int height, std::string strTitle,
                         bool bFullScreen = false);

  /**
   * Intercambia el backbuffer al frente, se debe estar llamando este función por cada frame.
   */
  virtual void SwapTheBuffers();

  /**
   * Método que maneja los eventos de entrada de la aplicación.
   * @param continueApplication Bandera que indica la ejecución de la aplicacaión.
   * @return True siempre que no se apriete la tecal Esc, falso en caso contrario.
   */
  virtual bool ProcessInput(bool continueGame);

  /**
   * Método que maneja el redimensionamiento de la ventana.
   * @param Window Ventana que se va a redimencionar.
   * @param widthRes Ancho objetivo.
   * @param heightRes Altura objetivo.
   */
  void static ReshapeGL(GLFWwindow* Window, int widthRes, int heightRes);

  /**
   * Destruye la ventana y el contexto de OpenGL.
   */
  virtual void Destroy();

 protected:

  // Objeto GLFW window que maneja las ventanas y entradas.
  GLFWwindow* Window;
  bool clicked = false;
};

#endif
