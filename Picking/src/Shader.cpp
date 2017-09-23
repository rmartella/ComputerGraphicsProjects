#include "Headers/Shader.h"
/**
 * Este método carga los archivos de texto para cada shaders.
 * @param strFile Nombre del arhchivo que se va a cargar.
 * @return Cadena que contiene el cdigo del shader.
 */
std::string Shader::LoadShaderFile(std::string strFile) {
  // Abrimos el archivo que contiene el código del shader.
  std::ifstream fin(strFile.c_str());

  // Aseguramos que fue abierto el shader.
  if (!fin)
    return "";

  std::string strLine = "";
  std::string strText = "";

  // Concatenamos las lineas de texto del archivo en una objeto "string".
  while (getline(fin, strLine)) {
    strText = strText + "\n" + strLine;
  }

  // Cerramos el archivo
  fin.close();

  // Regresa los datos del shader.
  return strText;
}

/**
 * Cargan los shaders de vertices y fragmento de un archivo de texto, se manejan rutas relativas.
 * @param strVertexFile Nombre del archivo del shader de vertices.
 * @param strFragmentFile Nombre del archivo del shader de fragmento.
 */
void Shader::Initialize(std::string strVertexFile,
                        std::string strFragmentFile) {

  std::string strVShader, strFShader;

  // Se asegura que se haya pasado los archivos de los shaders.
  if (!strVertexFile.length() || !strFragmentFile.length())
    return;

  // Se asegura que esten limpias las variables de los shaders.
  if (VertexShaderId || FragmentShaderId || ShaderProgramId)
    Destroy();

  // Reinicia el estado de errores de OpenGL.
  GLenum ErrorCheckValue = glGetError();

  // Se crean los identificadores de los shaders.
  VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
  FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

  // Se cargan el codigo de los shaders en un objeto string.
  strVShader = LoadShaderFile(strVertexFile.c_str());
  strFShader = LoadShaderFile(strFragmentFile.c_str());

  // Se obtienen los apuntadores de caracteres del código de los shaders.
  const char *szVShader = strVShader.c_str();
  const char *szFShader = strFShader.c_str();

  // Se asigna el codigo del arhivo a cada apuntador de los shader.
  glShaderSource(VertexShaderId, 1, &szVShader, nullptr);
  glShaderSource(FragmentShaderId, 1, &szFShader, nullptr);

  GLint isCompiled = 0;
  // Se compila el shader de vertices.
  glCompileShader(VertexShaderId);
  glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &isCompiled);
  // Se revisa si fue correcta la compilación.
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(VertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(VertexShaderId, maxLength, &maxLength, &errorLog[0]);
    printf("%s\n", &errorLog[0]);

    glDeleteShader(VertexShaderId);
    return exit(-1);
  }
  // Se revisa si fue correcta la compilación.
  glCompileShader(FragmentShaderId);
  glGetShaderiv(FragmentShaderId, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    GLint maxLength = 0;
    glGetShaderiv(FragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(FragmentShaderId, maxLength, &maxLength, &errorLog[0]);
    printf("%s\n", &errorLog[0]);

    glDeleteShader(FragmentShaderId);
    return exit(-1);
  }

  // Se crea un objeto de programa que representa a los shaders creados.
  ShaderProgramId = glCreateProgram();

  // Agregamos los shaders a un programa.
  glAttachShader(ShaderProgramId, VertexShaderId);
  glAttachShader(ShaderProgramId, FragmentShaderId);

  // Ligamos nuestros shaders al programa.
  glLinkProgram(ShaderProgramId);

  // Revisamos errores.
  ErrorCheckValue = glGetError();

  if (ErrorCheckValue != GL_NO_ERROR) {
    fprintf(stderr,
            "ERROR: Could not create the shader program with error Id: %d\n",
            ErrorCheckValue);
    exit(-1);
  }
}

/**
 * Obtiene ID de una variable uniform del shader.
 * @param strVariable Nombre de la variable del shader.
 * @return Identificador de la variable del shader.
 */
GLint Shader::GetVariable(std::string strVariable) {
  // Se valida que se un programa valido.
  if (!ShaderProgramId)
    return -1;

  // Regresa el ID de la variable solicitada.
  return glGetUniformLocation(ShaderProgramId, strVariable.c_str());
}

/**
 * Libera los datos de memoria que ocupan los shaders.
 */
void Shader::Destroy() {
  if (VertexShaderId) {
    glDetachShader(ShaderProgramId, VertexShaderId);
    glDeleteShader(VertexShaderId);
    VertexShaderId = 0;
  }

  if (FragmentShaderId) {
    glDetachShader(ShaderProgramId, FragmentShaderId);
    glDeleteShader(FragmentShaderId);
    FragmentShaderId = 0;
  }

  if (ShaderProgramId) {
    glDeleteShader(ShaderProgramId);
    ShaderProgramId = 0;
  }
}
