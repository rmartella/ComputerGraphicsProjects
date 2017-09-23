/*
 * Mesh.h
 *
 *  Created on: 18/04/2015
 *      Author: rey
 */
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include "Texture.h"
#include "Util.h"

#ifndef MESH_H_
#define MESH_H_

namespace Mesh {

/**
 * Estructura que va almacenar el parseo de vertices a un formato intermedio.
 */
struct Vertex {
  glm::vec3 m_pos;
  glm::vec2 m_tex;
  glm::vec3 m_normal;

  /**
   * Constructor por default.
   */
  Vertex() {
  }

  /**
   * Constructor de un vertice.
   * @param pos Posición del vertice.
   * @param tex Coórdenadas de textura del vertice.
   * @param normal Normal del vertice.
   */
  Vertex(const glm::vec3& pos, const glm::vec2& tex, const glm::vec3& normal) {
    m_pos = pos;
    m_tex = tex;
    m_normal = normal;
  }
};

/**
 *Clase que representa el parseo de los objetos que obtiene la librería Assimpl a una objeto
 *conocido para la aplicación. Un objeto Mesh puede estar compuestos de entidades, es decir una
 *malla puede tener varias entidades de malla, esto sirve para poder separar materiales o tener
 *entidades de mallas y poder renderizar por separado..
 */
class Mesh {
 public:
  /**
   * Estructura que representa una entidad de la malla, varias MeshEntry forman el objeto a renderizar.
   */
  struct MeshEntry {
    /**
     * Constructor por defualt.
     */
    MeshEntry();
    /**
     * Destructor por defualt.
     */
    ~MeshEntry();

    /**
     * Método que inicializa una MeshEntry.
     * @param Vertices Apuntador a los vertices.
     * @param Indices Apuntador a los indices.
     */
    void Init(const std::vector<Vertex>& Vertices,
              const std::vector<unsigned int>& Indices);

    /**
     * Identificador del buffer de vertices.
     */
    GLuint VB;
    /**
     * Identificador del buffer de indices.
     */
    GLuint IB;
    /**
     * Apuntador a los vertices del objeto que conforma el Modelo.
     */
    Vertex * Vertices;
    /**
     * Apuntador a los indices del objeto que conforma el Modelo.
     */
    unsigned int * Indices;
    /**
     * Numero de indices.
     */
    unsigned int NumIndices;
    /**
     * Indice de materiales.
     */
    unsigned int MaterialIndex;
  };
  /**
   * Constructor carga una archivo para poder cargarl la malla.
   * @param Filename Nombre del archivo que se va utilizar para la carga.
   */
  Mesh(std::string Filename);
  std::vector<MeshEntry> * GetMeshEntry() {
    return &m_Entries;
  }
  /**
   * Destructor de la malla.
   */
  ~Mesh();
  /**
   * Método que carga una malla.
   * @return
   */
  bool LoadMesh();
  /**
   * Método que se encarga de renderizar un malla.
   */
  void Render();
  /**
   * Vector que contiene las MeshEntry (Partes de la malla).
   */
  std::vector<MeshEntry> m_Entries;
  /**
   * Vector que contiene las Tetxruas para cada MeshEntry (Partes de la malla).
   */
  std::vector<Texture::Texture*> m_Textures;
  /**
   * Nombre del archivo que se va a cargar.
   */
  std::string Filename;

 private:
  bool InitFromScene(const aiScene* pScene);
  void InitMesh(unsigned int Index, const aiMesh* paiMesh);
  bool InitMaterials(const aiScene* pScene);
  void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

};

} /* namespace Mesh */

#endif /* MESH_H_ */
