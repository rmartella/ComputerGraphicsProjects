/*
 * Mesh.cpp
 *
 *  Created on: 18/04/2015
 *      Author: rey
 */

#include "Headers/Mesh.h"
#include <assert.h>

using namespace glm;

namespace Mesh {

/**
 * Constructor por defualt.
 */
Mesh::MeshEntry::MeshEntry() {
  VB = INVALID_OGL_VALUE;
  IB = INVALID_OGL_VALUE;
  NumIndices = 0;
  MaterialIndex = INVALID_MATERIAL;
}
;

/**
 * Destructor por defualt.
 */
Mesh::MeshEntry::~MeshEntry() {
  if (VB != INVALID_OGL_VALUE) {
    glDeleteBuffers(1, &VB);
  }

  if (IB != INVALID_OGL_VALUE) {
    glDeleteBuffers(1, &IB);
  }
}

/**
 * Método que inicializa una MeshEntry.
 * @param Vertices Apuntador a los vertices.
 * @param Indices Apuntador a los indices.
 */
void Mesh::MeshEntry::Init(const std::vector<Vertex>& Vertices,
                           const std::vector<unsigned int>& Indices) {
  NumIndices = Indices.size();

  // Crea el buffer de vertices.
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0],
  GL_STATIC_DRAW);
  // Crea el buffer de indices.
  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices,
               &Indices[0], GL_STATIC_DRAW);
}

/**
 * Constructor carga una archivo para poder cargarl la malla.
 * @param Filename Nombre del archivo que se va utilizar para la carga.
 */
Mesh::Mesh(std::string Filename) {
  this->Filename = Filename;
}

/**
 * Destructor de la malla.
 */
Mesh::~Mesh() {
}

/**
 * Método que elimina los objetos de textura.
 */
void Mesh::Clear() {
  for (unsigned int i = 0; i < m_Textures.size(); i++) {
    SAFE_DELETE(m_Textures[i]);
  }
}

/**
 * Método que carga una malla.
 * @return
 */
bool Mesh::LoadMesh() {
  // Se libera las cargas previas de datos de la malla si existen.
  Clear();

  bool Ret = false;
  // Objeto importador de mallas.
  Assimp::Importer Importer;

  // Se lee el archivo que contiene el modelo.
  // Se le indican las banderas:
  //                            aiProcess_Triangulate : Se triangule la malla si las primitivas no son triangulos.
  //                            aiProcess_GenSmoothNormals: Se suavicen las normales.
  //                            aiProcess_FlipUVs : Que guire las coordenadas de mapeo si es necesario.

  const aiScene* pScene = Importer.ReadFile(
      Filename.c_str(),
      aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

  if (pScene) {
    // Se inicializa la escena.
    Ret = InitFromScene(pScene);
  } else {
    printf("Error parsing '%s': '%s'\n", Filename.c_str(),
           Importer.GetErrorString());
  }

  return Ret;
}

/**
 * Inicialización de una escena de la aplicación.
 * @param pScene Escena de Assimpl.
 * @return Si fue valida la inicialización.
 */
bool Mesh::InitFromScene(const aiScene* pScene) {
  // Redimiensionamiento de los arreglos que contiene la información de los vertices y materiales.
  m_Entries.resize(pScene->mNumMeshes);
  m_Textures.resize(pScene->mNumMaterials);

  // Se inicializan las mallas una por una.
  for (unsigned int i = 0; i < m_Entries.size(); i++) {
    const aiMesh* paiMesh = pScene->mMeshes[i];
    InitMesh(i, paiMesh);
  }

  return InitMaterials(pScene);
}

/**
 * Inicialización de la malla.
 * @param Index Id de la MeshEntry.
 * @param paiMesh Malla de Assimp.
 */
void Mesh::InitMesh(unsigned int Index, const aiMesh* paiMesh) {
  // Inicialización del número de materiales
  m_Entries[Index].MaterialIndex = paiMesh->mMaterialIndex;
  // Se crean el arreglo de indices y vertices para que puedan ser obtenidas por clases que requieran
  // iterar y modificar algún atributo de los vertices, hay clases que basta con cargar a los buffers
  // los datos y no obtener estos datos se hace generica.
  m_Entries[Index].Indices = (unsigned int *) malloc(sizeof(unsigned int));
  m_Entries[Index].Vertices = (Vertex *) malloc(sizeof(Vertex *));

  std::vector<Vertex> Vertices;
  std::vector<unsigned int> Indices;

  const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

  // Se redimensiona el arreglo de vertices.
  m_Entries[Index].Vertices = (Vertex *) realloc(
      m_Entries[Index].Vertices, sizeof(Vertex) * paiMesh->mNumVertices);
  // Se itera sobre la cantidad de vertices.
  for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
    // Se obtienen los apuntadores a la información de los vertices.
    const aiVector3D* pPos = &(paiMesh->mVertices[i]);
    const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
    const aiVector3D* pTexCoord =
        paiMesh->HasTextureCoords(0) ?
            &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

    // Se insertan los datos al vector de datos que puede ser obtenido por otras clases
    // para manipular información.
    Vertex v(vec3(pPos->x, pPos->y, pPos->z), vec2(pTexCoord->x, pTexCoord->y),
             vec3(pNormal->x, pNormal->y, pNormal->z));
    Vertices.push_back(v);
    // Se desreferencia el contenido del apuntador a los vetices que se cargaran en el buffer
    *(m_Entries[Index].Vertices + i) = v;
  }

  // Se redimensiona el arreglo de indices.
  m_Entries[Index].Indices = (unsigned int *) realloc(
      m_Entries[Index].Indices,
      sizeof(unsigned int) * (paiMesh->mNumFaces * 3));
  // Iteracion por número de caras de la malla para crear el arreglo de indices.
  for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
    // Apuntador a la cara.
    const aiFace& Face = paiMesh->mFaces[i];
    // Valida que el número de indices por cara sea de 3
    assert(Face.mNumIndices == 3);
    // Se insertan los datos al vector de indices que puede ser obtenido por otras clases para
    // manipular información.
    Indices.push_back(Face.mIndices[0]);
    Indices.push_back(Face.mIndices[1]);
    Indices.push_back(Face.mIndices[2]);
    // Se desreferencia el contenido del apuntador a los indices que se cargaran en el buffer
    *(m_Entries[Index].Indices + i * 3) = Face.mIndices[0];
    *(m_Entries[Index].Indices + i * 3 + 1) = Face.mIndices[1];
    *(m_Entries[Index].Indices + i * 3 + 2) = Face.mIndices[2];
  }

  m_Entries[Index].Init(Vertices, Indices);
}

/**
 * Inicialización de los materiales.
 * @param pScene Escena de Assimp
 * @return Bandera la correcta carga de materiales.
 */
bool Mesh::InitMaterials(const aiScene* pScene) {
  // Extrae el directorio del archivo a cargar.
  std::string::size_type SlashIndex = Filename.find_last_of("/");
  std::string Dir;

  if (SlashIndex == std::string::npos) {
    Dir = ".";
  } else if (SlashIndex == 0) {
    Dir = "/";
  } else {
    Dir = "Texture";
  }

  bool Ret = true;

  // Inicialización de los materiales.
  for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
    const aiMaterial* pMaterial = pScene->mMaterials[i];

    m_Textures[i] = NULL;

    // Se obtiene el número de texturas difusas definidas en el material
    if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
      aiString Path;

      // Se obtiene el Path de la textura
      if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL,
      NULL,
                                NULL, NULL, NULL) == AI_SUCCESS) {
        std::string FullPath = Dir + "/" + Path.data;
        // Se crea el objeto textura.
        m_Textures[i] = new Texture::Texture(GL_TEXTURE_2D, FullPath.c_str());

        // Carga la textura
        if (!m_Textures[i]->Load()) {
          printf("Error cargando la texture '%s'\n", FullPath.c_str());
          delete m_Textures[i];
          m_Textures[i] = NULL;
          Ret = false;
        } else {
          //printf("Textura cargada '%s'\n", FullPath.c_str());
        }
      }
    }

    // Carga la textura defual en caso de que el modelo no tenga textura para esa MeshEntry
    if (!m_Textures[i]) {
      m_Textures[i] = new Texture::Texture(GL_TEXTURE_2D, "Texture/white.png");

      Ret = m_Textures[i]->Load();
    }
  }

  return Ret;
}

/**
 * Método que se encarga de renderizar un malla.
 */
void Mesh::Render() {
  // Se habilitan tres VAO'S (Posición, UV, Normales)
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  // Se itera con el número de sub-objetos que conforman la malla.
  for (unsigned int i = 0; i < m_Entries.size(); i++) {
    // Se enlaza el buffer vertices.
    glBindBuffer(GL_ARRAY_BUFFER, m_Entries[i].VB);
    // Se indica como estan estructurados los datos de los vertices.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (const GLvoid*) 12);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (const GLvoid*) 20);

    // Se enlaza el buffer de indices.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Entries[i].IB);

    // Se obtiene el número de materiales.
    const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

    // Hace el binding de la unida de la textura.
    if (MaterialIndex < m_Textures.size() && m_Textures[MaterialIndex]) {
      m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
    }

    // Dibuja la malla basa en triangulos.
    glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
  }

  // Se deshabilitan los VAO's (Posición, UV, Normales).
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(2);
  glDisableVertexAttribArray(3);
}

} /* namespace Mesh */
