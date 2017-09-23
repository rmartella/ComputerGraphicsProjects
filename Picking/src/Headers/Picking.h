#include "Camera.h"
#include <vector>
#include "Model.h"
#include <cmath>

#ifndef PICKING_H_
#define PICKING_H_

namespace Picking {

/**
 * Estructura que se utiliza para obtener las coordenadas de la pantalla,
 * para asi encolarlos y el programa principal obtenga la cola de eventos del mouse,
 * y compute el click que se realiz�.
 */
struct Click {
  float x, y;
};

/**
 * Clase que representa los landmarks.
 */
class Mark {
 public:
  /**
   * Constructor de un landmark.
   * @param pickPoints3D Punto 3D del modelo.
   * @param mark Nombre del landmark.
   */
  Mark(vec3 * pickPoints3D, std::string mark) {
    this->pickPoints3D = pickPoints3D;
    this->markStr = mark;
  }
  /**
   * Getter para la cadena del landmark.
   * @return Nombre del landmark.
   */
  const std::string& getMarkStr() const {
    return markStr;
  }

  /**
   * Setter para la cadena del landmark.
   * @param markStr
   */
  void setMarkStr(const std::string& markStr) {
    this->markStr = markStr;
  }

  /**
   * Getter para el punto asociado al landmark.
   * @return
   */
  vec3* getPickPoints3D() {
    return pickPoints3D;
  }

  /**
   * Setter para el punto del landmark.
   * @param pickPoints3D
   */
  void setPickPoints3D(vec3*pickPoints3D) {
    this->pickPoints3D = pickPoints3D;
  }
 private:
  vec3 * pickPoints3D;
  std::string markStr;
};

/**
 * Clase que manaje los eventos de picking.
 */
class Picking {
 public:
  /**
   * Constructor default.
   */
  Picking();
  /**
   * Setter para la c�mara, se utiliza al momento de obtener el rayo,
   * y que este este en coordenadas del modelo.
   * @param camera
   */
  void SetCamera(Camera * camera) {
    this->camera = camera;
  }
  /**
   * Getter para el arreglo de clicks
   * @return
   */
  std::vector<Click*> * GetClicks() {
    return clicks;
  }
  /**
   * M�todo que obtiene las corrdenadas del origen y destino del rayo,
   * cooresponden a las coordenadas del rayo en el plano de recorte,
   * plano cercano y plano lejano, las coordenadas ya estan en espaci� del modelo,
   * al realizar las transformaci�nes inversas de proyeci�n al modelo.
   * @param modelMatrix Matrix del modelo.
   * @param x Coordenada en x, en el espacio de la pantalla.
   * @param y Coordenada en y, en el espacio de la pantalla.
   * @param ScreenWidth Ancho de ventana.
   * @param ScreenHeight Altura de ventana.
   * @param v1 Apuntador a la coordenada del origen del rayo (Near Plane).
   * @param v2 Apuntador a la coordenada del destino del rayo (Far Plane).
   */
  void Get3DRayUnderMouse(glm::mat4 modelMatrix, float x, float y,
                          int ScreenWidth, int ScreenHeight, glm::vec3* v1,
                          glm::vec3* v2);
  /**
   *M�todo que obtiene la intersecci�n del rayo con un tri�ngulo,
   *Se utiliza el algortimo de Tomas Moller.
   *  Representaci�n del rayo en coordenadas parametricas.
   *    R ( t ) = O + tD  ------- (1)
   *  Un punto de un tri�ngulo esta dado por las coordenadas varicentricas,
   *  del triangulo.
   *    T ( u v ) = (1 ; u ; v ) V 0 + uV 1 + vV 2  ------- (2)
   * @param orig Origen del rayo.
   * @param dir Direcci�n del rayo.
   * @param vert0 Vertice 1 del triangulo a probar.
   * @param vert1 Vertice 2 del triangulo a probar.
   * @param vert2 Vertice 3 del triangulo a probar.
   * @param t Valor de ecuaci�n de (1)
   * @param u Coordenada u varicentrica del tri�ngulo (2).
   * @param v Coordenada v varicentrica del tri�ngulo (2).
   * @return Bandera que indica si hay intersecci�n o no, true existe la intersecci�n
   * false no existe la intersecci�n.
   */
  bool RayTriangleIntersect(glm::vec3 orig, glm::vec3 dir, glm::vec3 vert0,
                            glm::vec3 vert1, glm::vec3 vert2, double *t,
                            double *u, double *v);

  /**
   * M�todo que prueba si una linea interseca con una esfera.
   * @param orig Origen del rayo.
   * @param dest Punto final del rayo.
   * @param dir Direcci�n del rayo.
   * @param vSphereCenter Centro de la esfera.
   * @param sphereRadius Radio de la esfera.
   * @param t Indica la distancia que existe entre el orign de la esfera
   * y el punto de intersecci�n.
   * @return Bandera que indica si existe intersecci�n de rayo con la esfera,
   * true existe la intersecci�n false no existe la intersecci�n.
   */
  bool RaySphereIntersect(glm::vec3 * orig, glm::vec3 * dest, glm::vec3 * dir,
                          glm::vec3 * vSphereCenter, float sphereRadius,
                          float * t);
  /**
   * M�todo que valida la superposici�n de marcas y evitar tener muchas marcas,
   * en un miso sitio, la condici�n de una marca es que el radio de tolerancia
   * de la marca no sobrepase dos veces el radio.
   * @param marks Lista actual de marcas agregadas.
   * @param sphereRadius  Radio de la esfera.
   * @param mark Marca que se desea validar.
   * @param markHit Bandera que indica que es una marca ya existente.
   * @param idLandMark Identificador de la marca con la que pego.
   * @return Bandera que indica si es valida o no la marca.
   */
  bool ValidateMark(std::vector<Mark*> marks, float sphereRadius, Mark * mark,
                    Mark * markHit, int * idLandMark);
  /**
   * Destructor de clase.
   */
  virtual ~Picking();
 private:
  Camera * camera;
  std::vector<Click*> * clicks = new std::vector<Click*>();
};

} /* namespace Picking */
#endif /* PICKING_H_ */
