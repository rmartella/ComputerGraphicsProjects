#ifndef _WindowManager_H
#define _WindowManager_H

#include <string>
#include <fstream>
#include "InputManager.h"
/**
 * Esta clase es una abstracción de herencia, especifica los metodos que
 * Se deben implementar para el la creación del contexto de OpenGL.
 * Se puede tener una clase que implementé el manejo de ventanas, entradas por
 * otra libreria, solo se debe de incluir y apegar a esta clase.
 */
class WindowManager {
 public:

  /**
   * Destructor.
   */
  virtual ~WindowManager() {
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
                         bool bFullScreen = false) = 0;

  /**
   * Intercambia el backbuffer al frente, se debe estar llamando este función por cada frame.
   */
  virtual void SwapTheBuffers() = 0;

  /**
   * Método que maneja los eventos de entrada de la aplicación.
   * @param continueApplication Bandera que indica la ejecución de la aplicacaión.
   * @return True siempre que no se apriete la tecal Esc, falso en caso contrario.
   */
  virtual bool ProcessInput(bool continueApplication) = 0;

  /**
   * Destruye la ventana y el contexto de OpenGL.
   */
  virtual void Destroy() = 0;

  /**
   * Obtiene el manejdaor de eventos de entrada, esto con fines de que accedan
   * a los evetos de entrada y se le indique la funcionalidad a realizar.
   * @return
   */
  virtual InputManager *GetInputManager() {
    return &inputManager;
  }

 protected:

  // Atributo que controla los movimientos de la cámara.
  InputManager inputManager;

};

#endif
