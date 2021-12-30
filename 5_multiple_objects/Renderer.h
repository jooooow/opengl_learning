#pragma once

#include <GL/glew.h>

#define CHECK_OPENGL_ERR {\
						    GLenum err;\
						    while ((err = glGetError()) != GL_NO_ERROR)\
						    {\
							    printf("OpenGL Error : 0x%02X\n", err);\
						    }\
						 }