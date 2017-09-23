/*
 * Texture.h
 *
 *  Created on: 10/04/2015
 *      Author: rey
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <GL/glew.h>
//#include <ImageMagick-6/Magick++.h>
#include <FreeImage.h>

namespace Texture {
/**
 * Clase que se utiliza para crear texturas, crear los binds de las texturas e inicializar
 * los buffers de textura. La clase hace uso de la libreria de FreeImage para cargar los
 * formatos de textura.
 */
class Texture {
 public:
  /**
   * Constructor de parametros de la textura.
   * @param TextureTarget Tipo de textura que será objetivo para este objeto
   * @param FileName Ruta del archivo donde se encuentran las texturas
   */
  Texture(GLenum TextureTarget, const std::string& FileName);
  /**
   * Método que carga la textura
   * @return Bandera que indica que la texura fue cargada correctamente
   */
  bool Load();
  /**
   * Este método hace el enalce del objeto de textura con su unidad de textura.
   * De acuerdo al diseño de la arquitectura puede haber diferentes objetos de textura
   * para un módelo, si se desea enlazar una unidad de textura a un objeto de textura digamos A
   * lo pirmero que se hace es activar la unidad de textura 0.
   * @param TextureUnit
   */
  void Bind(GLenum TextureUnit);
  /**
   * Destructor de la clase textura
   */
  virtual ~Texture();
 private:
  std::string m_fileName;
  GLenum m_textureTarget;
  GLuint m_textureObj;
  FIBITMAP * m_image;
  /*Magick::Image m_image;
   Magick::Blob m_blob;*/
};

} /* namespace Texture */
#endif /* TEXTURE_H_ */
