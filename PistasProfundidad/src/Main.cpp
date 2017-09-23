#include <GL/glew.h>
#include "Headers/Main.h"

const float GLApplication::nearPlane = 0.0001f;
const float GLApplication::farPlane = 12.0f;

/**
 * Constantes para la iluminación.
 */
const glm::vec4 white(1);
const glm::vec4 black(0);
const glm::vec4 ambient(0.9f, 0.9f, 0.9f, 1.0f);

Mesh::Mesh * mesh = new Mesh::Mesh("Models/suzanne.obj");

Model mon(GL_TRIANGLES);
Model environment(GL_TRIANGLES);
Model cube1(GL_TRIANGLES);
Model cube2(GL_TRIANGLES);
Model sphere(GL_TRIANGLES);

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
                                   "Pistas de profundidad.", false) != 0) {
    exit(-1);
  }

  // Viewport de las mismas dimensiones de la ventana.
  glViewport(0, 0, ScreenWidth, ScreenHeight);

  // Se habilita las banderas de OpenGL para la prueba de profundiad y
  // recorte de caras ocultas.
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // Se crea la matriz de proyección de la cámara.
  // FOV  Aspect Ratio  Near / Far Planes
  camera->SetPerspective(glm::radians(60.0),
                         ScreenWidth / (double) ScreenHeight,
                         GLApplication::nearPlane, GLApplication::farPlane);

  camera->PositionCamera(0.0f, -3.0, 3.0, 0, 0);

  // Cargamos una malla indpendiente a un modelo.
  mesh->LoadMesh();

  // Se inicializa el modelo de texeles.
  modelText->Initialize();

  rayCasting->setScale(1.0f);
  rayCasting->setOffset(0.0f);
  rayCasting->setContrast(false);

}

/**
 * Loop de la aplicación, solo se sale de esta función si el usuario
 * pulsa Esc o cierra la ventana.
 */
void GLApplication::ApplicationLoop() {

  // Declaración de variables para número de Vertices e Indices del objeto de la malla.
  int sizeVertexMesh = 0;
  int sizeIndexMesh = 0;

  // Declaración de variables para el número de Vertices e Indices del objeto de environment.
  int sizeVertexEnvironment = 34;
  int sizeIndexEnvironment = 36;

  // Declaración de variables para el número de Vertices e Indices del objeto cubo.
  int sizeIndexCube = 36;
  int sizeVertexCube = 24;

  // Declaración de variables para el radio de la esfera, como se construira la esfera, tamaño de Vertices,
  // tamaño de indices, y donde se encuentra el centro de la esfera.
  float ratioSphere = 0.75;
  int slices = 13, stacks = 13;
  int sizeVertexSphere = (slices + 1) * (stacks + 1);
  int sizeIndexSphere = (slices * stacks + slices) * 6;
  glm::vec3 centerSphere = glm::vec3(-1.5f, -4.25f, -2.0f);

  // Declaración de Vertices e indices del caja del environment.
  Vertex3 * verticesEnvironmentCube = (Vertex3 *) malloc(
      sizeof(Vertex3) * sizeVertexEnvironment);
  GLuint * indexEnvironment = (GLuint *) malloc(
      sizeof(GLuint) * sizeIndexEnvironment);
  // Declaración de Vertices e indices del primer cubo.
  Vertex3 * verticesCube = (Vertex3 *) malloc(sizeof(Vertex3) * sizeVertexCube);
  GLuint * indexCube = (GLuint *) malloc(sizeof(GLuint) * sizeIndexCube);
  // Declaración de Vertices del segundo cubo, se ocupa el mismo arreglo de indices para el segundo cubo.
  Vertex3 * verticesCube2 = (Vertex3 *) malloc(
      sizeof(Vertex3) * sizeVertexCube);
  // Declaración de Vertices e indices de la esfera.
  Vertex3 * verticesSphere = (Vertex3 *) malloc(
      sizeof(Vertex3) * (slices + 1) * (stacks + 1));
  GLuint * indexSphere = (GLuint *) malloc(
      sizeof(GLuint) * (slices * stacks + slices) * 6);

  // Se obtiene el tamaño de los objetos de la malla.
  objectCreation.getSizeMesh(mesh->GetMeshEntry(), &sizeVertexMesh,
                             &sizeIndexMesh);
  // Declaración de Vertices e indices del objeto Monkey.
  Vertex3 * verticesZ = new Vertex3[sizeVertexMesh];
  GLuint * index = new GLuint[sizeIndexMesh];
  // Se convierte el formato de la malla a formato de vertices que se le enviará a la GPU.
  // Creación de objeto Monkey.
  objectCreation.formatMesh(mesh->GetMeshEntry(), sizeVertexMesh, sizeIndexMesh,
                            verticesZ, index);
  // Creación de la caja environment.
  objectCreation.CubeEnviornment(verticesEnvironmentCube, indexEnvironment,
                                 sizeVertexEnvironment, sizeIndexEnvironment);
  // Creación del primer cubo.
  objectCreation.Cube(verticesCube, indexCube, sizeVertexCube, sizeIndexCube);
  // Creación del segundo cubo.
  memcpy(verticesCube2, verticesCube, sizeof(Vertex3) * sizeVertexCube);
  // Creación de la esfera.
  objectCreation.SolidSphere(ratioSphere, slices, stacks, verticesSphere,
                             indexSphere);

  // Declaración a nullPtr de los vertices que serán transformados, dependiendo el punto de vista de la escena.
  // Esto se hace para que no se hagan haga la transformación del mismo numero veces de pixeles de la escena
  // Por cada modelo, solo se hace una vez la transformación y se utilizan posteriormente.
  Vertex3 * verticesEnvironmentCubeTransform = nullptr;
  Vertex3 * verticesCube1Transform = nullptr;
  Vertex3 * verticesCube2Transform = nullptr;
  Vertex3 * verticesMonkeyTransform = nullptr;
  Vertex3 * verticesSphereTransform = nullptr;
  // Declaración del el centro de la esfera  transaformado y evitar hacer esa transformación muchas veces,
  glm::vec3 * centerSphereTransform = nullptr;

  // Declaración  de vertices para hacer el boundingBox de la escena para cada objeto.
  // Se toma como tamaño el mismo que el cubo.
  Vertex3 * verticesEnvironmentBoundingBox = (Vertex3 *) malloc(
      sizeof(Vertex3) * sizeVertexCube);
  Vertex3 * verticesMonkeyBoundingBox = (Vertex3 *) malloc(
      sizeof(Vertex3) * sizeVertexCube);
  Vertex3 * verticesCube1BoundingBox = (Vertex3 *) malloc(
      sizeof(Vertex3) * sizeVertexCube);
  Vertex3 * verticesCube2BoundingBox = (Vertex3 *) malloc(
      sizeof(Vertex3) * sizeVertexCube);

  // Este objeto nos permite encapsular las pruebas de intersección del BoudningBox del objeto
  // y el rayo.
  TestObjectHit * testEnvironment = nullptr;
  TestObjectHit * testMonkey = nullptr;
  TestObjectHit * testCube1 = nullptr;
  TestObjectHit * testCube2 = nullptr;
  TestObjectHit * testSphere = nullptr;

  // Debido a problemas de optimización por cada frame de la escene se calculan los triangulos que son invisibles
  // para el observador (caras ocultas), así por minimo se reduciria a la mitad el número de pruebas que se harán,
  // Se crean apuntadores de tipo booleano a las caras de los objetos, y son calculados una vez por frame.
  // True indica visble, false no visible, cada elemento de esta puntador corresponde a tres vertices que forman
  // el triangulo.
  bool * cullFacesPointerEnvi = nullptr;
  bool * cullFacesPointerMon = nullptr;
  bool * cullFacesPointerCube1 = nullptr;
  bool * cullFacesPointerCube2 = nullptr;
  bool * cullFacesPointerSphere = nullptr;

  // Se indica el número de luces que se desean manejar.
  int numLights = 1;
  // Inicialización del arregl ode luces.
  LightAtrr * light = new LightAtrr[numLights];

  // Inicialización de valores de luces.
  light[0].ambient = ambient;
  light[0].lightColor = white;
  light[0].lightPosW = glm::vec4(0, 0, -10, 0);
  light[0].materialDiffuse = white;
  light[0].materialEmissive = black;
  light[0].materialShininess = 15.0f;
  light[0].materialSpecular = white;

  // Se incializan los modelo que se ocuparan para renderizar la escena.
  environment.SetCamera(camera);
  /*environment.setNumLights(numLights);
   environment.SetLightAtrr(light);*/
  environment.Initialize(verticesEnvironmentCube, indexEnvironment,
                         sizeVertexEnvironment, sizeIndexEnvironment,
                         "Shaders/Shader.vertex.glsl",
                         "Shaders/Shader.fragment.glsl");
  mon.SetCamera(camera);
  /*mon.setNumLights(numLights);
   mon.SetLightAtrr(light);*/
  mon.Initialize(verticesZ, index, sizeVertexMesh, sizeIndexMesh,
                 "Shaders/Shader.vertex.glsl", "Shaders/Shader.fragment.glsl");
  cube1.SetCamera(camera);
  /* cube1.setNumLights(numLights);
   cube1.SetLightAtrr(light);*/
  cube1.Initialize(verticesCube, indexCube, sizeVertexCube, sizeIndexCube,
                   "Shaders/Shader.vertex.glsl",
                   "Shaders/Shader.fragment.glsl");
  cube2.SetCamera(camera);
  /*cube2.setNumLights(numLights);
   cube2.SetLightAtrr(light);*/
  cube2.Initialize(verticesCube2, indexCube, sizeVertexCube, sizeIndexCube,
                   "Shaders/Shader.vertex.glsl",
                   "Shaders/Shader.fragment.glsl");
  sphere.SetCamera(camera);
  /* sphere.setNumLights(numLights);
   sphere.SetLightAtrr(light);*/
  sphere.Initialize(verticesSphere, indexSphere, sizeVertexSphere,
                    sizeIndexSphere, "Shaders/Shader.vertex.glsl",
                    "Shaders/Shader.fragment.glsl");

  // Loop que se ejecuta mientras no se pulse Esc o cierre la ventana.
  while (windowManager->ProcessInput(true)) {
    // Se obtiene los FPS.
    double FPS = TimeManager::Instance().CalculateFrameRate(false);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Se pre-calcula la matriz del espacio del WorldSpace.
    glm::mat4 VP = camera->GetProjectionMatrix() * camera->GetViewMatrix();

    // Todos los objetos se moveran respecto al movimiento del mouse izquierdo.
    glm::mat4 rootMatrix(1.0f);
    rootMatrix = glm::toMat4(gRotation);

    // Matriz del modelo del la caja del environment.
    mat4 environmentModelMatrix = glm::translate(glm::mat4(1.0),
                                                 glm::vec3(0, 0, 0.0f));
    // Matriz del modelo del primer cubo.
    mat4 cube1ModelMatrix = glm::translate(glm::mat4(1.0),
                                           glm::vec3(0.0f, -4.5f, -4.0f));
    cube1ModelMatrix = glm::scale(cube1ModelMatrix, glm::vec3(0.5, 0.5, 0.5));
    // Matriz del modelo del segundo cubo.
    mat4 cube2ModelMatrix = glm::translate(glm::mat4(1.0),
                                           glm::vec3(1.0f, -4.25f, -3.0f));
    cube2ModelMatrix = glm::scale(cube2ModelMatrix, glm::vec3(0.5, 0.75, 0.5));
    // Matriz del modelo del modelo del Mono.
    mat4 modelMonMatrix = glm::translate(glm::mat4(1.0),
                                         glm::vec3(-0.5, -4.4, -1.0f));
    // Matriz del modelo de la esfera.
    mat4 sphereModelMatrix = glm::translate(glm::mat4(1.0), centerSphere);

    // Valida si los apuntadores tiene referencia nulla para liberar memoria y recalcular, los vertices transformados.
    if (verticesEnvironmentCubeTransform != nullptr)
      free(verticesEnvironmentCubeTransform);
    if (verticesMonkeyTransform != nullptr)
      free(verticesMonkeyTransform);
    if (testCube1 != nullptr)
      free(verticesCube1Transform);
    if (verticesCube2Transform != nullptr)
      free(verticesCube2Transform);
    if (verticesSphereTransform != nullptr)
      free(verticesSphereTransform);

    // Valida si los apuntadores a la prueba de los Bounding Box no tiene referencia nulla para liberar memoría.
    if (testEnvironment != nullptr)
      free(testEnvironment);
    if (testMonkey != nullptr)
      free(testMonkey);
    if (testCube1 != nullptr)
      free(testCube1);
    if (testCube2 != nullptr)
      free(testCube2);
    if (testSphere != nullptr)
      free(testSphere);

    // Se crea el modelo del environmente transformando sus vertices al espacio de la escena.
    verticesEnvironmentCubeTransform = objectCreation.createModelTransform(
        rootMatrix * environmentModelMatrix, verticesEnvironmentCube,
        verticesEnvironmentBoundingBox, sizeVertexEnvironment);
    // Se crea el modelo del mono transformando sus vertices al espacio de la escena.
    verticesMonkeyTransform = objectCreation.createModelTransform(
        rootMatrix * modelMonMatrix, verticesZ, verticesMonkeyBoundingBox,
        sizeIndexMesh);
    // Se crea el modelo del cubo1 transformando sus vertices al espacio de la escena.
    verticesCube1Transform = objectCreation.createModelTransform(
        rootMatrix * cube1ModelMatrix, verticesCube, verticesCube1BoundingBox,
        sizeVertexCube);
    // Se crea el modelo del cubo2 transformando sus vertices al espacio de la escena.
    verticesCube2Transform = objectCreation.createModelTransform(
        rootMatrix * cube2ModelMatrix, verticesCube2, verticesCube2BoundingBox,
        sizeVertexCube);
    // Se crea el modelo de la esfera transformando sus vertices al espacio de la escena.
    verticesSphereTransform = objectCreation.createModelTransform(
        rootMatrix * sphereModelMatrix, verticesSphere, NULL, sizeVertexSphere);

    // Se crean la banderas si una cara es oculta o no.
    cullFacesPointerEnvi = objectCreation.createCullFacesPointer(
        verticesEnvironmentCubeTransform, indexEnvironment,
        sizeIndexEnvironment, camera->GetPosition());
    cullFacesPointerMon = objectCreation.createCullFacesPointer(
        verticesMonkeyTransform, index, sizeIndexMesh, camera->GetPosition());
    cullFacesPointerCube1 = objectCreation.createCullFacesPointer(
        verticesCube1Transform, indexCube, sizeIndexCube,
        camera->GetPosition());
    cullFacesPointerCube2 = objectCreation.createCullFacesPointer(
        verticesCube2Transform, indexCube, sizeIndexCube,
        camera->GetPosition());
    cullFacesPointerSphere = objectCreation.createCullFacesPointer(
        verticesSphereTransform, indexSphere, sizeIndexSphere,
        camera->GetPosition());

    // Se crean los objetos para probar si el rayo interseca al objeto por medio del bounding box del objeto enviroment.
    testEnvironment = rayCasting->createTestObjectHit(
        verticesEnvironmentBoundingBox, indexCube, sizeIndexCube, Box, nullptr,
        0.0f);
    // Se crean los objetos para probar si el rayo interseca al objeto por medio del bounding box del objeto mono.
    testMonkey = rayCasting->createTestObjectHit(verticesMonkeyBoundingBox,
                                                 indexCube, sizeIndexCube, Box,
                                                 nullptr, 0.0f);
    // Se crean los objetos para probar si el rayo interseca al objeto por medio del bounding box del objeto cubo1.
    testCube1 = rayCasting->createTestObjectHit(verticesCube1BoundingBox,
                                                indexCube, sizeIndexCube, Box,
                                                nullptr, 0.0f);
    // Se crean los objetos para probar si el rayo interseca al objeto por medio del bounding box del objeto cubo2.
    testCube2 = rayCasting->createTestObjectHit(verticesCube2BoundingBox,
                                                indexCube, sizeIndexCube, Box,
                                                nullptr, 0.0f);
    // Se crean los objetos para probar si el rayo interseca al objeto por medio del bounding box de la esfera.
    // Primero se transforma el centro de la esfera al mismo espacio.
    centerSphereTransform = new glm::vec3(
        rootMatrix * glm::vec4(centerSphere, 1.0));
    testSphere = rayCasting->createTestObjectHit(nullptr, nullptr, 0, Sphere,
                                                 centerSphereTransform,
                                                 ratioSphere);

    // Iteración para cada pixel de la pantalla.
    for (int i = rayCasting->xBottom; i < rayCasting->xTop; i += 1) {
      for (int j = rayCasting->yBottom; j < rayCasting->yTop; j += 1) {
        // Se obtiene el origen y destino del rayo.
        glm::vec3 * v1 = new glm::vec3(), *v2 = new glm::vec3();
        rayCasting->Get3DRayUnderScrean(i, j, ScreenWidth, ScreenHeight, v1,
                                        v2);
        // Se crea la dirección del rayo.
        glm::vec3 direction = glm::normalize(*v2 - *v1);

        // Hace la prueba para el objeto environment si el rayo alcanza el objeto por su boundingBox.
        if (rayCasting->hitObjectTest(v1, v2, &direction, testEnvironment)) {
          // Si el rayo alcanza el la caja del objeto, ahora si obtenemos se obtiene la escala de grises del objeto.
          // Solo prueba las caras que no son ocultas.
          rayCasting->iterateObjectTriangle(v1, &direction, &VP,
                                            verticesEnvironmentCube,
                                            verticesEnvironmentCubeTransform,
                                            indexEnvironment,
                                            sizeIndexEnvironment,
                                            cullFacesPointerEnvi);
        }
        // Hace la prueba para el objeto mono si el rayo alcanza el objeto por su boundingBox.
        if (rayCasting->hitObjectTest(v1, v2, &direction, testMonkey)) {
          // Si el rayo alcanza el la caja del objeto, ahora si obtenemos se obtiene la escala de grises del objeto.
          // Solo prueba las caras que no son ocultas.
          rayCasting->iterateObjectTriangle(v1, &direction, &VP, verticesZ,
                                            verticesMonkeyTransform, index,
                                            sizeIndexMesh, cullFacesPointerMon);
        }
        // Hace la prueba para el objeto cubo1 si el rayo alcanza el objeto por su boundingBox.
        if (rayCasting->hitObjectTest(v1, v2, &direction, testCube1)) {
          // Si el rayo alcanza el la caja del objeto, ahora si obtenemos se obtiene la escala de grises del objeto.
          // Solo prueba las caras que no son ocultas.
          rayCasting->iterateObjectTriangle(v1, &direction, &VP, verticesCube,
                                            verticesCube1Transform, indexCube,
                                            sizeIndexCube,
                                            cullFacesPointerCube1);
        }
        // Hace la prueba para el objeto cubo2 si el rayo alcanza el objeto por su boundingBox.
        if (rayCasting->hitObjectTest(v1, v2, &direction, testCube2)) {
          // Si el rayo alcanza el la caja del objeto, ahora si obtenemos se obtiene la escala de grises del objeto.
          // Solo prueba las caras que no son ocultas.
          rayCasting->iterateObjectTriangle(v1, &direction, &VP, verticesCube2,
                                            verticesCube2Transform, indexCube,
                                            sizeIndexCube,
                                            cullFacesPointerCube2);
        }
        // Hace la prueba para el objeto esfera si el rayo alcanza el objeto por su boundingBox.
        if (rayCasting->hitObjectTest(v1, v2, &direction, testSphere)) {
          rayCasting->iterateObjectTriangle(v1, &direction, &VP, verticesSphere,
                                            verticesSphereTransform,
                                            indexSphere, sizeIndexSphere,
                                            cullFacesPointerSphere);
        }
        // Libera memoria de Vectores origen y destino del rayo.
        free(v1);
        free(v2);
      }
    }

    // Actualiza los buffers del modelo environment.
    environment.UpdateBuffer(verticesEnvironmentCube, sizeVertexEnvironment);
    // Actualiza los buffers del modelo mono.
    mon.UpdateBuffer(verticesZ, sizeVertexMesh);
    // Actualiza los buffers del modelo cubo1.
    cube1.UpdateBuffer(verticesCube, sizeVertexCube);
    // Actualiza los buffers del modelo cubo2.
    cube2.UpdateBuffer(verticesCube2, sizeVertexCube);
    // Actualiza los buffers del modelo esfera.
    sphere.UpdateBuffer(verticesSphere, sizeVertexSphere);
    // Renderizado de objetos, se toman las transformaciones de los modelos transformados.
    environment.Render(rootMatrix * environmentModelMatrix);
    mon.Render(rootMatrix * modelMonMatrix);
    cube1.Render(rootMatrix * cube1ModelMatrix);
    cube2.Render(rootMatrix * cube2ModelMatrix);
    sphere.Render(rootMatrix * sphereModelMatrix);

    // Si el mouse se encuentra en la posición de la vetana.
    // Calcula la distancia de profundiad de la escena.
    if (this->mousePos->x >= 0 && this->mousePos->y >= 0) {
      int objects = 5;
      float * distance = new float[objects];

      // Obtiene la distancia al objeto environment.
      distance[0] = rayCasting->getDistanceScaleValue(
          this->mousePos, verticesEnvironmentCubeTransform, indexEnvironment,
          sizeIndexEnvironment, this->ScreenWidth, this->ScreenHeight, &VP,
          testEnvironment, cullFacesPointerEnvi);
      // Obtiene la distancia al objeto mono.
      distance[1] = rayCasting->getDistanceScaleValue(this->mousePos,
                                                      verticesMonkeyTransform,
                                                      index, sizeIndexMesh,
                                                      this->ScreenWidth,
                                                      this->ScreenHeight, &VP,
                                                      testMonkey,
                                                      cullFacesPointerMon);
      // Obtiene la distancia al objeto cubo1.
      distance[2] = rayCasting->getDistanceScaleValue(this->mousePos,
                                                      verticesCube1Transform,
                                                      indexCube, sizeIndexCube,
                                                      this->ScreenWidth,
                                                      this->ScreenHeight, &VP,
                                                      testCube1,
                                                      cullFacesPointerCube1);
      // Obtiene la distancia al objeto cubo2.
      distance[3] = rayCasting->getDistanceScaleValue(this->mousePos,
                                                      verticesCube2Transform,
                                                      indexCube, sizeIndexCube,
                                                      this->ScreenWidth,
                                                      this->ScreenHeight, &VP,
                                                      testCube2,
                                                      cullFacesPointerCube2);
      // Obtiene la distancia al objeto esfera.
      distance[4] = rayCasting->getDistanceScaleValue(this->mousePos,
                                                      verticesSphereTransform,
                                                      indexSphere,
                                                      sizeIndexSphere,
                                                      this->ScreenWidth,
                                                      this->ScreenHeight, &VP,
                                                      testSphere,
                                                      cullFacesPointerSphere);

      // Determina que distancia de los objetos es la mas cercana al observador.
      float d = -1;
      for (int i = 0; i < objects; i++) {
        if (d == -1 || (distance[i] > -1 && distance[i] < d))
          d = distance[i];
      }

      // Render de la distancia.
      std::ostringstream ss;
      ss << d;
      std::string text = "Distancia:" + ss.str();
      modelText->render(text, -1.0, 0.82);
    }
    // Render de los FPS.
    std::ostringstream * convert = new std::ostringstream();
    *convert << "FPS : " << FPS;
    std::string str = convert->str();
    modelText->render(str, -1.0, 0.9);
    free(convert);

    // Intercambio de los buffers para renderizar.
    windowManager->SwapTheBuffers();
  }
}

/**
 * Método que hace limpieza de los recursos utilizados por la aplicación.
 */
void GLApplication::Destroy() {
  // Se libera memoria de los modelos creados.
  environment.Destroy();
  mon.Destroy();
  cube1.Destroy();
  cube2.Destroy();
  sphere.Destroy();

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
