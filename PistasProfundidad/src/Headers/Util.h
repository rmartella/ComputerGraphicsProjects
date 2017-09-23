/*
 * Util.h
 *
 *  Created on: 17/04/2015
 *      Author: rey
 */

#ifndef UTIL_H_
#define UTIL_H_

/**
 * Macro que obtiene el tamaño de un arreglo.
 */
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
/**
 * Constane que indica valores invalidos para OpenGL.
 */
#define INVALID_OGL_VALUE 0xffffffff

/**
 * Macro que libera la memoria eliminando la instancia p.
 */
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }

#endif /* UTIL_H_ */
