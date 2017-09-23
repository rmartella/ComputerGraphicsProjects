#include <GL/glew.h>
#include "Headers/Main.h"

/**
 * Constantes para la iluminación.
 */
const glm::vec4 white(1);
const glm::vec4 black(0);
const glm::vec4 ambient(0.7f, 0.7f, 0.7f, 1.0f);

Model g_quad1;

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
      || windowManager->Initialize(ScreenWidth, ScreenHeight,
                                   "Origami", false) != 0) {
    exit(-1);
  }

  // Viewport de las mismas dimensiones de la ventana.
  glViewport(0, 0, ScreenWidth, ScreenHeight);

  // Se deshabilita la bandera de OpenGL para que no muestre las caras ocultas,
  // debido aque se requiere mostrar ambas caras del modelo.
  glFrontFace(GL_CCW);
  glDisable(GL_CULL_FACE);
  // Se habilitan las banderas de OpenGL para la prueba de profundiad.
  glEnable(GL_DEPTH_TEST);

  // Inicialización del modelo, los atributos de vertices estan almacenados,
  // en nuestra clase personalizada: Vertice, UVs por ahora es suficiente.
  Vertex3 vertices1[] = {
  //Vertex1
      { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
      //Vertex2
      { { 0.0f, 0.5f, 0.0f }, { 0.0f, 1.0f } },
      //Vertex3
      { { 0.5f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
      //Vertex4
      { { 0.5f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
      //Vertex5
      { { 0.0f, 0.5f, 0.0f }, { 0.0f, 1.0f } },
      //Vertex6
      { { 0.5f, 0.5f, 0.0f }, { 1.0f, 1.0f } } };

  // Se calcula las normales tomando los vertices de la cara,
  // obteniendo el producto cruz con los vertices de esta.
  for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(vertices1); i += 3) {
    glm::vec3 v1 = vertices1[i + 1].xyz - vertices1[i].xyz;
    glm::vec3 v2 = vertices1[i + 2].xyz - vertices1[i].xyz;
    glm::vec3 Normal = glm::cross(v2, v1);
    Normal = glm::normalize(Normal);

    vertices1[i].normal += glm::normalize(Normal);
    vertices1[i + 1].normal += glm::normalize(Normal);
    vertices1[i + 2].normal += glm::normalize(Normal);

  }

  // Se indica el número de luces que se desean manejar.
  int numLights = 1;
  // Inicialización del arreglo de luces.
  LightAtrr * light = new LightAtrr[numLights];

  // Inicialización de valores de luces.
  light[0].ambient = ambient;
  light[0].lightColor = glm::vec4(1.0f, 0.9f, 0.1f, 1.0f);
  light[0].lightPosW = glm::vec4(0, 0, -10.0, 0);
  light[0].materialDiffuse = white;
  light[0].materialEmissive = black;
  light[0].materialShininess = 20.0f;
  light[0].materialSpecular = white;

  // Se settean número de luces y atributos de la iluminación..
  g_quad1.SetLightAtrr(light);
  g_quad1.setNumLights(1);

  // Se crea la matriz de proyección de la cámara.
  // FOV  Aspect Ratio  Near / Far Planes
  camera->SetPerspective(glm::radians(60.0f),
                         ScreenWidth / (float) ScreenHeight, 0.5f, 150.f);

  camera->PositionCamera(0, 0, 2, 0, 0);

  g_quad1.SetCamera(camera);

  // Inicialización de VAOs y VBOs
  g_quad1.Initialize(vertices1, 6, "Shaders/Light.Shader.vertex",
                     "Shaders/Light.Shader.fragment");

}

/**
 * Loop de la aplicación, solo se sale de esta función si el usuario
 * pulsa Esc o cierra la ventana.
 */
void GLApplication::ApplicationLoop() {
  float faceRotate[5] = { 0, 0, 0, 0, 0 };
  int indice = 0;
  float ratiox = 0;
  float ratioy = 0;
  bool direction = 0;
  auto radians90 = glm::radians(90.0f);
  // Loop que se ejecuta mientras no se pulse Esc o cierre la ventana.
  while (windowManager->ProcessInput(true)) {
    // Se obtiene los FPS.
    TimeManager::Instance().CalculateFrameRate(false);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.89f, 0.89f, 0.89f, 1.0f);

    // Se calcula la siguiente rotación de una cara
    // Cuando llega a 90° incrementa el indice para indicar la siguiente cara.
    if (indice < 5 && !direction) {
      // Velocidad de giro.
      auto speed = radians90 * TimeManager::Instance().DeltaTime * 0.3f;
      faceRotate[indice] += speed;
      if (faceRotate[indice] >= radians90) {
        faceRotate[indice] = radians90;
        indice++;
      }
    } else if (indice >= 5 && !direction) {
      // Completo la rotación de las caras por lo que se procese a regresar
      // los indices para desarmar el cubo.
      direction = 1;
      indice -= 1;
    }
    if (indice >= 0 && direction) {
      // Velocidad de giro.
      auto speed = radians90 * TimeManager::Instance().DeltaTime * 0.3f;
      faceRotate[indice] -= speed;
      if (faceRotate[indice] <= 0) {
        faceRotate[indice] = 0;
        indice--;
      }
    } else if (indice < 0 && direction) {
      // Completo la rotación de las caras por lo que se procese a incrementar
      // los indices para armar otra cara del cubo.
      direction = 0;
      indice++;
    }

    // Rotación de la escena del cubo.
    glm::mat4 rootMatrix = glm::toMat4(gRotation);
    // Matriz root de cubo.
    glm::mat4 parentMatrix(1.0f);

    // Se renderiza la cara que no se mueve del cubo.
    g_quad1.SetScale(vec3(1, 1, 1));
    g_quad1.SetPosition(vec3(-0.25, -0.25, 0));
    g_quad1.SetRotation(vec3(0, 0, 0));
    g_quad1.Render(parentMatrix * rootMatrix);

    // Se renderiza la siguiente cara en base a la matriz
    // de transformación de la primera, se multplica para poder girar la escena.
    g_quad1.SetPosition(vec3(0.25, -0.25, 0));
    g_quad1.SetRotation(vec3(0, -faceRotate[3], 0));
    g_quad1.Render(parentMatrix * rootMatrix);

    glm::mat4 parentMatrix1 = translate(parentMatrix * rootMatrix,
                                        g_quad1.GetPosition());


    // Matriz de transformación para la siguiente cara que se moverá
    // emparentada a la anterior.
    parentMatrix1 = scale(parentMatrix1, g_quad1.GetScale());
    parentMatrix1 = rotate(parentMatrix1, g_quad1.GetRotation().x,
                           vec3(1, 0, 0));		// x-axis
    parentMatrix1 = rotate(parentMatrix1, g_quad1.GetRotation().y,
                           vec3(0, 1, 0));		// y-axis
    parentMatrix1 = rotate(parentMatrix1, g_quad1.GetRotation().z,
                           vec3(0, 0, 1));		// z-axis

    g_quad1.SetPosition(vec3(0.5, 0, 0));
    g_quad1.SetRotation(vec3(0, -faceRotate[4], 0));
    g_quad1.Render(parentMatrix1);

    // Se crean las transformaciones para la siguinte cara.
    g_quad1.SetScale(vec3(-1, 1, 1));
    g_quad1.SetPosition(vec3(-0.25, -0.25, 0));
    g_quad1.SetRotation(vec3(0, -faceRotate[1], 0));
    g_quad1.Render(parentMatrix * rootMatrix);

    // Se crean las transformaciones para la siguinte cara.
    g_quad1.SetScale(vec3(1, -1, 1));
    g_quad1.SetPosition(vec3(-0.25, -0.25, 0));
    g_quad1.SetRotation(vec3(faceRotate[2], 0, 0));
    g_quad1.Render(parentMatrix * rootMatrix);

    // Se crean las transformaciones para la siguinte cara.
    g_quad1.SetScale(vec3(1, 1, 1));
    g_quad1.SetPosition(vec3(-0.25, 0.25, 0));
    g_quad1.SetRotation(vec3(faceRotate[0], 0, 0));
    g_quad1.Render(parentMatrix * rootMatrix);

    // Intercambio de los buffers para renderizar.
    windowManager->SwapTheBuffers();
  }
}

/**
 * Método que hace limpieza de los recursos utilizados por la aplicación.
 */
void GLApplication::Destroy() {
  // Se libera memoria de los modelos creados.
  g_quad1.Destroy();

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
