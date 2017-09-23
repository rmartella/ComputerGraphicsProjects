#ifndef _GLFWManager_H
#define _GLFWManager_H

#include <string>
#include <fstream>
#include <GLFW/glfw3.h>
#include "WindowManager.h"
#include <iostream>

/**
 * Implementaci�n de la clase abstracta WindowManager, utliza la liberia GLFW para crear ventanas,
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
   * @return Bandera que indica la inicializaci�n correcta de la ventana y el contexto de OpenGL.
   */
  virtual int Initialize(int width, int height, std::string strTitle,
                         bool bFullScreen = false);

  /**
   * Intercambia el backbuffer al frente, se debe estar llamando este funci�n por cada frame.
   */
  virtual void SwapTheBuffers();

  /**
   * M�todo que maneja los eventos de entrada de la aplicaci�n.
   * @param continueApplication Bandera que indica la ejecuci�n de la aplicacai�n.
   * @return True siempre que no se apriete la tecal Esc, falso en caso contrario.
   */
  virtual bool ProcessInput(bool continueGame);

  /**
   * M�todo que maneja el redimensionamiento de la ventana.
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
