#pragma once

#ifdef __cplusplus
extern "C" {
#endif

//#include <glad/glad.h>

// Forward declarations for Khronos types. Don't really want to include the whole-ass glad.h header..
typedef unsigned int GLenum;
typedef int GLint;

GLuint size_from_gl_type(const GLenum type);
void upload_from_gl_type(const GLint location, const GLenum type, const GLint elements, const void* data);

#ifdef __cplusplus
}
#endif
