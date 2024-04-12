//
//    Many useful helper functions for GLSL shaders - gleaned from various sources including orange book
//    Created by zwood on 2/21/10.
//    Modified by sueda 10/15/15.
//

#pragma once
#ifndef LAB471_GLSL_H_INCLUDED
#define LAB471_GLSL_H_INCLUDED

#ifndef GL_STACK_OVERFLOW
#define GL_FLOAT 0x1406
#define GL_DOUBLE 0x140A
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_CLEAR 0x1500
#define GL_AND 0x1501
#endif

#include <glad/glad.h>
#include <string>


namespace GLSL
{

	void printOpenGLErrors(char const * const Function, char const * const File, int const Line);
	void checkError(const char *str = 0);
	void printProgramInfoLog(GLuint program);
	void printShaderInfoLog(GLuint shader);
	void checkVersion();
	GLint getAttribLocation(const GLuint program, const char varname[], bool verbose = true);
	GLint getUniformLocation(const GLuint program, const char varname[], bool verbose = true);
	void enableVertexAttribArray(const GLint handle);
	void disableVertexAttribArray(const GLint handle);
	void vertexAttribPointer(const GLint handle, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
}


#ifndef DISABLE_OPENGL_ERROR_CHECKS
#define CHECKED_GL_CALL(x) do { GLSL::printOpenGLErrors("{{BEFORE}} "#x, __FILE__, __LINE__); (x); GLSL::printOpenGLErrors(#x, __FILE__, __LINE__); } while (0)
#else
#define CHECKED_GL_CALL(x) (x)
#endif

#endif // LAB471_GLSL_H_INCLUDED
