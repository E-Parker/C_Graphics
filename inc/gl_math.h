#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <glad/glad.h>

#ifndef GL_MATH_INCLUDED
#define GL_MATH_INCLUDED

// GENERAL MATH CONSTANTS:
// 
//

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef EPSILON
#define EPSILON 0.000001f
#endif

#ifndef DEG2RAD
#define DEG2RAD (PI/180.0f)
#endif

#ifndef RAD2DEG
#define RAD2DEG (180.0f/PI)
#endif


// GLSL TYPE ANALOGUES:
//
//

#define vecX(T, vecAdress) (((T*)vecAdress)[0])
#define vecY(T, vecAdress) (((T*)vecAdress)[1])
#define vecZ(T, vecAdress) (((T*)vecAdress)[2])
#define vecW(T, vecAdress) (((T*)vecAdress)[3])

//#define USING_MEMCPY

#ifdef USING_MEMCPY
#define mat4_copy(from, to) memcpy((void*)from, (void*)to, sizeof(mat4))
#define vec2_copy(from, to) memcpy((void*)from, (void*)to, sizeof(vec2))
#define vec3_copy(from, to) memcpy((void*)from, (void*)to, sizeof(vec3))
#define vec4_copy(from, to) memcpy((void*)from, (void*)to, sizeof(vec4))
#define quaternion_copy(from, to) vec4_copy(from, to)
#else
#define mat4_copy(from, to) do { GLfloat* a = (GLfloat*)to; GLfloat* b = (GLfloat*)from;\
    a[0] = b[0]; a[1] = b[1]; a[2] = b[2]; a[3] = b[3];\
    a[4] = b[4]; a[5] = b[5]; a[6] = b[6]; a[7] = b[7];\
    a[8] = b[8]; a[9] = b[9]; a[10] = b[10]; a[11] = b[11];\
    a[12] = b[12]; a[13] = b[13]; a[14] = b[14]; a[15] = b[15];\
}while(0)\

#define vec2_copy(from, to) do { GLfloat* a = (GLfloat*)to; GLfloat* b = (GLfloat*)from;\
    a[0] = b[0]; \
    a[1] = b[1]; \
}while(0)\

#define vec3_copy(from, to) do { GLfloat* a = (GLfloat*)to; GLfloat* b = (GLfloat*)from;\
    a[0] = b[0]; \
    a[1] = b[1]; \
    a[2] = b[2]; \
}while(0)\

#define vec4_copy(from, to) do { GLfloat* a = (GLfloat*)to; GLfloat* b = (GLfloat*)from;\
    a[0] = b[0]; \
    a[1] = b[1]; \
    a[2] = b[2]; \
    a[3] = b[3]; \
}while(0)\

#define quaternion_copy(from, to) vec4_copy(from, to)
#endif

#define vec2_def_add(a, b) { a[0] + b[0], a[1] + b[1] }
#define vec3_def_add(a, b) { a[0] + b[0], a[1] + b[1], a[2] + b[2] }
#define vec4_def_add(a, b) { a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3] }

#define vec2_def_multiply(a, b) { a[0] * b[0], a[1] * b[1] }
#define vec3_def_multiply(a, b) { a[0] * b[0], a[1] * b[1], a[2] * b[2] }
#define vec4_def_multiply(a, b) { a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3] }

#define vec2_def_scale(v, scale) { v[0] * scale, v[1] * scale }
#define vec3_def_scale(v, scale) { v[0] * scale, v[1] * scale, v[2] * scale }
#define vec4_def_scale(v, scale) { v[0] * scale, v[1] * scale, v[2] * scale, v[3] * scale }

#define vec2_equal(a, b) (a[0] == b[0] && a[1] == b[1])
#define vec3_equal(a, b) (a[0] == b[0] && a[1] == b[1] && a[2] == b[2])
#define vec4_equal(a, b) (a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3])
#define quaternion_equal(a, b) vec4_equal(a, b)

#define vec2_magnitudef(v) (float)vec2_magnitude(v)
#define vec3_magnitudef(v) (float)vec3_magnitude(v)
#define vec4_magnitudef(v) (float)vec4_magnitude(v)

#define quaternion_normalize(q) vec4_normalize(q)



// GLSL TYPE ANALOGUES:
//
//

typedef GLfloat vec2[2];
typedef GLfloat vec3[3];
typedef GLfloat vec4[4];
typedef GLfloat quaternion[4];

typedef GLint vec2i[2];
typedef GLint vec3i[3];
typedef GLint vec4i[4];

typedef GLuint vec2ui[2];
typedef GLuint vec3ui[3];
typedef GLuint vec4ui[4];

typedef GLboolean vec2b[2];
typedef GLboolean vec3b[3];
typedef GLboolean vec4b[4];

typedef GLfloat mat2[4];
typedef GLfloat mat2x3[6];
typedef GLfloat mat2x4[8];
typedef GLfloat mat3[9];
typedef GLfloat mat3x2[6];
typedef GLfloat mat3x4[9];
typedef GLfloat mat4[16];
typedef GLfloat mat4x2[8];
typedef GLfloat mat4x3[12];

extern const mat4 MAT4_IDENTITY;
extern const vec2 V2_RIGHT;
extern const vec2 V2_UP;
extern const vec2 V2_ZERO;
extern const vec2 V2_ONE;
extern const vec3 V3_RIGHT;
extern const vec3 V3_UP;
extern const vec3 V3_FORWARD;
extern const vec3 V3_LEFT;
extern const vec3 V3_DOWN;
extern const vec3 V3_BACKWARD;
extern const vec3 V3_ZERO;
extern const vec3 V3_ONE;

void vec2_add(vec2 a, vec2 b, vec2 out);
void vec3_add(vec3 a, vec3 b, vec3 out);
void vec4_add(vec4 a, vec4 b, vec4 out);

void vec2_sub(vec2 a, vec2 b, vec2 out);
void vec3_sub(vec3 a, vec3 b, vec3 out);
void vec4_sub(vec4 a, vec4 b, vec4 out);

void vec2_multiply(vec2 a, vec2 b, vec2 out);
void vec3_multiply(vec3 a, vec3 b, vec3 out);
void vec4_multiply(vec4 a, vec4 b, vec4 out);

double vec2_magnitude(const vec2 v);
double vec3_magnitude(const vec3 v);
double vec4_magnitude(const vec4 v);

void vec2_normalize(vec2 v); // normalize vector in-place.
void vec3_normalize(vec3 v); // normalize vector in-place.
void vec4_normalize(vec4 v); // normalize vector in-place.

void quaternion_invert(quaternion q);
void quaternion_multiply(const quaternion left, const quaternion right, quaternion out);
void quaternion_mat4(const quaternion q, mat4 out); // 4x4 matrix from quaternion.

void mat4_multiply(const mat4 left, const mat4 right, mat4 out); // Multiply two 4x4 matrices.


// MISC TYPE FUNCTIONS
//
//

const GLuint size_from_gl_type(const GLenum type);
void upload_from_gl_type(const GLint location, const GLenum type, const GLint elements, const void* data);

#ifdef __cplusplus
}
#endif
#endif


