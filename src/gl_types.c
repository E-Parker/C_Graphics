#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "gl_types.h"

const GLuint size_from_gl_type(const GLenum type) {
    // I would like to not have a massive switch case but there isn't really a better way.
    // The API just does not have functionality for this since GLfloat and GLint are always assumed to be 
    // exactly the same as C / C++ standards. 
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

double vec2magnitude(const vec2 v) {
    double result = v[0] + v[1];
    return (result != 0.0) ? sqrt(result) : result;
}

double vec3magnitude(const vec3 v){
    double result = v[0] + v[1] + v[2];
    return (result != 0.0) ? sqrt(result) : result;
}

double vec4magnitude(const vec4 v) {
    double result = v[0] + v[1] + v[2] + v[3];
    return (result != 0.0) ? sqrt(result) : result;
}

void quaternion_mat4(const quaternion q, mat4 out) {
   
    float a2 = q[0] * q[0];
    float b2 = q[1] * q[1];
    float c2 = q[2] * q[2];
    float ac = q[0] * q[2];
    float ab = q[0] * q[1];
    float bc = q[1] * q[2];
    float ad = q[3] * q[0];
    float bd = q[3] * q[1];
    float cd = q[3] * q[2];

    mat4 result = { 1.0f - 2.0f * (b2 + c2), 2.0f * (ab + cd), 2.0f * (ac - bd), 0.0f,
                    2.0f * (ab - cd), 1.0f - 2.0f * (a2 + c2), 2.0f * (bc + ad), 0.0f,
                    2.0f * (ac + bd), 2.0f * (bc - ad), 1.0f - 2.0f * (a2 + b2), 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f };

    mat4copy(out, result);
}

void mat4mul(const mat4 a, const mat4 b, mat4 out) {
    // multiply two 4x4 matrices.

    mat4 result = {
        a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12],
        a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13],
        a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14],
        a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15],
        a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12],
        a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13],
        a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14],
        a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15],
        a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12],
        a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13],
        a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14],
        a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15],
        a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12],
        a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13],
        a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14],
        a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15],
    };

    mat4copy(out, result);

}

