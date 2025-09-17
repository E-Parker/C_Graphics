#include <glad/glad.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "engine/math.h"

// GL TYPE CONSTANT DEFINITIONS:
//
//

const mat4 MAT4_IDENTITY = { \
    1.0f, 0.0f, 0.0f, 0.0f,\
    0.0f, 1.0f, 0.0f, 0.0f,\
    0.0f, 0.0f, 1.0f, 0.0f,\
    0.0f, 0.0f, 0.0f, 1.0f,\
};

const vec2 V2_RIGHT = { 1.0f, 0.0f };
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


const GLuint size_from_gl_type(const GLenum type) {
    // Returns the size of a Open GL type from its GLenum. 
    // 
    // I would like to not have a massive switch case but there isn't really a better way.
    // 
    // The API just does not have functionality for this since GLfloat and GLint are always
    // assumed to be exactly the same as C / C++ standards. 
    // 
    // At least it's effectively a constant lookup.
    //
    switch (type) {
    case GL_FLOAT: return sizeof(GLfloat);
    case GL_FLOAT_VEC2: return sizeof(GLfloat[2]);
    case GL_FLOAT_VEC3: return sizeof(GLfloat[3]);
    case GL_FLOAT_VEC4: return sizeof(GLfloat[4]);
    case GL_INT: return sizeof(GLint);
    case GL_INT_VEC2: return sizeof(GLint[2]);
    case GL_INT_VEC3: return sizeof(GLint[3]);
    case GL_INT_VEC4: return sizeof(GLint[4]);
    case GL_UNSIGNED_INT: return sizeof(GLuint);
    case GL_UNSIGNED_INT_VEC2: return sizeof(GLuint[2]);
    case GL_UNSIGNED_INT_VEC3: return sizeof(GLuint[3]);
    case GL_UNSIGNED_INT_VEC4: return sizeof(GLuint[4]);
    case GL_BOOL: return sizeof(GLboolean);
    case GL_BOOL_VEC2: return sizeof(GLboolean[2]);
    case GL_BOOL_VEC3: return sizeof(GLboolean[3]);
    case GL_BOOL_VEC4: return sizeof(GLboolean[4]);
    case GL_FLOAT_MAT2: return sizeof(GLfloat[4]);
    case GL_FLOAT_MAT2x3: return sizeof(GLfloat[6]);
    case GL_FLOAT_MAT2x4: return sizeof(GLfloat[8]);
    case GL_FLOAT_MAT3: return sizeof(GLfloat[9]);
    case GL_FLOAT_MAT3x2: return sizeof(GLfloat[6]);
    case GL_FLOAT_MAT3x4: return sizeof(GLfloat[12]);
    case GL_FLOAT_MAT4: return sizeof(GLfloat[16]);
    case GL_FLOAT_MAT4x2: return sizeof(GLfloat[8]);
    case GL_FLOAT_MAT4x3: return sizeof(GLfloat[12]);
    default: return 0;
    }
}


void upload_from_gl_type(const GLint location, const GLenum type, const GLint elements, const void* data) {
    switch (type) {
    
    case GL_FLOAT: glUniform1f(location, vecX(float, data)); break;
    case GL_FLOAT_VEC2: glUniform2f(location, vecX(float, data), vecY(float, data)); break;
    case GL_FLOAT_VEC3: glUniform3f(location, vecX(float, data), vecY(float, data), vecZ(float, data)); break;
    case GL_FLOAT_VEC4: glUniform4f(location, vecX(float, data), vecY(float, data), vecZ(float, data), vecW(float, data)); break;

    case GL_INT: glUniform1i(location, vecX(int, data)); break;
    case GL_INT_VEC2: glUniform2i(location, vecX(int, data), vecY(int, data)); break;
    case GL_INT_VEC3: glUniform3i(location, vecX(int, data), vecY(int, data), vecZ(int, data)); break;
    case GL_INT_VEC4: glUniform4i(location, vecX(int, data), vecY(int, data), vecZ(int, data), vecW(int, data)); break;

    case GL_UNSIGNED_INT: glUniform1ui(location, vecX(GLuint, data)); break;
    case GL_UNSIGNED_INT_VEC2: glUniform2ui(location, vecX(GLuint, data), vecY(GLuint, data)); break;
    case GL_UNSIGNED_INT_VEC3: glUniform3ui(location, vecX(GLuint, data), vecY(GLuint, data), vecZ(GLuint, data)); break;
    case GL_UNSIGNED_INT_VEC4: glUniform4ui(location, vecX(GLuint, data), vecY(GLuint, data), vecZ(GLuint, data), vecW(GLuint, data)); break;
    
    case GL_BOOL: glUniform1i(location, (int)vecX(GLboolean, data)); break;
    case GL_BOOL_VEC2: glUniform2i(location, (int)vecX(GLboolean, data), (int)vecY(GLboolean, data)); break;
    case GL_BOOL_VEC3: glUniform3i(location, (int)vecX(GLboolean, data), (int)vecY(GLboolean, data), (int)vecZ(GLboolean, data)); break;
    case GL_BOOL_VEC4: glUniform4i(location, (int)vecX(GLboolean, data), (int)vecY(GLboolean, data), (int)vecZ(GLboolean, data), (int)vecW(GLboolean, data)); break;

    case GL_FLOAT_MAT2: glUniformMatrix2fv(location, elements, false, (GLfloat*)data); break;
    case GL_FLOAT_MAT2x3: glUniformMatrix2x3fv(location, elements, false, (GLfloat*)data); break;
    case GL_FLOAT_MAT2x4: glUniformMatrix2x4fv(location, elements, false, (GLfloat*)data); break;
    case GL_FLOAT_MAT3: glUniformMatrix3fv(location, elements, false, (GLfloat*)data); break;
    case GL_FLOAT_MAT3x2: glUniformMatrix3x2fv(location, elements, false, (GLfloat*)data); break;
    case GL_FLOAT_MAT3x4: glUniformMatrix3x4fv(location, elements, false, (GLfloat*)data); break;
    case GL_FLOAT_MAT4: glUniformMatrix4fv(location, elements, false, (GLfloat*)data); break;
    case GL_FLOAT_MAT4x2:glUniformMatrix4x2fv(location, elements, false, (GLfloat*)data); break;
    case GL_FLOAT_MAT4x3: glUniformMatrix4x3fv(location, elements, false, (GLfloat*)data); break;
    }
}


// GL MATH DEFINITIONS
//
//


void vec2_add(const vec2 a, const vec2 b, vec2 out) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
}

void vec3_add(const vec3 a, const vec3 b, vec3 out) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
}

void vec4_add(const vec4 a, const vec4 b, vec4 out) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
    out[3] = a[3] + b[3];
}


void vec2_sub(const vec2 a, const vec2 b, vec2 out) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
}

void vec3_sub(const vec3 a, const vec3 b, vec3 out) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

void vec4_sub(const vec4 a, const vec4 b, vec4 out) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
    out[3] = a[3] - b[3];
}


void vec2_multiply(const vec2 a, const vec2 b, vec2 out) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
}

void vec3_multiply(const vec3 a, const vec3 b, vec3 out) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
    out[2] = a[2] * b[2];
}

void vec4_multiply(const vec4 a, const vec4 b, vec4 out) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
    out[2] = a[2] * b[2];
    out[3] = a[3] * b[3];
}


void vec3_cross(const vec3 a, const vec3 b, vec3 out) {
    vec3 result = {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    };
    vec3_copy(result, out);
}


double vec2_magnitude(const vec2 v) {
    double result = v[0] + v[1];
    return (result != 0.0) ? sqrt(result) : 1.0;
}

double vec3_magnitude(const vec3 v){
    double result = v[0] + v[1] + v[2];
    return (result != 0.0) ? sqrt(result) : 1.0;
}

double vec4_magnitude(const vec4 v) {
    double result = v[0] + v[1] + v[2] + v[3];
    return (result != 0.0) ? sqrt(result) : 1.0;
}

// Yes, these redefine the same exact math that is in vec_magnitude but it's probably slightly faster than another function call.
// might be worth actually testing this or something.

void vec2_normalize(vec2 v) {
    float length = v[0] * v[0] + v[1] * v[1];
    length = (length != 0.0f) ? (float)(1.0 / sqrt((double)length)) : 1.0f;
    v[0] *= length;
    v[1] *= length;
}

void vec3_normalize(vec3 v) {
    float length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    length = (length != 0.0f) ? (float)(1.0 / sqrt((double)length)) : 1.0f;
    v[0] *= length;
    v[1] *= length;
    v[2] *= length;
}

void vec4_normalize(vec4 v) {
    float length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
    length = (length != 0.0f) ? (float)(1.0 / sqrt((double)length)) : 1.0f; 
    v[0] *= length;
    v[1] *= length;
    v[2] *= length;
    v[3] *= length;
}

void quaternion_invert(quaternion q) {
    float length = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
    length = (length != 0.0f) ? (float)(1.0 / sqrt((double)length)) : 1.0f;
    q[0] *= -length;
    q[1] *= -length;
    q[2] *= -length;
    q[3] *= length;
}

void quaternion_multiply(const quaternion left, const quaternion right, quaternion out) {
    quaternion result = {
        left[0] * right[3] + left[3] * right[0] + left[1] * right[2] - left[2] * right[1],
        left[1] * right[3] + left[3] * right[1] + left[2] * right[0] - left[0] * right[2],
        left[2] * right[3] + left[3] * right[2] + left[0] * right[1] - left[1] * right[0],
        left[3] * right[3] - left[0] * right[0] - left[1] * right[1] - left[2] * right[2],
    };
    quaternion_copy(result, out);
}

void quaternion_mat4(const quaternion q, mat4 out) {
   
    mat4 result = { 0.0f };
    mat4_copy(result, out);
}

void mat4_multiply(const mat4 left, const mat4 right, mat4 out) {
    // multiply two 4x4 matrices.

    mat4 result = {
        left[0] * right[0] + left[4] * right[1] + left[8] * right[2] + left[12] * right[3],
        left[1] * right[0] + left[5] * right[1] + left[9] * right[2] + left[13] * right[3],
        left[2] * right[0] + left[6] * right[1] + left[10] * right[2] + left[14] * right[3],
        left[3] * right[0] + left[7] * right[1] + left[11] * right[2] + left[15] * right[3],
        left[0] * right[4] + left[4] * right[5] + left[8] * right[6] + left[12] * right[7],
        left[1] * right[4] + left[5] * right[5] + left[9] * right[6] + left[13] * right[7],
        left[2] * right[4] + left[6] * right[5] + left[10] * right[6] + left[14] * right[7],
        left[3] * right[4] + left[7] * right[5] + left[11] * right[6] + left[15] * right[7],
        left[0] * right[8] + left[4] * right[9] + left[8] * right[10] + left[12] * right[11],
        left[1] * right[8] + left[5] * right[9] + left[9] * right[10] + left[13] * right[11],
        left[2] * right[8] + left[6] * right[9] + left[10] * right[10] + left[14] * right[11],
        left[3] * right[8] + left[7] * right[9] + left[11] * right[10] + left[15] * right[11],
        left[0] * right[12] + left[4] * right[13] + left[8] * right[14] + left[12] * right[15],
        left[1] * right[12] + left[5] * right[13] + left[9] * right[14] + left[13] * right[15],
        left[2] * right[12] + left[6] * right[13] + left[10] * right[14] + left[14] * right[15],
        left[3] * right[12] + left[7] * right[13] + left[11] * right[14] + left[15] * right[15],
    };
    mat4_copy(result, out);
}


void mat4_translate(const vec3 translation, mat4 out) {
    mat4 result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translation[0], translation[1], translation[2], 1.0f
    };
    mat4_copy(result, out);
}


void mat4_scale(const vec3 scale, mat4 out) {
    mat4 result = { 
        scale[0], 0.0f, 0.0f, 0.0f,
        0.0f, scale[1], 0.0f, 0.0f,
        0.0f, 0.0f, scale[2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f 
    };
    mat4_copy(result, out);
}


void mat4_get_forward(const mat4 m, vec3 out) {
    out[0] = m[2];
    out[1] = m[6];
    out[2] = m[10];
}


void mat4_get_right(const mat4 m, vec3 out) {
    out[0] = m[0];
    out[1] = m[4];
    out[2] = m[8];   
}


void mat4_get_up(const mat4 m, vec3 out) {
    out[0] = m[1];
    out[1] = m[5];
    out[2] = m[9];    
}


void mat4_get_translation(const mat4 m, vec3 out) {
    out[0] = m[3];
    out[1] = m[7];
    out[2] = m[11];
}

void mat4_lookat(const vec3 viewer, const vec3 target, const vec3 up, mat4 out) {
    // look-at matrix implementation borrowed from connor's code.
    // Massively simplified using helper functions. 
    //

    float length = 0.0f;
    float ilength = 0.0f;

    vec3 relativePosition = vec3_def_sub(viewer, target);
    vec3 viewZ = vec3_def_copy(relativePosition);
    vec3_normalize(viewZ);

    vec3 viewX;
    vec3_cross(up, viewZ, viewX);
    vec3_normalize(viewX);

    vec3 viewY;
    vec3_cross(viewZ, viewX, viewY);
    vec3_normalize(viewY);
    
    // Put it together into the final view matrix.
    mat4 result = {
        viewX[0], viewX[1], viewX[2], -vec3_dot(viewX, viewer),
        viewY[0], viewY[1], viewY[2], -vec3_dot(viewY, viewer),
        viewZ[0], viewZ[1], viewZ[2], -vec3_dot(viewZ, viewer),
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    mat4_copy(result, out);
}