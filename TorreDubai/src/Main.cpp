#include <GL/glew.h>
#include "Headers/Main.h"
#include "Headers/SkyBoxModel.h"

const glm::vec4 white(1);
const glm::vec4 black(0);
const glm::vec4 ambient(0.9f, 0.9f, 0.9f, 1.0f);

int GLApplication::GLMain() {
  // Inicialización de la apliciación.
  Initialize();

  // Loop de la aplicación, solo se sale de esta función si el usuario pulsa Esc o cierra la ventana.
  ApplicationLoop();

  // Método que hace limpieza de los recursos utilizados por la aplicación.
  Destroy();

  return 0;
}

// This function initializes the window, the shaders and the triangle vertex data.
void GLApplication::Initialize() {
  // Incializa y asegura que el manejador de ventanas ha sido inicializado,
  // y asi inicializar el contexto de OpenGL.
  if (!windowManager
      || windowManager->Initialize(ScreenWidth, ScreenHeight,
                                   "Torre Dubai.", false) != 0) {
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
                         ScreenWidth / (double) ScreenHeight, 0.0001f, 350.0f);

  camera->PositionCamera(0, -3.5, 2.0, 0, 0);

  LightAtrr * light = new LightAtrr();

  light->ambient = ambient;
  light->lightColor = white;
  light->lightPosW = glm::vec4(0, -10.0, -10.0, 0);
  light->materialDiffuse = white;
  light->materialEmissive = black;
  light->materialShininess = 15.0f;
  light->materialSpecular = white;

  std::string texturePrefix = "cloud9_";
  std::string textureExtension = ".tga";

  skyBoxModel = new SkyBoxModel();
  skyBoxModel->Initialize("Texture", texturePrefix + "right" + textureExtension,
                          texturePrefix + "left" + textureExtension,
                          texturePrefix + "top" + textureExtension,
                          texturePrefix + "bot" + textureExtension,
                          texturePrefix + "front" + textureExtension,
                          texturePrefix + "back" + textureExtension,
                          "Shaders/skybox.Shader.vertex.glsl",
                          "Shaders/skybox.Shader.fragment.glsl");
  skyBoxModel->SetCamera(camera);

  GLuint indices[] = { 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8,
      12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
      24, 25, 26, 26, 27, 24, 28, 29, 30, 30, 31, 28, 32, 33, 34, 34, 35, 32,
      36, 37, 38, 38, 39, 36, 40, 41, 42, 42, 43, 40, 44, 45, 46, 46, 47, 44 };

  Vertex3 verticesPlaneEnvi[] = { { { -1.0000f, 0.0000f, 1.0000f },
      { 0.0f, 0.0f }, { 0.0000f, 1.0000f, 0.0000f } }, { { 1.0000f, 0.0000f,
      1.0000f }, { 1.0f, 0.0f }, { 0.0000f, 1.0000f, 0.0000f } }, { { 1.0000f,
      0.0000f, -1.0000f }, { 1.0f, 1.0f }, { 0.0000f, 1.0000f, 0.0000f } }, { {
      -1.0000f, 0.0000f, -1.0000f }, { 0.0f, 1.0f },
      { 0.0000f, 1.0000f, 0.0000f } } };

  // Modelo que se va a reflejar.
  modelEnvironment = new ModelEnvironment();
  Mesh::Mesh * mesh = new Mesh::Mesh("Models/level2.2.1.obj");
  modelEnvironment->SetCamera(camera);
  //modelEnironment->SetLightAtrr(light);
  modelEnvironment->SetMesh(mesh);
  modelEnvironment->Initialize(
      NULL, NULL, 0, 0, "Shaders/Environment.Reflection.Shader.vertex.glsl",
      "Shaders/Environment.Reflection.Shader.fragment.glsl");

  // Modelo solido.
  modelSolid = new Model(GL_TRIANGLES, "Texture/metal.jpg");
  Mesh::Mesh * meshBox = new Mesh::Mesh("Models/level2.2.2.obj");
  modelSolid->SetCamera(camera);
  modelSolid->setNumLights(1);
  modelSolid->SetLightAtrr(light);
  modelSolid->SetMesh(meshBox);
  modelSolid->Initialize(NULL, NULL, 0, 0, "Shaders/Light.Shader.vertex.glsl",
                         "Shaders/Light.Shader.fragment.glsl");

  // Modelo plano donde se apoya la torre.
  planeModel = new Model(GL_TRIANGLES, "Texture/cloud9_bot.tga");
  planeModel->SetCamera(camera);
  //planeModel->SetLightAtrr(light);
  planeModel->Initialize(verticesPlaneEnvi, indices, 4, 6,
                         "Shaders/Texture.Shader.vertex.glsl",
                         "Shaders/Texture.Shader.fragment.glsl");

}

/**
 * Loop de la aplicación, solo se sale de esta función si el usuario
 * pulsa Esc o cierra la ventana.
 */
void GLApplication::ApplicationLoop() {

  float giro = 0;

  modelEnvironment->SetCubemapTexture(skyBoxModel->GetCubeTextureMap());
  TowerDubai * towerDubai = new TowerDubai(100, modelSolid, modelEnvironment);
  towerDubai->setHeightLevel(
      towerDubai->getDistanceForTranslate(
          modelSolid->GetMesh()->GetMeshEntry()));
  towerDubai->createLevelsTower();

  //skyBoxModel->SetPosition(camera->GetPosition());

  // Loop que se ejecuta mientras no se pulse Esc o cierre la ventana.
  while (windowManager->ProcessInput(true)) {
    // Se obtiene los FPS.
    TimeManager::Instance().CalculateFrameRate(false);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 parentMatrix(1.0f);

    // Render del domo.
    skyBoxModel->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
    //skyBoxModel->SetPosition(camera->GetPosition());
    skyBoxModel->Render(parentMatrix);

    // Velocidad de giro.
    giro = 0.006500f;

    glm::mat4 towerTransform = glm::mat4(1.0);

    towerTransform = glm::translate(towerTransform, vec3(0.0, -7.5, 0.0));
    towerTransform = glm::scale(towerTransform, vec3(0.3, 0.2, 0.3));
    //towerTransform = glm::scale(towerTransform, vec3(0.5, 0.3, 0.5));
    //towerTransform = glm::scale(towerTransform, vec3(0.1, 0.04, 0.1));
    /*towerTransform = glm::rotate(towerTransform,
     glm::radians(360.0f) * giro, glm::vec3(0.0f, 1.0f, 0));
     towerTransform = glm::rotate(towerTransform, glm::radians(0.0f),
     glm::vec3(0.0f, 1.0f, 0));*/

    // Calcula el siguiente giro de la torre.
    towerDubai->cordinateRotation(giro);
    towerDubai->setTowerTransform(&towerTransform);
    // Render de la torre.
    towerDubai->render();

    // Render del plano donde se apoya la torrre.
    planeModel->SetScale(glm::vec3(100.0f, 100.0f, 100.0f));
    planeModel->Render(towerTransform);

    // Intercambio de los buffers para renderizar.
    windowManager->SwapTheBuffers();
  }
}

/**
 * Método que hace limpieza de los recursos utilizados por la aplicación.
 */
void GLApplication::Destroy() {
  // Se libera memoria de los modelos creados.
  modelEnvironment->Destroy();
  modelSolid->Destroy();
  skyBoxModel->Destroy();
  planeModel->Destroy();

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
