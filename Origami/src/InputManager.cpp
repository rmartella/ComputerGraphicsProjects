#include "Headers/InputManager.h"
#include "Headers/TimeManager.h"

/**
 * Envia a la c�mara el tipo de entrada para que la c�mara se mueva.
 * @param code Codigos de entrada.
 */
void InputManager::KeyPressed(InputCodes code) {
  // Valida que el aputador a la c�mara haya sido inicializado.
  if (camera == nullptr)
    return;

  // Manejador del c�digo de entrada.
  switch (code) {
    // Se mueve hacia el eje Z
    case Up:
    case W:
    case w:
      camera->MoveCamera(camera->GetSpeed() * TimeManager::Instance().DeltaTime,
                         glm::vec3(0, 0, -1));
      break;
    case Down:
    case S:
    case s:
      camera->MoveCamera(
          -1 * camera->GetSpeed() * TimeManager::Instance().DeltaTime,
          glm::vec3(0, 0, -1));
      break;
      // Se mueve hacia el eje X
    case Left:
    case A:
    case a:
      camera->MoveCamera(-1 * camera->GetSpeed() * TimeManager::Instance().DeltaTime,
                         glm::vec3(-1, 0, 0));
      break;
    case Right:
    case d:
    case D:
      camera->MoveCamera(
          camera->GetSpeed() * TimeManager::Instance().DeltaTime,
          glm::vec3(-1, 0, 0));
      break;
      // Se mueve hacia el eje y
    case Q:
    case q:
      camera->MoveCamera(camera->GetSpeed() * TimeManager::Instance().DeltaTime,
                         glm::vec3(0, -1, 0));
      break;
    case E:
    case e:
      camera->MoveCamera(
          -1 * camera->GetSpeed() * TimeManager::Instance().DeltaTime,
          glm::vec3(0, -1, 0));
      break;
  }
}

/**
 * M�todo que maneja los moviemientos del mouse.
 * @param mouseX Posici�n del mouse en X.
 * @param mouseY Posici�n del mouse en Y
 */
void InputManager::MouseMoved(float mouseX, float mouseY) {
  if (camera == nullptr)
    return;
  camera->SetViewByMouse(mouseX, mouseY);
}
