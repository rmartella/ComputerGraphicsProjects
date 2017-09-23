#include <GL/glew.h>
#include "Headers/Main.h"

/**
 * Constantes para la iluminación.
 */
const glm::vec4 white(1);
const glm::vec4 black(0);
const glm::vec4 ambient(0.9f, 0.9f, 0.9f, 1.0f);

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
      || windowManager->Initialize(ScreenWidth, ScreenHeight, "Picking", false)
          != 0) {
    exit(-1);
  }

  // Viewport de las mismas dimensiones de la ventana.
  glViewport(0, 0, ScreenWidth, ScreenHeight);

  // Se habilita las banderas de OpenGL para la prueba de profundiad y
  // recorte de caras ocultas.
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // Se indica el número de luces que se desean manejar.
  int numLights = 1;
  // Inicialización del arreglo de luces.
  LightAtrr * light = new LightAtrr[numLights];

  // Inicialización de valores de luces.
  light[0].ambient = ambient;
  light[0].lightColor = white;
  light[0].lightPosW = glm::vec4(0, 0, -10, 0);
  light[0].materialDiffuse = white;
  light[0].materialEmissive = black;
  light[0].materialShininess = 15.0f;
  light[0].materialSpecular = white;

  // Se crea el modelo para los landmarks.
  sphere = new Model(GL_TRIANGLES, "Texture/white.png");
  // Se crea el modelo de la cabeza humana.
  humanHead = new Model(GL_TRIANGLES, "");

  // Se settean número de luces y atributos de la iluminación.
  humanHead->setNumLights(numLights);
  humanHead->SetLightAtrr(light);

  // Se crea la matriz de proyección de la cámara.
  // FOV  Aspect Ratio  Near / Far Planes
  camera->SetPerspective(glm::radians(60.0f),
                         ScreenWidth / (float) ScreenHeight, 0.0001f, 150.0f);

  camera->PositionCamera(0, 0, 1.2, 0, 0);

  humanHead->SetCamera(camera);

  // Se instancia un objeto malla que contiene los vertices de esta.
  Mesh::Mesh * m_pMesh = new Mesh::Mesh("Models/TheRock2.2.obj");
  // Se settea la malla al modelo.
  humanHead->SetMesh(m_pMesh);

  // Inicialización de VAOs y VBOs
  humanHead->Initialize(NULL, NULL, 0, 0, "Shaders/Light.Shader.vertex.glsl",
                        "Shaders/Light.Shader.fragment.glsl");

  // Se crea proceduralmente la malla de la esfera.
  int slices = 10, stacks = 10;
  Vertex3 * vertices = (Vertex3 *) malloc(
      sizeof(Vertex3) * (slices + 1) * (stacks + 1));
  GLuint * indices = (GLuint *) malloc(
      sizeof(GLuint) * (slices * stacks + slices) * 6);
  SolidSphere(ratioSphere, slices, stacks, vertices, indices);
  // Setten los valores de iluminación, cámara e inicializa el modelo.
  sphere->SetLightAtrr(light);
  sphere->setNumLights(numLights);
  sphere->SetCamera(camera);
  sphere->Initialize(vertices, indices, (slices + 1) * (stacks + 1),
                     (slices * stacks + slices) * 6,
                     "Shaders/Light.Shader.vertex.glsl",
                     "Shaders/Light.Shader.fragment.glsl");

  // Se inicializa el modelo de texeles.
  modelText->Initialize();

}

/**
 * Loop de la aplicación, solo se sale de esta función si el usuario
 * pulsa Esc o cierra la ventana.
 */
void GLApplication::ApplicationLoop() {
  // Loop que se ejecuta mientras no se pulse Esc o cierre la ventana.
  while (windowManager->ProcessInput(true)) {
    // Se obtiene los FPS.
    double FPS = TimeManager::Instance().CalculateFrameRate(false);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Se crea una matriz para el modelo.
    glm::mat4 rotationEsceneMatrix(1.0f);
    rotationEsceneMatrix = glm::toMat4(gRotation);

    glm::mat4 modelMatrix = glm::translate(glm::mat4(), vec3(0, 0, 0));
    modelMatrix = scale(modelMatrix, vec3(0.15, 0.15, 0.15));

    modelMatrix = modelMatrix * rotationEsceneMatrix;

    // Iterador para lo eventos que se han capturado.
    for (unsigned int i = 0; i < picking->GetClicks()->size(); i++) {
      glm::vec3 * v1 = new glm::vec3(), *v2 = new glm::vec3();
      // Se obtiene el primer evento del mouse.
      Picking::Click * clickTop = (*picking->GetClicks())[0];
      picking->GetClicks()->erase(picking->GetClicks()->begin());
      int x = clickTop->x;
      int y = clickTop->y;
      // Se obtiene el rayo que fue capturado por el mouse.
      picking->Get3DRayUnderMouse(modelMatrix, x, y, ScreenWidth, ScreenHeight,
                                  v1, v2);
      glm::vec4 v1p = glm::vec4(*v1, 1.0f);
      glm::vec4 v2p = glm::vec4(*v2, 1.0f);
      glm::vec3 direction = glm::normalize(
          glm::vec3(v2p.x, v2p.y, v2p.z) - glm::vec3(v1p.x, v1p.y, v1p.z));

      // Se obtiene la mayya del modelo.
      Mesh::Mesh * mesh = humanHead->GetMesh();
      std::vector<Mesh::Mesh::MeshEntry> * meshEntryesPtr =
          mesh->GetMeshEntry();
      vec3 * intersection = new vec3(0.0f);
      float minD = 0;
      // Iterador de los MeshEntrys que conforman la maya.
      for (unsigned int j = 0; j < meshEntryesPtr->size(); j++) {
        // Indices y vertices del MeshEntry
        unsigned int * Indices = (*meshEntryesPtr)[j].Indices;
        Mesh::Vertex * Vertices = (*meshEntryesPtr)[j].Vertices;
        // Se itera sobre las caras de los vertices.
        for (unsigned int k = 0; k < (*meshEntryesPtr)[j].NumIndices; k += 3) {
          double t, u, v;
          glm::vec4 pos1 = glm::vec4(Vertices[Indices[k]].m_pos, 1.0);
          glm::vec4 pos2 = glm::vec4(Vertices[Indices[k + 1]].m_pos, 1.0);
          glm::vec4 pos3 = glm::vec4(Vertices[Indices[k + 2]].m_pos, 1.0);

          // Se obtiene la intersección del rayo con el triangulo.
          bool isPicking = picking->RayTriangleIntersect(
              glm::vec3(v1p.x, v1p.y, v1p.z), direction,
              glm::vec3(pos1.x, pos1.y, pos1.z),
              glm::vec3(pos2.x, pos2.y, pos2.z),
              glm::vec3(pos3.x, pos3.y, pos3.z), &t, &u, &v);
          // Si existe intersección.
          if (isPicking == true) {
            // Se determina la intersección que este mas cerca del punto de origen del rayo.
            if (minD == 0 || (minD > 0 && t < minD)) {
              minD = t;
              float factor = 1 - u - v;
              glm::vec3 minInt = glm::vec3(factor * pos1.x, factor * pos1.y,
                                           factor * pos1.z)
                  + glm::vec3(u * pos2.x, u * pos2.y, u * pos2.z)
                  + glm::vec3(v * pos3.x, v * pos3.y, v * pos3.z);
              intersection->x = minInt.x;
              intersection->y = minInt.y;
              intersection->z = minInt.z;

            }
          }
        }
      }
      // Intersección en coordenadas de la proyección.
      glm::vec3 * inter_MVP = new glm::vec3(
          camera->GetProjectionMatrix() * camera->GetViewMatrix() * modelMatrix
              * glm::vec4(*intersection, 1.0f));
      // Identificador del landmark.
      std::ostringstream convert;
      convert << "LandMark " << (marks.size() + 1) << ": P(" << inter_MVP->x
              << "," << inter_MVP->y << "," << inter_MVP->z << ")";
      std::string idStr = convert.str();
      Picking::Mark * newMark = new Picking::Mark(new glm::vec3(*intersection),
                                                  idStr);
      Picking::Mark * markHit = new Picking::Mark(new glm::vec3(*intersection),
                                                  idStr);
      int idHit;
      // Valida la marca, si se se translapan las eferas deterina
      // Con que esfera esta translapando.
      bool validMark = picking->ValidateMark(marks, sizeLandMark * ratioSphere,
                                             newMark, markHit, &idHit);
      // Si no esta habilitada el calculo de la distancia,  y no se superpone
      // se agrega una nueva marca.
      if (validMark && !distanceEnable && !deleteEnable) {
        marks.push_back(newMark);
        Vertex3 rayVertices[] = { { { v1->x, v1->y, v1->z }, { 0.0f, 0.0f } }, {
            { v2->x, v2->y, v2->z }, { 0.0f, 1.0f } } };
        GLuint indexRay[] = { 0, 1 };
        Model * modelo = new Model(GL_LINES, "Texture/white.png");
        modelo->SetCamera(camera);
        modelo->Initialize(rayVertices, indexRay, 2, 2,
                           "Shaders/Texture.Shader.vertex.glsl",
                           "Shaders/Texture.Shader.fragment.glsl");
        rays->push_back(modelo);
      } else if (!validMark && distanceEnable && !deleteEnable
          && numMarks < 2) {  // Caso contrario es marca para obtener distancia.
        free(newMark);
        marksToDistance->push_back(markHit);
        numMarks++;
      } else if (!validMark && !distanceEnable && deleteEnable
          && numMarks == 0) {  // La marca se va a eliminar.
        marks.erase(marks.begin() + idHit);
        rays[0][idHit]->Destroy();
        rays->erase(rays->begin() + idHit);
        free(newMark);
      }
      free(intersection);
    }

    // Render de la cabeza humana.
    humanHead->Render(modelMatrix);

    // Render de trazado de rayos
    if (enableRayCastingGL) {
      for (unsigned int i = 0; i < rays->size(); i++) {
        rays[0][i]->Render(modelMatrix);
      }
    }

    Picking::Mark * minMark = nullptr;
    bool hitSphere = false;
    float tmin;
    float xText = -0.97;
    float offsetText = 0.05;
    float yInitPos = 0.95;
    // Iteardor para las marcas ya manejadas por la aplicación.
    for (unsigned int i = 0; i < marks.size(); i++) {
      glm::vec3 pickPoints3D = *(marks[i]->getPickPoints3D());
      // Condición que no permite obtener información mientras
      // Se haya calculado la distancia entre dos de ellas.
      if (!distanceEnable || (distanceEnable && numMarks < 2)) {
        if (this->mousePos->x >= 0 && this->mousePos->y >= 0) {
          float t = 0;
          glm::vec3 * v1 = new glm::vec3(), *v2 = new glm::vec3();
          // Obtiene rayo de la posición actual del mouse.
          picking->Get3DRayUnderMouse(modelMatrix, this->mousePos->x,
                                      this->ScreenHeight - this->mousePos->y,
                                      ScreenWidth, ScreenHeight, v1, v2);
          glm::vec3 direction = glm::normalize(*v2 - *v1);
          bool pickSphere;
          // Determina la intersección con el landmark.
          pickSphere = picking->RaySphereIntersect(v1, v2, &direction,
                                                   &pickPoints3D,
                                                   sizeLandMark * ratioSphere,
                                                   &t);
          // Calcula el landmark mas cercano al observador.
          if (pickSphere) {
            hitSphere = true;
            if (minMark == nullptr) {
              tmin = t;
              minMark = marks[i];
            } else if (t < tmin) {
              tmin = t;
              minMark = marks[i];
            }
          }
        }
        // Render de información del landmark cuando no esta habilitado
        // el calculo de la distancia.
        if (hitSphere)
          modelText->render(minMark->getMarkStr(), xText,
                            yInitPos - numMarks * offsetText);
      }
      // Render de los landmarks seleccionados.
      for (unsigned int markId = 0; markId < marksToDistance->size();
          markId++) {
        modelText->render(marksToDistance[0][markId]->getMarkStr(), xText,
                          yInitPos - markId * offsetText);
      }
      // Renderiza la marca.
      sphere->SetScale(vec3(sizeLandMark, sizeLandMark, sizeLandMark));
      sphere->SetPosition(pickPoints3D);
      sphere->Render(modelMatrix);
    }

    // Render de texto de los frames por segundo tamaño de landmark.
    std::ostringstream * convert = new std::ostringstream();
    *convert << "FPS : " << FPS;
    std::string str = convert->str();
    modelText->render(str, xText, 0.75);
    free(convert);
    convert = new std::ostringstream();
    *convert << sizeLandMark;
    str = "Size landMark : " + convert->str();
    modelText->render(str, xText, 0.8);
    // Renderizado de la distancia
    if (numMarks == 2) {
      float distance = glm::distance(*marksToDistance[0][0]->getPickPoints3D(),
                                     *marksToDistance[0][1]->getPickPoints3D());
      convert = new std::ostringstream();
      *convert << "Distancia : " << distance;
      modelText->render(convert->str(), xText, 0.85);
    }

    // Intercambio de los buffers para renderizar.
    windowManager->SwapTheBuffers();
  }
}

/**
 * Genera los vetices de una esfera.
 * @param radius Radio de la esfera.
 * @param slices Revanadas horizontales.
 * @param stacks Revanadas Verticales.
 * @param vertices Apuntador donde se almacenan por referencia los vertices.
 * @param indices Apuntador donde se almacenan por referencia los indices.
 */
void GLApplication::SolidSphere(float radius, int slices, int stacks,
                                Vertex3 * vertices, GLuint * indices) {
  // Uso local de name spaces.
  using namespace glm;
  using namespace std;

  const float pi = 3.1415926535897932384626433832795f;
  const float _2pi = 2.0f * pi;

  vector<vec3> positions;
  vector<vec3> normals;
  vector<vec2> textureCoords;

  for (int i = 0; i <= stacks; ++i) {
    // Coordenadas V de la textura.
    float V = i / (float) stacks;
    float phi = V * pi;

    for (int j = 0; j <= slices; ++j) {
      // Coordenadas U de la textura.
      float U = j / (float) slices;
      float theta = U * _2pi;

      float X = cos(theta) * sin(phi);
      float Y = cos(phi);
      float Z = sin(theta) * sin(phi);
      // Se calculan los vertices, UV, normales de la esfera.
      *(vertices + i * (slices + 1) + j) = {vec3(X, Y, Z) * radius, vec2(U, V), vec3(X, Y, Z)};
    }
  }

  // Generaicón de indices.
  for (int i = 0; i < slices * stacks + slices; ++i) {
    *(indices + i * 6) = i;
    *(indices + i * 6 + 1) = i + slices + 1;
    *(indices + i * 6 + 2) = i + slices;
    *(indices + i * 6 + 3) = i + slices + 1;
    *(indices + i * 6 + 4) = i;
    *(indices + i * 6 + 5) = i + 1;
  }
}

/**
 * Método que hace limpieza de los recursos utilizados por la aplicación.
 */
void GLApplication::Destroy() {
  // Se libera memoria de los modelos creados.
  humanHead->Destroy();
  sphere->Destroy();
  for (unsigned int i = 0; i < rays->size(); i++) {
    rays[0][i]->Destroy();
  }

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
