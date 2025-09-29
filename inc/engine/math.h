#pragma once

#include "glad/glad.h"

#include "stdarg.h"

// GENERAL MATH CONSTANTS:
// 
//

#define PI 3.14159265358979323846f
#define EPSILON 0.000001f
#define DEG2RAD (PI/180.0f)
#define RAD2DEG (180.0f/PI)


#define vec2_print(v) printf("%f, %f, \n", v[0], v[1], v[2])
#define vec3_print(v) printf("%f, %f, %f \n", v[0], v[1], v[2])
#define vec4_print(v) printf("%f, %f, %f, %f \n", v[0], v[1], v[2], v[3])
#define mat4_print(m) printf("%f %f %f %f \n%f %f %f %f \n%f %f %f %f \n%f %f %f %f \n\n", m[0], m[4], m[8], m[12], m[1], m[5], m[9], m[13], m[2], m[6], m[10], m[14], m[3], m[7], m[11], m[15])

// Explicitly use the memcpy function instead of inline version. 
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

// Any macro with "def" in the name means a new object of that type must be initialized by it. for example: vec3 copyOfVector = vec3_def_copy(vector);

#define vec2_def_copy(from) { from[0], from[1] }
#define vec3_def_copy(from) { from[0], from[1], from[2] }
#define vec4_def_copy(from) { from[0], from[1], from[2], from[3] }

#define vec2_def_add(a, b) { a[0] + b[0], a[1] + b[1] }
#define vec3_def_add(a, b) { a[0] + b[0], a[1] + b[1], a[2] + b[2] }
#define vec4_def_add(a, b) { a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3] }

#define vec2_def_sub(a, b) { a[0] - b[0], a[1] - b[1] }
#define vec3_def_sub(a, b) { a[0] - b[0], a[1] - b[1], a[2] - b[2] }
#define vec4_def_sub(a, b) { a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3] }

#define vec2_def_multiply(a, b) { a[0] * b[0], a[1] * b[1] }
#define vec3_def_multiply(a, b) { a[0] * b[0], a[1] * b[1], a[2] * b[2] }
#define vec4_def_multiply(a, b) { a[0] * b[0], a[1] * b[1], a[2] * b[2], a[3] * b[3] }

#define vec2_def_scale(v, scale) { v[0] * scale, v[1] * scale }
#define vec3_def_scale(v, scale) { v[0] * scale, v[1] * scale, v[2] * scale }
#define vec4_def_scale(v, scale) { v[0] * scale, v[1] * scale, v[2] * scale, v[3] * scale }

#define vec3_def_cross(a, b) { a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2], a[0] * b[1] - a[1] * b[0] }

#define vec2_scale(v, scale) v[0] *= scale; v[1] *= scale 
#define vec3_scale(v, scale) v[0] *= scale; v[1] *= scale; v[2] *= scale 
#define vec4_scale(v, scale) v[0] *= scale; v[1] *= scale; v[2] *= scale; v[3] *= scale 

#define vec2_dot(a, b) ((a[0] * b[0]) + (a[1] * b[1]))
#define vec3_dot(a, b) ((a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]))
#define vec4_dot(a, b) ((a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]) + (a[3] * b[3]))

#define vec2_equal(a, b) (a[0] == b[0] && a[1] == b[1])
#define vec3_equal(a, b) (a[0] == b[0] && a[1] == b[1] && a[2] == b[2])
#define vec4_equal(a, b) (a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3])

#define quaternion_equal(a, b) vec4_equal(a, b)

#define quaternion_def_from_axis(axis, angle) { axis[0] * sinf(angle), axis[1] * sinf(angle), axis[2] * sinf(angle), cosf(angle) }

// Short-hand casts to float. I figured might as well just use this because all the mathf functions are just float casted doubles anyway.

#define vec2_magnitudef(v) (float)vec2_magnitude(v)
#define vec3_magnitudef(v) (float)vec3_magnitude(v)
#define vec4_magnitudef(v) (float)vec4_magnitude(v)

#define vec2_sqr_magnitude(v) (v[0] + v[1])
#define vec3_sqr_magnitude(v) (v[0] + v[1] + v[2])
#define vec4_sqr_magnitude(v) (v[0] + v[1] + v[2] + v[3])

// Normalizing a quaternion is the same as a vec4. this alias is just here to make the code easier to understand.
#define quaternion_normalize(q) vec4_normalize(q)

#define vecX(T, vecAdress) (((T*)vecAdress)[0])
#define vecY(T, vecAdress) (((T*)vecAdress)[1])
#define vecZ(T, vecAdress) (((T*)vecAdress)[2])
#define vecW(T, vecAdress) (((T*)vecAdress)[3])

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

// GLSL TYPE CONSTANTS:
//
//

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

extern const vec4 V4_IDENTIY;
extern const vec4 V4_ONE;

void vec2_add(const vec2 a, const vec2 b, vec2 out);
void vec3_add(const vec3 a, const vec3 b, vec3 out);
void vec4_add(const vec4 a, const vec4 b, vec4 out);

void vec2_sub(const vec2 a, const vec2 b, vec2 out);
void vec3_sub(const vec3 a, const vec3 b, vec3 out);
void vec4_sub(const vec4 a, const vec4 b, vec4 out);

void vec2_multiply(const vec2 a, const vec2 b, vec2 out);
void vec3_multiply(const vec3 a, const vec3 b, vec3 out);
void vec4_multiply(const vec4 a, const vec4 b, vec4 out);

void vec3_cross(const vec3 a, const vec3 b, vec3 out);

double vec2_magnitude(const vec2 v);
double vec3_magnitude(const vec3 v);
double vec4_magnitude(const vec4 v);

void vec2_normalize(vec2 v); // normalize vector in-place.
void vec3_normalize(vec3 v); // normalize vector in-place.
void vec4_normalize(vec4 v); // normalize vector in-place.

// Honestly, just use a quaternion instead. it's faster.
void vec3_rotate_axis(const vec3 v, const vec3 axis, double angle, vec3 out);
void vec3_rotate(const vec3 v, const quaternion q, vec3 out);

// Create a quaternion from an axis and an angle. Performs checks and normalizations the def version cannot not.
void quaternion_from_axis(const vec3 axis, const double angle, quaternion out);
void quaternion_invert(quaternion q);
void quaternion_multiply(const quaternion left, const quaternion right, quaternion out);
void mat4_from_quaternion(const quaternion q, mat4 out); // 4x4 matrix from quaternion.

void mat4_multi_multiply (u64 count, ... );
void mat4_multiply(const mat4 left, const mat4 right, mat4 out); // Multiply two 4x4 matrices.
void mat4_translate(const vec3 translation, mat4 out);
void mat4_scale(const vec3 scale, mat4 out);

void mat4_get_forward(const mat4 m, vec3 out);
void mat4_get_right(const mat4 m, vec3 out);
void mat4_get_up(const mat4 m, vec3 out);
void mat4_get_translation(const mat4 m, vec3 out);

float mat4_determinant(const mat4 m);
void mat4_inverse(const mat4 m, mat4 out);
void mat4_transpose(const mat4 m, mat4 out);
void mat4_lookat(const vec3 viewer, const vec3 target, const vec3 up, mat4 out);
void mat4_projection_perspective(const double fov, const double aspect, const double near, const double far, mat4 out);
void mat4_projection_frustum(const double left, const double right, const double top, const double bottom, const double near, const double far, mat4 out);
void mat4_projection_orthographic(const double left, const double right, const double top, const double bottom, const double near, const double far, mat4 out);


// internal size of lookup.
const GLuint size_from_gl_type(const GLenum type);

// internal function to upload data to GPU.
void upload_from_gl_type(const GLint location, const GLenum type, const GLint elements, const void* data);

