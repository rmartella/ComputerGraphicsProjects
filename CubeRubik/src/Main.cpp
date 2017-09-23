#include <GL/glew.h>
#include "Headers/Main.h"

// Modelo del cubo.
Model g_cube;

// Vertices del cubo.
Vertex3 vertices1[] = {  //    X 	  Y	    Z      	   R	  G	   B	 A
    //FRONT ORANGE BLACK
        { { 1.0f, 1.0f, 0.99f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { 1.0f, -1.0f,
            0.99f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { -1.0f, -1.0f, 0.99f }, {
            0.0f, 0.0f, 0.0f, 1.0f } }, { { -1.0f, 1.0f, 0.99f }, { 0.0f, 0.0f,
            0.0f, 1.0f } },
        //FRONT ORANGE
        { { 0.9f, 0.9f, 1.0f }, { 1.0f, 0.4f, 0.1f, 1.0f } }, { { 0.9f, -0.9f,
            1.0f }, { 1.0f, 0.4f, 0.1f, 1.0f } }, { { -0.9f, -0.9f, 1.0f }, {
            1.0f, 0.4f, 0.1f, 1.0f } }, { { -0.9f, 0.9f, 1.0f }, { 1.0f, 0.4f,
            0.1f, 1.0f } },
        //TOP YELLOW BLACK
        { { 1.0f, 0.99f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { -1.0f, 0.99f,
            1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { -1.0f, 0.99f, -1.0f }, {
            0.0f, 0.0f, 0.0f, 1.0f } }, { { 1.0f, 0.99f, -1.0f }, { 0.0f, 0.0f,
            0.0f, 1.0f } },
        // TOP YELLOW
        { { 0.9f, 1.0f, 0.9f }, { 1.0f, 0.9f, 0.1f, 1.0f } }, { { -0.9f, 1.0f,
            0.9f }, { 1.0f, 0.9f, 0.1f, 1.0f } }, { { -0.9f, 1.0f, -0.9f }, {
            1.0f, 0.9f, 0.1f, 1.0f } }, { { 0.9f, 1.0f, -0.9f }, { 1.0f, 0.9f,
            0.1f, 1.0f } },
        //LEFT BLUE BLACK
        { { 0.99f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { 0.99f, 1.0f,
            -1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { 0.99f, -1.0f, -1.0f }, {
            0.0f, 0.0f, 0.0f, 1.0f } }, { { 0.99f, -1.0f, 1.0f }, { 0.0f, 0.0f,
            0.0f, 1.0f } },
        //LEFT BLUE
        { { 1.0f, 0.9f, 0.9f }, { 0.1f, 0.3f, 1.0f, 1.0f } }, { { 1.0f, 0.9f,
            -0.9f }, { 0.1f, 0.3f, 1.0f, 1.0f } }, { { 1.0f, -0.9f, -0.9f }, {
            0.1f, 0.3f, 1.0f, 1.0f } }, { { 1.0f, -0.9f, 0.9f }, { 0.1f, 0.3f,
            1.0f, 1.0f } },
        //BACK RED BLACK
        { { 1.0f, 1.0f, -0.99f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { -1.0f, 1.0f,
            -0.99f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { -1.0f, -1.0f, -0.99f },
            { 0.0f, 0.0f, 0.0f, 1.0f } }, { { 1.0f, -1.0f, -0.99f }, { 0.0f,
            0.0f, 0.0f, 1.0f } },
        //BACK RED
        { { 0.9f, 0.9f, -1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f } }, { { -0.9f, 0.9f,
            -1.0f }, { 0.9f, 0.0f, 0.0f, 1.0f } }, { { -0.9f, -0.9f, -1.0f }, {
            0.9f, 0.0f, 0.0f, 1.0f } }, { { 0.9f, -0.9f, -1.0f }, { 0.9f, 0.0f,
            0.0f, 1.0f } },
        //BOTTOM WHITE BLACK
        { { 1.0f, -0.99f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { 1.0f,
            -0.99f, -1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { -1.0f, -0.99f,
            -1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { -1.0f, -0.99f, 1.0f }, {
            0.0f, 0.0f, 0.0f, 1.0f } },
        //BOTTOM WHITE
        { { 0.9f, -1.0f, 0.9f }, { 0.9f, 0.9f, 0.9f, 1.0f } }, { { 0.9f, -1.0f,
            -0.9f }, { 0.9f, 0.9f, 0.9f, 1.0f } }, { { -0.9f, -1.0f, -0.9f }, {
            0.9f, 0.9f, 0.9f, 1.0f } }, { { -0.9f, -1.0f, 0.9f }, { 0.9f, 0.9f,
            0.9f, 1.0f } },
        //RIGHT GREN BLACK
        { { -0.99f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { -0.99f,
            -1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { -0.99f, -1.0f,
            -1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } }, { { -0.99f, 1.0f, -1.0f }, {
            0.0f, 0.0f, 0.0f, 1.0f } },
        //RIGHT GREN
        { { -1.0f, 0.9f, 0.9f }, { 0.1f, 0.9f, 0.2f, 1.0f } }, { { -1.0f, -0.9f,
            0.9f }, { 0.1f, 0.9f, 0.2f, 1.0f } }, { { -1.0f, -0.9f, -0.9f }, {
            0.1f, 0.9f, 0.2f, 1.0f } }, { { -1.0f, 0.9f, -0.9f }, { 0.1f, 0.9f,
            0.2f, 1.0f } } };

//Indices del cubo.
GLubyte index[] = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8, 12,
    13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20, 24, 25,
    26, 26, 27, 24, 28, 29, 30, 30, 31, 28, 32, 33, 34, 34, 35, 32, 36, 37, 38,
    38, 39, 36, 40, 41, 42, 42, 43, 40, 44, 45, 46, 46, 47, 44 };

/**
 * Funcion principal de la aplicación, debe de ser llamada en la función WindowManager.
 * @return Terimno del programa.
 */
int GLApplication::GLMain() {

  // Inicialización de la apliciación.
  Initialize();

  // Loop de la aplicación, solo se sale de esta función si el usuario pulsa Esc o cierra la ventana.
  ApplicationLoop();

  // Método que hace limpieza de los recursos utilizados por la aplicación.
  Destroy();

  return 0;
}

/**
 * Inicialización de la aplicación.
 */
void GLApplication::Initialize() {
  // Incializa y asegura que el manejador de ventanas ha sido inicializado,
  // y asi inicializar el contexto de OpenGL.
  if (!windowManager
      || windowManager->Initialize(ScreenWidth, ScreenHeight, "Cubo Rubick",
                                   false) != 0) {
    exit(-1);
  }

  // Viewport de las mismas dimensiones de la ventana.
  glViewport(0, 0, ScreenWidth, ScreenHeight);

  glEnable(GL_DEPTH_TEST);

  // Inicialización de VAOs y VBOs para el cubo.
  g_cube.Initialize(vertices1, index, 48, 72, "Shaders/Shader.vertex",
                    "Shaders/Shader.fragment");

  // Se crea la matriz de proyección de la cámara.
  // FOV  Aspect Ratio  Near / Far Planes
  camera->SetPerspective(glm::radians(60.0f),
                         ScreenWidth / (float) ScreenHeight, 0.5f, 150.f);
  camera->PositionCamera(0, 0, 6, 0, 0);

  g_cube.SetCamera(camera);

  this->rotationCubeManager->SetCamera(camera);
  this->rotationCubeManager->SetVerticesCube(vertices1);
  this->rotationCubeManager->SetIndexCube(index);
  speedRotattionFaces = 2.0f;

  this->rotationCubeManager->loadRotations();

}

/**
 * Loop de la aplicación, solo se sale de esta función si el usuario
 * pulsa Esc o cierra la ventana.
 */
void GLApplication::ApplicationLoop() {
  // Se crea el rubick con su modelo que corresponde a un cubo.
  Node rootNode = Node(&g_cube, false);
  // Se crean los nodos y la estructura gerarquica del cubo de rubick.
  RubiksCube rubikCube = RubiksCube(&rootNode);
  // Se le indica al rubick sobre que objeto se estara logeando los movimientos.
  rubikCube.SetRotationCubeManager(this->rotationCubeManager);
  // Declaración de variable para incrementar su rotación.
  float ratio1 = 0;
  bool readyReset = false;

  bool completeFaceAuto = false;
  bool completeFaceInit = false;

  while (windowManager->ProcessInput(true)) {
    // Se obtiene los FPS.
    TimeManager::Instance().CalculateFrameRate(false);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glm::mat4 parentMatrix(1.0f);
    glm::mat4 rotateRoot(glm::toMat4(gRotation));
    parentMatrix = glm::translate(parentMatrix, vec3(0, 0, -8));
    parentMatrix = parentMatrix * rotateRoot;
    rootNode.setTransformMatrix(&parentMatrix);
    // Se le settea al manejador de la orientación la matriz asociada al cubo.
    this->rotationCubeManager->SetWorldMatrix(parentMatrix);

    // Se incrmenta la rotación.
    ratio1 = ratio1 + TimeManager::Instance().DeltaTime * speedRotattionFaces;

    // Inicialización de los movimientos, de la ultima sesión.
    if (!completeFaceInit) {
      if (this->rotationCubeManager->GetMovimientosSave()->size() > 0) {
        rubikCube.rotateFaceAuto(fmod(ratio1, 1), false, &completeFaceInit);
      } else
        completeFaceInit = true;
      if (completeFaceInit)
        speedRotattionFaces = 1.0f;
    } else {
      // Se rota la cara solicitada por el usuario.
      if (!readyReset && this->rotationCubeManager->getCurrentRotation() != NULL)
        rubikCube.rotateFace(fmod(ratio1, 1), armar, &readyReset);
      else if (!readyReset && !armar)
        ratio1 = 0;
      else if (armar && !completeFaceAuto
          && this->rotationCubeManager->GetMovimientosSave()->size() > 0) {
        // Se arma el cubo de rubick.
        readyReset = true;
        speedRotattionFaces = 2.0f;
        rubikCube.rotateFaceAuto(fmod(ratio1, 1), armar, &completeFaceAuto);
      } else if (completeFaceAuto) {
        // Se completa la rotación automatica del reinicio del estado.
        readyReset = false;
        completeFaceAuto = false;
        armar = false;
        ratio1 = 0;
        speedRotattionFaces = 1.0f;
        while (this->rotationCubeManager->GetMovimientosSave()->size() > 0) {
          this->rotationCubeManager->GetMovimientosSave()->pop_back();
        }
      }
    }

    // Render del cubo Rubick
    rootNode.renderNode();

    // Intercambio de los buffers para renderizar.
    windowManager->SwapTheBuffers();
  }
}

/**
 * Método que hace limpieza de los recursos utilizados por la aplicación.
 */
void GLApplication::Destroy() {
  // Se libera memoria de los modelos creados.
  g_cube.Destroy();

  if (windowManager) {
    windowManager->Destroy();

    delete windowManager;
    windowManager = nullptr;
  }

  if (camera) {
    delete camera;
    camera = nullptr;
  }

}
