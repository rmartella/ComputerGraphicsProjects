#ifndef _Shader_H
#define _Shader_H

#include <string>
#include <fstream>
#include <GL/glew.h>
#include <vector>
/**
 * Clase dedicada a la creación, inicialización, obtención de variables uniform, enviar información,
 * prender y apagar shaders.
 */
class Shader {
 public:

  Shader() {
  }
  ~Shader() {
    Destroy();
  }

  /**
   * Este método carga los archivos de texto para cada shader.
   * @param strFile Nombre del arhchivo que se va a cargar.
   * @return Cadena que contiene el cdigo del shader.
   */
  std::string LoadShaderFile(std::string strFile);

  /**
   * Cargan los shaders de vertices y fragmento de un archivo de texto, se manejan rutas relativas.
   * @param strVertexFile Nombre del archivo del shader de vertices.
   * @param strFragmentFile Nombre del archivo del shader de fragmento.
   */
  void Initialize(std::string strVertexFile, std::string strFragmentFile);

  /**
   * Obtiene ID de una variable uniform del shader.
   * @param strVariable Nombre de la variable del shader.
   * @return Identificador de la variable del shader.
   */
  GLint GetVariable(std::string strVariable);

  /**
   * Permite pasar un una matriz de 4x4 de flotantes al shader.
   * @param id Identificador de la variable uniform.
   * @param count Especifica el número de matrices que se van a actualizar.
   * @param transpose Especifica si se desea enviar la matriz transpuesta
   * @param value Apuntador al arreglo.
   */
  void SetMatrix4(GLint id, GLsizei count, GLboolean transpose,
                  const GLfloat *value) {
    glUniformMatrix4fv(id, count, transpose, value);
  }

  /**
   * Enciende el Shader
   */
  void TurnOn() {
    glUseProgram(ShaderProgramId);
  }
  /**
   * Apaga el Shader
   */
  void TurnOff() {
    glUseProgram(0);
  }

  /**
   * Libera los datos de memoria que ocupan los shaders.
   */
  void Destroy();

 private:

  GLuint VertexShaderId;
  GLuint FragmentShaderId;
  GLuint ShaderProgramId;
};

#endif
