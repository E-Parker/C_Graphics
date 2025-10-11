#include "glad/glad.h"

#include "math.h"

#include "engine_core/engine_types.h"
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

const vec4 V4_IDENTIY = { 0.0f, 0.0f, 0.0f, 1.0f };
const vec4 V4_ONE = { 1.0f, 1.0f, 1.0f, 1.0f };

const GLuint size_from_gl_type (const GLenum type) {
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


void upload_from_gl_type (const GLint location, const GLenum type, const GLint elements, const void* data) {
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


void vec2_add (const vec2 a, const vec2 b, vec2 out) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
}

void vec3_add (const vec3 a, const vec3 b, vec3 out) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
}

void vec4_add (const vec4 a, const vec4 b, vec4 out) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
    out[3] = a[3] + b[3];
}


void vec2_sub (const vec2 a, const vec2 b, vec2 out) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
}

void vec3_sub (const vec3 a, const vec3 b, vec3 out) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

void vec4_sub (const vec4 a, const vec4 b, vec4 out) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
    out[3] = a[3] - b[3];
}


void vec2_multiply (const vec2 a, const vec2 b, vec2 out) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
}

void vec3_multiply (const vec3 a, const vec3 b, vec3 out) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
    out[2] = a[2] * b[2];
}

void vec4_multiply (const vec4 a, const vec4 b, vec4 out) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
    out[2] = a[2] * b[2];
    out[3] = a[3] * b[3];
}


void vec3_cross (const vec3 a, const vec3 b, vec3 out) {
    vec3 result = {
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    };
    vec3_copy(result, out);
}


double vec2_magnitude (const vec2 v) {
    double result = v[0] + v[1];
    return (result != 0.0) ? sqrt(result) : 1.0;
}

double vec3_magnitude (const vec3 v){
    double result = v[0] + v[1] + v[2];
    return (result != 0.0) ? sqrt(result) : 1.0;
}

double vec4_magnitude (const vec4 v) {
    double result = v[0] + v[1] + v[2] + v[3];
    return (result != 0.0) ? sqrt(result) : 1.0;
}


// Yes, these redefine the same exact math that is in vec_magnitude but it's probably slightly faster than another function call.
// might be worth actually testing this or something.

void vec2_normalize (vec2 v) {
    float length = v[0] * v[0] + v[1] * v[1];
    length = (length != 0.0f) ? (float)(1.0 / sqrt((double)length)) : 1.0f;
    v[0] *= length;
    v[1] *= length;
}

void vec3_normalize (vec3 v) {
    float length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    length = (length != 0.0f) ? (float)(1.0 / sqrt((double)length)) : 1.0f;
    v[0] *= length;
    v[1] *= length;
    v[2] *= length;
}

void vec4_normalize (vec4 v) {
    float length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
    length = (length != 0.0f) ? (float)(1.0 / sqrt((double)length)) : 1.0f; 
    v[0] *= length;
    v[1] *= length;
    v[2] *= length;
    v[3] *= length;
}

void vec3_rotate_axis(const vec3 v, const vec3 axis, double angle, vec3 out) {
    vec3 axisNormaized = vec3_def_copy(axis);
    vec3_normalize(axisNormaized);

    angle /= 2.0;
    double sinAngle = sin(angle);
    double cosAngle = cos(angle);
    vec3 axisAngle = vec3_def_scale(axisNormaized, sinAngle);

    vec3 axisAngleCrossV = vec3_def_cross(axisAngle, v);
    vec3 axisAngleCrossWV = vec3_def_cross(axisAngleCrossV, v);
    vec3_scale(axisAngleCrossV, (2.0 * sinAngle));
    vec3_scale(axisAngleCrossWV, 2.0);

    vec3 result = vec3_def_add(axisAngleCrossV, axisAngleCrossWV);
}

void vec3_rotate(const vec3 v, const quaternion q, vec3 out) {
    vec3 result = {
        v[0] * (q[0] * q[0] + q[3] * q[3] - q[1] * q[1] - q[2] * q[2]) + v[1] * (2 * q[0] * q[1] - 2 * q[3] * q[2]) + v[2] * (2 * q[0] * q[2] + 2 * q[3] * q[1]),
        v[0] * (2 * q[3] * q[2] + 2 * q[0] * q[1]) + v[1] * (q[3] * q[3] - q[0] * q[0] + q[1] * q[1] - q[2] * q[2]) + v[2] * (-2 * q[3] * q[0] + 2 * q[1] * q[2]),
        v[0] * (-2 * q[3] * q[1] + 2 * q[0] * q[2]) + v[1] * (2 * q[3] * q[0] + 2 * q[1] * q[2]) + v[2] * (q[3] * q[3] - q[0] * q[0] - q[1] * q[1] + q[2] * q[2]),
    };
    vec3_copy(result, out);
}

void quaternion_from_axis(const vec3 axis, const  double angle, quaternion out) {
    vec3 axisNormalized = vec3_def_copy(axis);
    vec3_normalize(axisNormalized);
    quaternion result = quaternion_def_from_axis(axisNormalized, angle);    
    quaternion_copy(result, out);
}

void quaternion_invert (quaternion q) {
    float length = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
    length = (length != 0.0f) ? (float)(1.0 / sqrt((double)length)) : 1.0f;
    q[0] *= -length;
    q[1] *= -length;
    q[2] *= -length;
    q[3] *= length;
}

void quaternion_multiply (const quaternion left, const quaternion right, quaternion out) {
    quaternion result = {
        left[0] * right[3] + left[3] * right[0] + left[1] * right[2] - left[2] * right[1],
        left[1] * right[3] + left[3] * right[1] + left[2] * right[0] - left[0] * right[2],
        left[2] * right[3] + left[3] * right[2] + left[0] * right[1] - left[1] * right[0],
        left[3] * right[3] - left[0] * right[0] - left[1] * right[1] - left[2] * right[2],
    };
    quaternion_copy(result, out);
}

void mat4_from_quaternion (const quaternion q, mat4 out) {
    float a2 = q[0] * q[0];
    float b2 = q[1] * q[1];
    float c2 = q[2] * q[2];
    float ac = q[0] * q[2];
    float ab = q[0] * q[1];
    float bc = q[1] * q[2];
    float ad = q[3] * q[0];
    float bd = q[3] * q[1];
    float cd = q[3] * q[2];

    mat4 result = {
        1.0f - (2.0f * (b2 + c2)),  2.0f * (ab + cd),           2.0f * (ac - bd),           0.0f,
        2.0f * (ab - cd),           1.0f - 2.0f * (a2 + c2),    2.0f * (bc + ad),           0.0f,
        2.0f * (ac + bd),           2.0f * (bc - ad),           1.0f - (2.0f * (a2 + b2)),  0.0f,
        0.0f,                       0.0f,                       0.0f,                       1.0f,
    }; 

    mat4_copy(result, out);
}


void mat4_multi_multiply (u64 count, ... ) {
    va_list args;
    va_start(args, count);

    mat4 result;
    GLfloat* arg = va_arg(args, GLfloat*);
    mat4_copy(arg, result);

    for (u64 i = 1; i < count - 1; ++i) {
        arg = va_arg(args, GLfloat*);
        mat4_multiply(result, arg, result);
    }

    GLfloat* out = va_arg(args, GLfloat*);
    mat4_copy(result, out);
}


void mat4_multiply (const mat4 left, const mat4 right, mat4 out) {
    mat4 result = {
        left[0] * right[0] + left[1] * right[4] + left[2] * right[8] + left[3] * right[12],
        left[0] * right[1] + left[1] * right[5] + left[2] * right[9] + left[3] * right[13],
        left[0] * right[2] + left[1] * right[6] + left[2] * right[10] + left[3] * right[14],
        left[0] * right[3] + left[1] * right[7] + left[2] * right[11] + left[3] * right[15],
        left[4] * right[0] + left[5] * right[4] + left[6] * right[8] + left[7] * right[12],
        left[4] * right[1] + left[5] * right[5] + left[6] * right[9] + left[7] * right[13],
        left[4] * right[2] + left[5] * right[6] + left[6] * right[10] + left[7] * right[14],
        left[4] * right[3] + left[5] * right[7] + left[6] * right[11] + left[7] * right[15],
        left[8] * right[0] + left[9] * right[4] + left[10] * right[8] + left[11] * right[12],
        left[8] * right[1] + left[9] * right[5] + left[10] * right[9] + left[11] * right[13],
        left[8] * right[2] + left[9] * right[6] + left[10] * right[10] + left[11] * right[14],
        left[8] * right[3] + left[9] * right[7] + left[10] * right[11] + left[11] * right[15],
        left[12] * right[0] + left[13] * right[4] + left[14] * right[8] + left[15] * right[12],
        left[12] * right[1] + left[13] * right[5] + left[14] * right[9] + left[15] * right[13],
        left[12] * right[2] + left[13] * right[6] + left[14] * right[10] + left[15] * right[14],
        left[12] * right[3] + left[13] * right[7] + left[14] * right[11] + left[15] * right[15],
    };

    mat4_copy(result, out);
}

void mat4_translate (const vec3 translation, mat4 out) {
    mat4 result = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        translation[0], translation[1], translation[2], 1.0f
    };
    mat4_copy(result, out);
}

void mat4_scale (const vec3 scale, mat4 out) {
    mat4 result = { 
        scale[0], 0.0f, 0.0f, 0.0f,
        0.0f, scale[1], 0.0f, 0.0f,
        0.0f, 0.0f, scale[2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f 
    };
    mat4_copy(result, out);
}

void mat4_get_forward (const mat4 m, vec3 out) {
    out[0] = m[8];
    out[1] = m[9];
    out[2] = m[10];
}

void mat4_get_right (const mat4 m, vec3 out) {
    out[0] = m[0];
    out[1] = m[1];
    out[2] = m[2];   
}

void mat4_get_up (const mat4 m, vec3 out) {
    out[0] = m[4];
    out[1] = m[5];
    out[2] = m[6];    
}

void mat4_get_translation (const mat4 m, vec3 out) {
    out[0] = m[12];
    out[1] = m[13];
    out[2] = m[14];
}

void mat4_transpose (const mat4 m, mat4 out) {
    mat4 result = {
        m[0], m[4], m[8], m[12],
        m[1], m[5], m[9], m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    };
    mat4_copy(result, out);
}

float mat4_determinant(const mat4 m) {
    float b00 = m[0] * m[5] - m[4] * m[1];
    float b01 = m[0] * m[9] - m[8] * m[1];
    float b02 = m[0] * m[13] - m[12] * m[1];
    float b03 = m[4] * m[9] - m[8] * m[5];
    float b04 = m[4] * m[13] - m[12] * m[5];
    float b05 = m[8] * m[13] - m[12] * m[9];
    float b06 = m[2] * m[7] - m[6] * m[3];
    float b07 = m[2] * m[11] - m[10] * m[3];
    float b08 = m[2] * m[15] - m[14] * m[3];
    float b09 = m[6] * m[11] - m[10] * m[7];
    float b10 = m[6] * m[15] - m[14] * m[7];
    float b11 = m[10] * m[15] - m[14] * m[11];

    return b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;
}

void mat4_inverse(const mat4 m, mat4 out) {
    float b00 = m[0] * m[5] - m[4] * m[1];
    float b01 = m[0] * m[9] - m[8] * m[1];
    float b02 = m[0] * m[13] - m[12] * m[1];
    float b03 = m[4] * m[9] - m[8] * m[5];
    float b04 = m[4] * m[13] - m[12] * m[5];
    float b05 = m[8] * m[13] - m[12] * m[9];
    float b06 = m[2] * m[7] - m[6] * m[3];
    float b07 = m[2] * m[11] - m[10] * m[3];
    float b08 = m[2] * m[15] - m[14] * m[3];
    float b09 = m[6] * m[11] - m[10] * m[7];
    float b10 = m[6] * m[15] - m[14] * m[7];
    float b11 = m[10] * m[15] - m[14] * m[11];

    // Calculate the invert determinant 
    float invDet = 1.0f / (b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06);

    mat4 result = {
        (m[5] * b11 - m[9] * b10 + m[13] * b09) * invDet,
        (-m[1] * b11 + m[9] * b08 - m[13] * b07) * invDet,
        (m[1] * b10 - m[5] * b08 + m[13] * b06) * invDet,
        (-m[1] * b09 + m[5] * b07 - m[9] * b06) * invDet,
        (-m[4] * b11 + m[8] * b10 - m[12] * b09) * invDet,
        (m[0] * b11 - m[8] * b08 + m[12] * b07) * invDet,
        (-m[0] * b10 + m[4] * b08 - m[12] * b06) * invDet,
        (m[0] * b09 - m[4] * b07 + m[8] * b06) * invDet,
        (m[7] * b05 - m[11] * b04 + m[15] * b03) * invDet,
        (-m[3] * b05 + m[11] * b02 - m[15] * b01) * invDet,
        (m[3] * b04 - m[7] * b02 + m[15] * b00) * invDet,
        (-m[3] * b03 + m[7] * b01 - m[11] * b00) * invDet,
        (-m[6] * b05 + m[10] * b04 - m[14] * b03) * invDet,
        (m[2] * b05 - m[10] * b02 + m[14] * b01) * invDet,
        (-m[2] * b04 + m[6] * b02 - m[14] * b00) * invDet,
        (m[2] * b03 - m[6] * b01 + m[10] * b00) * invDet,
    };
    
    mat4_copy(result, out);
}

void mat4_lookat (const vec3 viewer, const vec3 target, const vec3 viewerUp, mat4 out) {
    float length = 0.0f;
    float ilength = 0.0f;

    vec3 relativePosition = vec3_def_sub(target, viewer);
    vec3 forward = vec3_def_copy(relativePosition);
    vec3_normalize(forward);

    vec3 side;
    vec3_cross(forward, viewerUp, side);
    vec3_normalize(side);

    vec3 up;
    vec3_cross(side, forward, up);
    vec3_normalize(up);
    
    mat4 result = {
        -side[0], -up[0], -forward[0], 0.0f,
        -side[1], -up[1], -forward[1], 0.0f,
        -side[2], -up[2], -forward[2], 0.0f,
        vec3_dot(side, viewer), vec3_dot(up, viewer), vec3_dot(forward, viewer), 1.0f,
    };

    mat4_copy(result, out);
}

void mat4_projection_perspective (const double fovy, const double aspect, const double near, const double far, mat4 out) {
    double top = near * tan(DEG2RAD * fovy * 0.5);
    double bottom = -top;
    double right = top * aspect;
    double left = -right;
    mat4_projection_frustum (left, right, top, bottom, near, far, out);
}

void mat4_projection_frustum (const double left, const double right, const double top, const double bottom, const double near, const double far, mat4 out) {  
    double invRightToLeft = 1.0 / (right - left);
    double invTopToBottom = 1.0 / (top - bottom);
    double invFarToNear = 1.0 / (far - near);

    mat4 result = {
        (float)(near * 2.0 * invRightToLeft), 0.0f, 0.0f, 0.0f,
        0.0f, (float)(near * 2.0 * invTopToBottom), 0.0f, 0.0f,
        (float)((right + left) * invRightToLeft), 0.0f, (float)(((top + bottom) * invTopToBottom) - ((far + near) * invFarToNear)), -1.0f,
        0.0f, 0.0f, (float)(-far * near * 2.0 * invFarToNear), 0.0f,
    };

    mat4_copy(result, out);
}

void mat4_projection_orthographic (const double left, const double right, const double top, const double bottom, const double near, const double far, mat4 out) {
    double invRightToLeft = 1.0 / (right - left);
    double invTopToBottom = 1.0 / (top - bottom);
    double invFarToNear = 1.0 / (far - near);

    mat4 result = {
        (float)(2.0 * invRightToLeft), 0.0f, 0.0f, (float)(-(left + right) * invRightToLeft),
        0.0f, (float)(2.0 * invTopToBottom), 0.0f, (float)(-(top + bottom) * invTopToBottom),
        0.0f, 0.0f, (float)(-2.0 * invFarToNear),  (float)(-(far + near) * invFarToNear),
        0.0f, 0.0f, 0.0f, 1.0f
    };

    mat4_copy(result, out);
}
