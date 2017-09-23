#include <GL/glew.h>
#include "Headers/GLFWManager.h"
#include "Headers/Main.h"

GLApplication application;
Camera *pCamera;

int main() {
  // Se crea el manejador de ventanas y eventos de OpenGL.
  GLFWManager *pWindowManager = new GLFWManager();

  // Se crea la c�mara que usaremos para la aplicaci�n.
  pCamera = new Camera();

  // Asocia el manejador de ventanas a la aplicaci�n.
  application.SetWindowManager(pWindowManager);

  // Asocia la c�mara que usar� la aplicaci�n.
  application.SetCamera(pCamera);
  // Se asocia la c�mara al manejador de entrada y poder manipular la c�mara.
  pWindowManager->GetInputManager()->SetCamera(pCamera);

  // Se llama el m�todo principal de la aplicaci�n.
  return application.GLMain();
}

/**
 * Inicializa la ventana y crea el contexto de OpenGL.
 * @param width Ancho de ventana.
 * @param height Altura de ventana.
 * @param strTitle Titulo de ventana.
 * @param bFullScreen Indica si se requiere ventana completa o no.
 * @return Bandera que indica la inicializaci�n correcta de la ventana y el contexto de OpenGL.
 */
int GLFWManager::Initialize(int width, int height, std::string strTitle,
                            bool bFullScreen) {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  // Se le dice a openGL que se quiere utilizar un multi-muestro de 4 para el anti-aliasing.
  glfwWindowHint(GLFW_SAMPLES, 4);

  // Contexto de OpenGL versi�n 4.2 usando el core profile.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Se crean las ventanas.
  if (bFullScreen)
    Window = glfwCreateWindow(width, height, strTitle.c_str(),
                              glfwGetPrimaryMonitor(), nullptr);
  else
    Window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
                              nullptr);

  // Se aegura que sea una ventana valida, que no exista error.
  if (Window == nullptr) {
    fprintf(
        stderr,
        "Error al crear GLFW window, puedes probar descargando la ultima versi�n de los drivers que soporte OpenGL 3.3\n");
    Destroy();

    return -1;
  }

  // Se crea el contexto de OpenGL.
  glfwMakeContextCurrent(Window);

  glfwSetInputMode(Window, GLFW_STICKY_KEYS, GL_TRUE);

  // Apaga vertical sync para que el monitor no espere a renderizar.
  glfwSwapInterval(0);

  glewExperimental = GL_TRUE;

  // Se coloca el callback a la funci�n para redimensionar.
  glfwSetWindowSizeCallback(Window, GLFWwindowsizefun(GLFWManager::ReshapeGL));

  // Se coloca el callback a la funci�n para redimensionar.
  GLenum err = glewInit();

  // Incializaci�n de la libreria GLEW para agrega funciones y extensiones de OpenGL.
  if (GLEW_OK != err) {
    fprintf(stderr, "Failed to initialize glew\n");
    return -1;
  }

  return 0;
}

/**
 * Intercambia el backbuffer al frente, se debe estar llamando este funci�n por cada frame.
 */
void GLFWManager::SwapTheBuffers() {
  glfwSwapBuffers(Window);
}

/**
 * M�todo que maneja los eventos de entrada de la aplicaci�n.
 * @param continueApplication Bandera que indica la ejecuci�n de la aplicacai�n.
 * @return True siempre que no se apriete la tecal Esc, falso en caso contrario.
 */
bool GLFWManager::ProcessInput(bool continueApplication = true) {
  // Revisa si fue presionado el botton Esc.
  if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS
      || glfwWindowShouldClose(Window) != 0)
    return false;

  // Posici�n en coordenadas de la ventan del cursor
  double xpos;
  double ypos;
  glfwGetCursorPos(Window, &xpos, &ypos);

  if (glfwGetKey(Window, GLFW_KEY_UP) || glfwGetKey(Window, GLFW_KEY_W))
    inputManager.KeyPressed(InputCodes::Up);
  if (glfwGetKey(Window, GLFW_KEY_DOWN) || glfwGetKey(Window, GLFW_KEY_S))
    inputManager.KeyPressed(InputCodes::Down);
  if (glfwGetKey(Window, GLFW_KEY_LEFT) || glfwGetKey(Window, GLFW_KEY_A))
    inputManager.KeyPressed(InputCodes::Left);
  if (glfwGetKey(Window, GLFW_KEY_RIGHT) || glfwGetKey(Window, GLFW_KEY_D))
    inputManager.KeyPressed(InputCodes::Right);
  if (glfwGetKey(Window, GLFW_KEY_W))
    inputManager.KeyPressed(InputCodes::Up);
  if (glfwGetKey(Window, GLFW_KEY_S))
    inputManager.KeyPressed(InputCodes::Down);
  if (glfwGetKey(Window, GLFW_KEY_Q))
    inputManager.KeyPressed(InputCodes::Q);
  if (glfwGetKey(Window, GLFW_KEY_E))
    inputManager.KeyPressed(InputCodes::E);
  if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_MIDDLE)) {
    if (xpos != 0 && ypos != 0) {
      glm::vec2 delta = glm::vec2(
          glm::ivec2(xpos, ypos)
              - glm::ivec2(inputManager.getGMousePos()->x,
                           inputManager.getGMousePos()->y));
      inputManager.MouseMoved(delta.x, delta.y);
    }
  }

  glm::ivec2 * mousePos = new glm::ivec2();
  mousePos->x = xpos;
  mousePos->y = ypos;
  int stateLeft = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT);
  // Boton izquierdo liberado.
  if (stateLeft == GLFW_RELEASE) {
    // Coloca coordenadas del cursos del mouse en coordenadas de ventana.
    inputManager.SetGMousePos(mousePos);
  } else {
    // Click izquierdo se obtiene la matriz de rotaci�n.
    glm::ivec2 * gMousePos = inputManager.getGMousePos();
    glm::vec2 delta = glm::vec2(
        glm::ivec2(mousePos->x, mousePos->y)
            - glm::ivec2(gMousePos->x, gMousePos->y));
    inputManager.SetGMousePos(mousePos);

    glm::quat rotX = glm::angleAxis<float>(glm::radians(delta.y) * 0.1f,
                                           glm::vec3(1, 0, 0));
    glm::quat rotY = glm::angleAxis<float>(glm::radians(delta.x) * 0.1f,
                                           glm::vec3(0, 1, 0));

    application.SetGRotation((rotX * rotY) * application.GetGRotation());
  }

  glfwPollEvents();

  return continueApplication;
}

/**
 * M�todo que maneja el redimensionamiento de la ventana.
 * @param Window Ventana que se va a redimencionar.
 * @param widthRes Ancho objetivo.
 * @param heightRes Altura objetivo.
 */
void GLFWManager::ReshapeGL(GLFWwindow* Window, int widthRes, int heightRes) {
  application.ScreenWidth = widthRes;
  application.ScreenHeight = heightRes;
  glViewport(0, 0, application.ScreenWidth, application.ScreenHeight);

  pCamera->SetPerspective(
      glm::radians(60.0f),
      application.ScreenWidth / (float) application.ScreenHeight, 0.5f, 150.0f);
}

/**
 * Destruye la ventana y el contexto de OpenGL.
 */
void GLFWManager::Destroy() {
  glfwTerminate();
}
