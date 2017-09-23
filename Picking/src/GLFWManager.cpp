#include <GL/glew.h>
#include "Headers/GLFWManager.h"
#include "Headers/Main.h"

GLApplication * application;
Picking::Picking * picking;
Camera * pCamera;

int main() {
  // Se crea el manejador de ventanas y eventos de OpenGL.
  GLFWManager *pWindowManager = new GLFWManager();

  // Se crea la cámara que usaremos para la aplicación.
  pCamera = new Camera();
  // Se crean los objetos aplicación y el objeto que tiene las funcionalidades del picking.
  picking = new Picking::Picking();
  application = new GLApplication();

  // Asocia el manejador de ventanas a la aplicación.
  application->SetWindowManager(pWindowManager);

  // Asocia la cámara que usará la aplicación.
  application->SetCamera(pCamera);
  // Se asocia la cámara al manejador de entrada y poder manipular la cámara.
  pWindowManager->GetInputManager()->SetCamera(pCamera);

  picking->SetCamera(pCamera);
  application->SetPicking(picking);

  // Se llama el método principal de la aplicación.
  return application->GLMain();
}

/**
 * Inicializa la ventana y crea el contexto de OpenGL.
 * @param width Ancho de ventana.
 * @param height Altura de ventana.
 * @param strTitle Titulo de ventana.
 * @param bFullScreen Indica si se requiere ventana completa o no.
 * @return Bandera que indica la inicialización correcta de la ventana y el contexto de OpenGL.
 */
int GLFWManager::Initialize(int width, int height, std::string strTitle,
                            bool bFullScreen) {

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    return -1;
  }

  // Se le dice a openGL que se quiere utilizar un multi-muestro de 4 para el anti-aliasing.
  glfwWindowHint(GLFW_SAMPLES, 4);

  // Contexto de OpenGL versión 4.2 usando el core profile.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
        "Error al crear GLFW window, puedes probar descargando la ultima versión de los drivers que soporte OpenGL 3.3\n");
    Destroy();

    return -1;
  }

  // Se crea el contexto de OpenGL.
  glfwMakeContextCurrent(Window);

  glfwSetInputMode(Window, GLFW_STICKY_KEYS, GL_TRUE);

  // Apaga vertical sync para que el monitor no espere a renderizar.
  glfwSwapInterval(0);

  glewExperimental = GL_TRUE;

  // Se coloca el callback a la función para redimensionar.
  glfwSetWindowSizeCallback(Window, GLFWwindowsizefun(GLFWManager::ReshapeGL));

  // Incialización de la libreria GLEW para agrega funciones y extensiones de OpenGL.
  GLenum err = glewInit();

  if (GLEW_OK != err) {
    fprintf(stderr, "Failed to initialize glew\n");
    return -1;
  }

  return 0;
}

/**
 * Intercambia el backbuffer al frente, se debe estar llamando este función por cada frame.
 */
void GLFWManager::SwapTheBuffers() {
  glfwSwapBuffers(Window);
}

/**
 * Método que maneja los eventos de entrada de la aplicación.
 * @param continueApplication Bandera que indica la ejecución de la aplicacaión.
 * @return True siempre que no se apriete la tecal Esc, falso en caso contrario.
 */
bool GLFWManager::ProcessInput(bool continueApplication = true) {
  // Revisa si fue presionado el botton Esc.
  if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS
      || glfwWindowShouldClose(Window) != 0)
    return false;

  // Posición en coordenadas de la ventan del cursor
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

  // Habilita el trazado de rayor grafico.
  if (glfwGetKey(Window, GLFW_KEY_T) || glfwGetKey(Window, GLFW_KEY_T))
    application->setEnableRayCastingGL(true);
  // Deshabilita el trazado de rayor grafico.
  if (glfwGetKey(Window, GLFW_KEY_Y) || glfwGetKey(Window, GLFW_KEY_Y))
    application->setEnableRayCastingGL(false);
  // Aumenta el tamaño de landmark
  if (glfwGetKey(Window, GLFW_KEY_2)) {
    if (application->getSizeLandMark() + 0.0001 < 0.1) {
      application->setSizeLandMark(application->getSizeLandMark() + 0.0001f);
    }
  }
  // Disminuye el taño del landmak.
  if (glfwGetKey(Window, GLFW_KEY_1)) {
    if (application->getSizeLandMark() - 0.0001 > 0.01) {
      application->setSizeLandMark(application->getSizeLandMark() - 0.0001f);
    }
  }
  // Habilita / Deshabilita la distancia de dos landmarks
  int statetShift = glfwGetKey(Window, GLFW_KEY_LEFT_SHIFT);
  if (statetShift == GLFW_PRESS) {
    application->setDistanceEnable(true);
  } else {
    application->setDistanceEnable(false);
    application->setNumMarks(0);
    application->getMarksToDistance()->clear();
  }
  // Habilita / Deshabilita eliminar un landmark
  int stateControl = glfwGetKey(Window, GLFW_KEY_LEFT_CONTROL);
  if (stateControl == GLFW_PRESS)
    application->setDeleteEnable(true);
  else
    application->setDeleteEnable(false);

  int left, right, top, bottom;
  glfwGetWindowFrameSize(Window, &left, &top, &right, &bottom);
  glm::ivec2 * mousePos = new glm::ivec2();
  mousePos->x = xpos;
  mousePos->y = ypos;
  int stateRight = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_RIGHT);
  // Boton derecho liberado.
  if (stateRight == GLFW_RELEASE) {
    // Coloca coordenadas del cursos del mouse en coordenadas de ventana.
    inputManager.SetGMousePos(mousePos);
    if (mousePos->x >= 0 && mousePos->y >= 0
        && mousePos->x <= application->ScreenWidth
        && mousePos->y <= application->ScreenHeight)
      application->SetMousePos(mousePos->x, mousePos->y);
    else
      application->SetMousePos(-1, -1);
  } else {
    // Click derecho se obtiene la matriz de rotación.
    glm::ivec2 * gMousePos = inputManager.getGMousePos();
    glm::vec2 delta = glm::vec2(
        glm::ivec2(mousePos->x, mousePos->y)
            - glm::ivec2(gMousePos->x, gMousePos->y));
    inputManager.SetGMousePos(mousePos);

    glm::quat rotX = glm::angleAxis<float>(glm::radians(delta.y) * 0.1f,
                                           glm::vec3(1, 0, 0));
    glm::quat rotY = glm::angleAxis<float>(glm::radians(delta.x) * 0.1f,
                                           glm::vec3(0, 1, 0));

    application->SetGRotation((rotX * rotY) * application->GetGRotation());
  }

  int stateLeft = glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT);
  // Click derecho se agrega un click al picking.
  if (stateLeft == GLFW_PRESS && !clicked) {
    clicked = true;
    Picking::Click * click = (Picking::Click *) malloc(sizeof(Picking::Click));
    click->x = mousePos->x;
#ifdef _WIN32
    click->y = application->ScreenHeight - mousePos->y;
#endif
#if defined(__linux__)
    click->y = application->ScreenHeight - mousePos->y;
#endif
    picking->GetClicks()->push_back(click);
  } else if (stateLeft == GLFW_RELEASE && clicked) {
    clicked = false;
  }

  glfwPollEvents();

  return continueApplication;
}

/**
 * Método que maneja el redimensionamiento de la ventana.
 * @param Window Ventana que se va a redimencionar.
 * @param widthRes Ancho objetivo.
 * @param heightRes Altura objetivo.
 */
void GLFWManager::ReshapeGL(GLFWwindow* Window, int widthRes, int heightRes) {
  application->ScreenWidth = widthRes;
  application->ScreenHeight = heightRes;
  glViewport(0, 0, application->ScreenWidth, application->ScreenHeight);

  pCamera->SetPerspective(
      glm::radians(60.0f),
      application->ScreenWidth / (float) application->ScreenHeight, 0.0001f,
      150.0f);
}

/**
 * Destruye la ventana y el contexto de OpenGL.
 */
void GLFWManager::Destroy() {
  // Cierra la ventana de OpenGL y finaliza la aplicación.
  glfwTerminate();
}
