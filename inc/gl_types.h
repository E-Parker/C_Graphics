#pragma once

#include <glad/glad.h>

#ifdef __cplusplus
extern "C" {
#endif

// MATH CONSTANT DEFINITIONS:
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

const mat4 MAT4_IDENTITY = {\
    1.0f, 0.0f, 0.0f, 0.0f,\
    0.0f, 1.0f, 0.0f, 0.0f,\
    0.0f, 0.0f, 1.0f, 0.0f,\
    0.0f, 0.0f, 0.0f, 1.0f,\
};

const vec2 V2_RIGHT = {1.0f, 0.0f};
const vec2 V2_UP = { 0.0f, 1.0f };
const vec2 V2_ZERO = { 0.0f, 0.0f };
const vec2 V2_ONE = { 1.0f, 1.0f };

const vec3 V3_RIGHT = { 1.0f, 0.0f, 0.0f };
const vec3 V3_UP = { 0.0f, 1.0f, 0.0f };
const vec3 V3_FORWARD = { 0.0f, 0.0f, 1.0f };
const vec3 V3_LEFT = { -1.0f, 0.0f, 0.0f };
const vec3 V3_DOWN = { 0.0f, -1.0f, 0.0f };
const vec3 V3_BACKWARD = { 0.0f, 0.0f, -1.0f };

const vec3 V3_ZERO = { 0.0f, 0.0f, 0.0f };
const vec3 V3_ONE = { 1.0f, 1.0f, 1.0f };

#define vec2_def_Add(a, b) { a[0] + b[0], a[1] + b[1] }
#define vec3_def_Add(a, b) { a[0] + b[0], a[1] + b[1], a[2] + b[2] }
#define vec4_def_Add(a, b) { a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3] }

#define vec2_def_Mul(a, b) { a[0] * b[0], a[1] * b[1] }
#define vec3_def_Mul(a, b) { a[0] * b[0], a[1] * b[1], a[2] * b[2] }
#define vec4_def_Mul(a, b) { a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3] }

#define vec2_def_Scale(v, scale) { v[0] * scale, v[1] * scale }
#define vec3_def_Scale(v, scale) { v[0] * scale, v[1] * scale, v[2] * scale }
#define vec4_def_Scale(v, scale) { v[0] * scale, v[1] * scale, v[2] * scale, v[3] * scale }

#define vec2Equal(a, b) (a[0] == b[0] && a[1] == b[1])
#define vec3Equal(a, b) (a[0] == b[0] && a[1] == b[1] && a[2] == b[2])
#define vec4Equal(a, b) (a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3])
#define quaternionEqual(a, b) vec4Equal(a, b)

GLuint size_from_gl_type(const GLenum type);
void upload_from_gl_type(const GLint location, const GLenum type, const GLint elements, const void* data);

void vec2_Add(vec2 a, vec2 b, vec2 out);
void vec3_Add(vec3 a, vec3 b, vec3 out);
void vec4_Add(vec4 a, vec4 b, vec4 out);

void vec2_Sub(vec2 a, vec2 b, vec2 out);
void vec3_Sub(vec3 a, vec3 b, vec3 out);
void vec4_Sub(vec4 a, vec4 b, vec4 out);

void vec2_Mul(vec2 a, vec2 b, vec2 out);
void vec3_Mul(vec3 a, vec3 b, vec3 out);
void vec4_Mul(vec4 a, vec4 b, vec4 out);

double vec2magnitude(const vec2 v);
double vec3magnitude(const vec3 v);
double vec4magnitude(const vec4 v);

#define mat4copy(from, to) do{ memcpy((void*)from, (void*)to, sizeof(mat4)); }while(0)

void quaternion_mat4(const quaternion q, mat4 out);

void mat4mul(const mat4 a, const mat4 b, mat4 out);

#ifdef __cplusplus
}
#endif
