/*
 * FontTypeRendering.h
 *
 *  Created on: 14/05/2015
 *      Author: rey
 */
#include <GL/glew.h>

#include <iostream>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#ifndef FONTTYPERENDERING_H_
#define FONTTYPERENDERING_H_

namespace FontTypeRendering {

/**
 * Clase que se encarga de renderizar los texeles, esta clase utiliza la librer�a
 * FreeType para cargar los TFF
 */
class FontTypeRendering {

  /**
   * Vertex Shader para el renderizado de las fuentes en openGL.
   */
  static const char *VERTEX_SHADER;
  /**
   * Fragment Shader para el renderizado de las fuentes en openGL.
   */
  static const char *FRAGMENT_SHADER;

  /**
   * Escala en x de la fuente
   */
  double SCALEX;
  /**
   * Escala en y de la fuente
   */
  double SCALEY;

 public:
  /**
   * Constructor para el renderizado de texto en openGL.
   * @param ScreenWidth Ancho de ventana.
   * @param ScreenHeight Largo de ventana.
   */
  FontTypeRendering(double ScreenWidth, double ScreenHeight);
  /**
   * Inicializaci�n del renderizado, este m�todo carga el shader, los compila,
   * los enlaza al programa para los shaders, crea los buffers, carga las TTF
   * Y genera la textura para el renderizado.
   */
  void Initialize();
  /**
   * M�todo que renderiza un texto en el espacio de la pantalla.
   * @param str Cadena a renderizar.
   * @param x Coordenada en X.
   * @param y Coordenada en Y.
   */
  void render(const std::string &str, float x, float y);
  /**
   * Destructor de clase.
   */
  virtual ~FontTypeRendering();
 private:
  void render_text(const std::string &str, FT_Face face, float x, float y,
                   float sx, float sy);
 private:
  GLuint texture, sampler;
  GLuint vbo, vao;
  GLuint vs, fs, program;
  GLuint texUniform, colorUniform;
  FT_Library ft_lib { nullptr };
  FT_Face face { nullptr };
};

} /* namespace FontTypeRendering */

#endif /* FONTTYPERENDERING_H_ */
