#pragma once
#include <cstdlib>
#include <cmath>

using namespace std;

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#define RMAPI inline

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

typedef struct float3 {
    float v[3]{};
} float3;

typedef struct float9 {
    float v[9]{};
} float9;

typedef struct float16 {
    float v[16]{};
} float16;

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

struct Vector2;
struct Vector3;
struct Vector4;

struct Vector2 {
    float x;
    float y;

    RMAPI Vector2 operator+=(Vector2 v);
    RMAPI Vector2 operator-=(Vector2 v);
    RMAPI Vector2 operator*=(Vector2 v);
    RMAPI Vector2 operator/=(Vector2 v);

    RMAPI Vector2 operator+=(float f);
    RMAPI Vector2 operator-=(float f);
    RMAPI Vector2 operator*=(float f);
    RMAPI Vector2 operator/=(float f);

    RMAPI operator Vector3() const;
};

RMAPI Vector2 operator+(Vector2 a, Vector2 b);
RMAPI Vector2 operator-(Vector2 a, Vector2 b);
RMAPI Vector2 operator*(Vector2 a, Vector2 b);
RMAPI Vector2 operator/(Vector2 a, Vector2 b);

RMAPI Vector2 operator+(Vector2 a, float b);
RMAPI Vector2 operator-(Vector2 a, float b);
RMAPI Vector2 operator*(Vector2 a, float b);
RMAPI Vector2 operator/(Vector2 a, float b);

struct Vector3 {
    float x;
    float y;
    float z;

    RMAPI Vector3 operator+=(Vector3 v);
    RMAPI Vector3 operator-=(Vector3 v);
    RMAPI Vector3 operator*=(Vector3 v);
    RMAPI Vector3 operator/=(Vector3 v);

    RMAPI Vector3 operator+=(float f);
    RMAPI Vector3 operator-=(float f);
    RMAPI Vector3 operator*=(float f);
    RMAPI Vector3 operator/=(float f);

    RMAPI operator Vector2() const;
    RMAPI operator Vector4() const;
};

RMAPI Vector3 operator+(Vector3 a, Vector3 b);
RMAPI Vector3 operator-(Vector3 a, Vector3 b);
RMAPI Vector3 operator*(Vector3 a, Vector3 b);
RMAPI Vector3 operator/(Vector3 a, Vector3 b);

RMAPI Vector3 operator+(Vector3 a, float b);
RMAPI Vector3 operator-(Vector3 a, float b);
RMAPI Vector3 operator*(Vector3 a, float b);
RMAPI Vector3 operator/(Vector3 a, float b);

struct Vector4 {
    float x;
    float y;
    float z;
    float w;

    RMAPI Vector4 operator+=(Vector4 v);
    RMAPI Vector4 operator-=(Vector4 v);
    RMAPI Vector4 operator*=(Vector4 v);
    RMAPI Vector4 operator/=(Vector4 v);

    RMAPI Vector4 operator+=(float f);
    RMAPI Vector4 operator-=(float f);
    RMAPI Vector4 operator*=(float f);
    RMAPI Vector4 operator/=(float f);

    RMAPI operator Vector3() const;
};

RMAPI Vector4 operator+(Vector4 a, Vector4 b);
RMAPI Vector4 operator-(Vector4 a, Vector4 b);
RMAPI Vector4 operator*(Vector4 a, Vector4 b);
RMAPI Vector4 operator/(Vector4 a, Vector4 b);

RMAPI Vector4 operator+(Vector4 a, float b);
RMAPI Vector4 operator-(Vector4 a, float b);
RMAPI Vector4 operator*(Vector4 a, float b);
RMAPI Vector4 operator/(Vector4 a, float b);

typedef Vector4 Quaternion;

// Logically column-major (c0 = [m0, m1, m2, m3]).
// Physically row-major (m[0] = m0, m[1] = m4, m[2] = m8, m[3] = m12).
// Transpose to change memory to column-major (OpenGL's memory layout).
typedef struct Matrix {
    float m0, m4, m8, m12;      // Matrix first row (4 components)
    float m1, m5, m9, m13;      // Matrix second row (4 components)
    float m2, m6, m10, m14;     // Matrix third row (4 components)
    float m3, m7, m11, m15;     // Matrix fourth row (4 components)
} Matrix;

RMAPI Matrix operator+(Matrix a, Matrix b);
RMAPI Matrix operator-(Matrix a, Matrix b);
RMAPI Matrix operator*(Matrix a, Matrix b);
// No need for matrix division.

RMAPI Vector4 operator*(Matrix m, Vector4 v);
RMAPI Vector3 operator*(Matrix m, Vector3 v);
RMAPI Vector2 operator*(Matrix m, Vector2 v);
RMAPI Vector3 operator*(Quaternion a, Vector3 b);

constexpr Vector2 V2_RIGHT = { 1.0f, 0.0f };
constexpr Vector2 V2_UP = { 0.0f, 1.0f };

constexpr Vector3 V3_RIGHT = { 1.0f, 0.0f, 0.0f };
constexpr Vector3 V3_UP = { 0.0f, 1.0f, 0.0f };
constexpr Vector3 V3_FORWARD = { 0.0f, 0.0f, 1.0f };
constexpr Vector3 V3_LEFT = { -1.0f, 0.0f, 0.0f };
constexpr Vector3 V3_DOWN = { 0.0f, -1.0f, 0.0f };
constexpr Vector3 V3_BACKWARD = { 0.0f, 0.0f, -1.0f };

constexpr Vector2 V2_ZERO = { 0.0f, 0.0f };
constexpr Vector2 V2_ONE = { 1.0f, 1.0f };
constexpr Vector3 V3_ZERO = { 0.0f, 0.0f, 0.0f };
constexpr Vector3 V3_ONE = { 1.0f, 1.0f, 1.0f };
constexpr Vector4 V4_IDENTITY = { 0.0f, 0.0f, 0.0f, 1.0f };

// Get float vector for Vector3
//#ifndef VecPtr
//#define VecPtr(vec) (ToFloat3(vec).v)
//#endif
//
// Get float vector for Matrix
//#ifndef MatPtr
//#define MatPtr(mat) (ToFloat16(mat).v)
//#endif
//
// (I don't like the above macros because you can just do ToFloatN.v for float*)

// Get Vector3 as float array
RMAPI float3 ToFloat3(Vector3 v)
{
    float3 buffer = { 0 };

    buffer.v[0] = v.x;
    buffer.v[1] = v.y;
    buffer.v[2] = v.z;

    return buffer;
}

RMAPI float9 ToFloat9(Matrix mat)
{
    float9 result = { 0 };

    result.v[0] = mat.m0;
    result.v[1] = mat.m1;
    result.v[2] = mat.m2;

    result.v[3] = mat.m4;
    result.v[4] = mat.m5;
    result.v[5] = mat.m6;

    result.v[6] = mat.m8;
    result.v[7] = mat.m9;
    result.v[8] = mat.m10;

    return result;
}

// Get float array of matrix data (transposes the matrix from row-major to column-major)!
// col0 = v[0-3], col1 = v[4-7], col2 = v[8-11], col3 = v[12-15]. Inspect in debugger!!!!
RMAPI float16 ToFloat16(Matrix mat)
{
    float16 result = { 0 };

    result.v[0] = mat.m0;
    result.v[1] = mat.m1;
    result.v[2] = mat.m2;
    result.v[3] = mat.m3;
    result.v[4] = mat.m4;
    result.v[5] = mat.m5;
    result.v[6] = mat.m6;
    result.v[7] = mat.m7;
    result.v[8] = mat.m8;
    result.v[9] = mat.m9;
    result.v[10] = mat.m10;
    result.v[11] = mat.m11;
    result.v[12] = mat.m12;
    result.v[13] = mat.m13;
    result.v[14] = mat.m14;
    result.v[15] = mat.m15;

    return result;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Scalar math
//----------------------------------------------------------------------------------

// Random value between min and max (can be negative)
RMAPI float Random(float min, float max)
{
    return min + (rand() / ((float)RAND_MAX / (max - min)));
}

// Clamp float value
RMAPI float Clamp(float value, float min, float max)
{
    float result = (value < min) ? min : value;

    if (result > max) result = max;

    return result;
}

// Calculate linear interpolation between two floats
RMAPI float Lerp(float start, float end, float amount)
{
    float result = start + amount * (end - start);

    return result;
}

// 1d tri-linear interpolation
RMAPI float Terp(float A, float B, float C, Vector3 t)
{
    return A * t.x + B * t.y + C * t.z;
}

// Normalize input value within input range
RMAPI float Normalize(float value, float start, float end)
{
    float result = (value - start) / (end - start);

    return result;
}

// Remap input value within input range to output range
RMAPI float Remap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
    float result = (value - inputStart) / (inputEnd - inputStart) * (outputEnd - outputStart) + outputStart;

    return result;
}

// Wrap input value from min to max
RMAPI float Wrap(float value, float min, float max)
{
    float result = value - (max - min) * floorf((value - min) / (max - min));

    return result;
}

// Check whether two given floats are almost equal
RMAPI int Equals(float x, float y)
{
    int result = (fabsf(x - y)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))));

    return result;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Vector2 math
//----------------------------------------------------------------------------------

// Add two vectors (v1 + v2)
RMAPI Vector2 Add(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x + v2.x, v1.y + v2.y };

    return result;
}

// Add vector and float value
RMAPI Vector2 Add(Vector2 v, float add)
{
    Vector2 result = { v.x + add, v.y + add };

    return result;
}

// Subtract two vectors (v1 - v2)
RMAPI Vector2 Subtract(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x - v2.x, v1.y - v2.y };

    return result;
}

// Subtract vector by float value
RMAPI Vector2 Subtract(Vector2 v, float sub)
{
    Vector2 result = { v.x - sub, v.y - sub };

    return result;
}

RMAPI float Length(Vector2 v)
{
    float result = sqrtf((v.x * v.x) + (v.y * v.y));

    return result;
}

// Calculate vector square length
RMAPI float LengthSqr(Vector2 v)
{
    float result = (v.x * v.x) + (v.y * v.y);

    return result;
}

// Calculate two vectors dot product
RMAPI float Dot(Vector2 v1, Vector2 v2)
{
    float result = (v1.x * v2.x + v1.y * v2.y);

    return result;
}

RMAPI float Cross(Vector2 v1, Vector2 v2)
{
    float result = v1.x * v2.y - v1.y * v2.x;

    return result;
}

// Calculate distance between two vectors
RMAPI float Distance(Vector2 v1, Vector2 v2)
{
    float result = sqrtf((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));

    return result;
}

// Calculate square distance between two vectors
RMAPI float DistanceSqr(Vector2 v1, Vector2 v2)
{
    float result = ((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));

    return result;
}

// -1 if below zero, +1 if above zero
RMAPI float Sign(float value)
{
    float result = (value < 0.0f) ? -1.0f : 1.0f;

    return result;
}

// Convert angle to direction
RMAPI Vector2 Direction(float angle)
{
    Vector2 result = { cosf(angle), sinf(angle) };

    return result;
}

// Convert direction to angle
RMAPI float Angle(Vector2 v)
{
    float result = atan2f(v.y, v.x);

    return result;
}

// Unsigned angle between two directions. Range of [0, 180]
RMAPI float UnsignedAngle(Vector2 start, Vector2 end)
{
    float result = 0.0f;

    float dot = start.x * end.x + start.y * end.y;      // Dot product

    float dotClamp = (dot < -1.0f) ? -1.0f : dot;    // Clamp
    if (dotClamp > 1.0f) dotClamp = 1.0f;

    result = acosf(dotClamp);

    return result;
}

// Signed angle between two directions. Range = [-180, 180]
RMAPI float SignedAngle(Vector2 from, Vector2 to)
{
    float sign = Sign(from.x * to.y - from.y * to.x);
    float angle = UnsignedAngle(from, to);
    return sign * angle;
}

// Scale vector (multiply by value)
RMAPI Vector2 Scale(Vector2 v, float scale)
{
    Vector2 result = { v.x * scale, v.y * scale };

    return result;
}

// Project v1 onto v2
RMAPI Vector2 Project(Vector2 v1, Vector2 v2)
{
    float t = Dot(v1, v2) / Dot(v2, v2);
    return { t * v2.x, t * v2.y };
}

// Scalar projection of v1 onto v2
RMAPI float ProjectScalar(Vector2 v1, Vector2 v2)
{
    float t = Dot(v1, v2) / Dot(v2, v2);
    return t;
}

// Projects point P onto line AB
RMAPI Vector2 ProjectPointLine(Vector2 A, Vector2 B, Vector2 P)
{
    Vector2 AB = Subtract(B, A);
    float t = Dot(Subtract(P, A), AB) / Dot(AB, AB);
    return Add(A, Scale(AB, Clamp(t, 0.0f, 1.0f)));
}

// Multiply vector by vector
RMAPI Vector2 Multiply(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x * v2.x, v1.y * v2.y };

    return result;
}

// Negate vector
RMAPI Vector2 Negate(Vector2 v)
{
    Vector2 result = { -v.x, -v.y };

    return result;
}

// Divide vector by vector
RMAPI Vector2 Divide(Vector2 v1, Vector2 v2)
{
    Vector2 result = { v1.x / v2.x, v1.y / v2.y };

    return result;
}

// Normalize provided vector
RMAPI Vector2 Normalize(Vector2 v)
{
    Vector2 result = { 0 };
    float length = sqrtf((v.x * v.x) + (v.y * v.y));

    if (length > 0)
    {
        float ilength = 1.0f / length;
        result.x = v.x * ilength;
        result.y = v.y * ilength;
    }

    return result;
}

// Transforms a Vector2 by a given Matrix
RMAPI Vector2 Multiply(Vector2 v, Matrix mat)
{
    Vector2 result = { 0 };

    float x = v.x;
    float y = v.y;
    float z = 0;

    result.x = mat.m0 * x + mat.m4 * y + mat.m8 * z + mat.m12;
    result.y = mat.m1 * x + mat.m5 * y + mat.m9 * z + mat.m13;

    return result;
}

// Calculate linear interpolation between two vectors
RMAPI Vector2 Lerp(Vector2 v1, Vector2 v2, float amount)
{
    Vector2 result = { 0 };

    result.x = v1.x + amount * (v2.x - v1.x);
    result.y = v1.y + amount * (v2.y - v1.y);

    return result;
}

// 2d tri-linear interpolation
RMAPI Vector2 Terp(Vector2 A, Vector2 B, Vector2 C, Vector3 t)
{
    return A * t.x + B * t.y + C * t.z;
}

// Calculate reflected vector to normal
RMAPI Vector2 Reflect(Vector2 v, Vector2 normal)
{
    Vector2 result = { 0 };

    float dotProduct = (v.x * normal.x + v.y * normal.y); // Dot product

    result.x = v.x - (2.0f * normal.x) * dotProduct;
    result.y = v.y - (2.0f * normal.y) * dotProduct;

    return result;
}

// Rotate vector by angle
RMAPI Vector2 Rotate(Vector2 v, float angle)
{
    Vector2 result = { 0 };

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.x = v.x * cosres - v.y * sinres;
    result.y = v.x * sinres + v.y * cosres;

    return result;
}

// Move Vector towards target
RMAPI Vector2 MoveTowards(Vector2 v, Vector2 target, float maxDistance)
{
    Vector2 result = { 0 };

    float dx = target.x - v.x;
    float dy = target.y - v.y;
    float value = (dx * dx) + (dy * dy);

    if ((value == 0) || ((maxDistance >= 0) && (value <= maxDistance * maxDistance))) return target;

    float dist = sqrtf(value);

    result.x = v.x + dx / dist * maxDistance;
    result.y = v.y + dy / dist * maxDistance;

    return result;
}

// Rotate max radians towards the target
RMAPI Vector2 RotateTowards(Vector2 from, Vector2 to, float maxRadians)
{
    float deltaRadians = UnsignedAngle(from, to);
    return Rotate(from, fminf(deltaRadians, maxRadians) * Sign(Cross(from, to)));
}

// Invert the given vector
RMAPI Vector2 Invert(Vector2 v)
{
    Vector2 result = { 1.0f / v.x, 1.0f / v.y };

    return result;
}

// Clamp the components of the vector between
// min and max values specified by the given vectors
RMAPI Vector2 Clamp(Vector2 v, Vector2 min, Vector2 max)
{
    Vector2 result = { 0 };

    result.x = fminf(max.x, fmaxf(min.x, v.x));
    result.y = fminf(max.y, fmaxf(min.y, v.y));

    return result;
}

// Clamp the magnitude of the vector between two min and max values
RMAPI Vector2 Clamp(Vector2 v, float min, float max)
{
    Vector2 result = v;

    float length = (v.x * v.x) + (v.y * v.y);
    if (length > 0.0f)
    {
        length = sqrtf(length);

        if (length < min)
        {
            float scale = min / length;
            result.x = v.x * scale;
            result.y = v.y * scale;
        }
        else if (length > max)
        {
            float scale = max / length;
            result.x = v.x * scale;
            result.y = v.y * scale;
        }
    }

    return result;
}

// Check whether two given vectors are almost equal
RMAPI bool Equals(Vector2 p, Vector2 q)
{
    bool result = ((fabsf(p.x - q.x)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
        ((fabsf(p.y - q.y)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y)))));

    return result;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Vector3 math
//----------------------------------------------------------------------------------

// Add two vectors
RMAPI Vector3 Add(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };

    return result;
}

// Add vector and float value
RMAPI Vector3 Add(Vector3 v, float add)
{
    Vector3 result = { v.x + add, v.y + add, v.z + add };

    return result;
}

// Subtract two vectors
RMAPI Vector3 Subtract(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };

    return result;
}

// Subtract vector by float value
RMAPI Vector3 Subtract(Vector3 v, float sub)
{
    Vector3 result = { v.x - sub, v.y - sub, v.z - sub };

    return result;
}

// Multiply vector by scalar
RMAPI Vector3 Scale(Vector3 v, float scalar)
{
    Vector3 result = { v.x * scalar, v.y * scalar, v.z * scalar };

    return result;
}

// Multiply vector by vector
RMAPI Vector3 Multiply(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };

    return result;
}

// Calculate two vectors cross product
RMAPI Vector3 Cross(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };

    return result;
}

// Calculate one vector perpendicular vector
RMAPI Vector3 Perpendicular(Vector3 v)
{
    Vector3 result = { 0 };

    float min = (float)fabs(v.x);
    Vector3 cardinalAxis = { 1.0f, 0.0f, 0.0f };

    if (fabsf(v.y) < min)
    {
        min = (float)fabs(v.y);
        Vector3 tmp = { 0.0f, 1.0f, 0.0f };
        cardinalAxis = tmp;
    }

    if (fabsf(v.z) < min)
    {
        Vector3 tmp = { 0.0f, 0.0f, 1.0f };
        cardinalAxis = tmp;
    }

    // Cross product between vectors
    result.x = v.y * cardinalAxis.z - v.z * cardinalAxis.y;
    result.y = v.z * cardinalAxis.x - v.x * cardinalAxis.z;
    result.z = v.x * cardinalAxis.y - v.y * cardinalAxis.x;

    return result;
}

// Calculate vector length
RMAPI float Length(const Vector3 v)
{
    float result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

    return result;
}

// Calculate vector square length
RMAPI float LengthSqr(const Vector3 v)
{
    float result = v.x * v.x + v.y * v.y + v.z * v.z;

    return result;
}

// Calculate two vectors dot product
RMAPI float Dot(Vector3 v1, Vector3 v2)
{
    float result = (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);

    return result;
}

// Calculate distance between two vectors
RMAPI float Distance(Vector3 v1, Vector3 v2)
{
    float result = 0.0f;

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    result = sqrtf(dx * dx + dy * dy + dz * dz);

    return result;
}

// Calculate square distance between two vectors
RMAPI float DistanceSqr(Vector3 v1, Vector3 v2)
{
    float result = 0.0f;

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    result = dx * dx + dy * dy + dz * dz;

    return result;
}

// Project v1 onto v2
RMAPI Vector3 Project(Vector3 v1, Vector3 v2)
{
    float t = Dot(v1, v2) / Dot(v2, v2);
    return { t * v2.x, t * v2.y, t * v2.z };
}

// Scalar projection of v1 onto v2
RMAPI float ProjectScalar(Vector3 v1, Vector3 v2)
{
    float t = Dot(v1, v2) / Dot(v2, v2);
    return t;
}

// Returns the point on line AB nearest to point P
RMAPI Vector3 ProjectPointLine(Vector3 A, Vector3 B, Vector3 P)
{
    Vector3 AB = Subtract(B, A);
    float t = Dot(Subtract(P, A), AB) / Dot(AB, AB);
    return Add(A, Scale(AB, Clamp(t, 0.0f, 1.0f)));
}

// Calculate angle between two vectors
RMAPI float Angle(Vector3 v1, Vector3 v2)
{
    float result = 0.0f;

    Vector3 cross = { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
    float len = sqrtf(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z);
    float dot = (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
    result = atan2f(len, dot);

    return result;
}

// Negate provided vector (invert direction)
RMAPI Vector3 Negate(Vector3 v)
{
    Vector3 result = { -v.x, -v.y, -v.z };

    return result;
}

// Divide vector by vector
RMAPI Vector3 Divide(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.x / v2.x, v1.y / v2.y, v1.z / v2.z };

    return result;
}

// Normalize provided vector
RMAPI Vector3 Normalize(Vector3 v)
{
    Vector3 result = v;

    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f / length;

    result.x *= ilength;
    result.y *= ilength;
    result.z *= ilength;

    return result;
}

// Orthonormalize provided vectors
// Makes vectors normalized and orthogonal to each other
// Gram-Schmidt function implementation
RMAPI void OrthoNormalize(Vector3* v1, Vector3* v2)
{
    float length = 0.0f;
    float ilength = 0.0f;

    // Vector3Normalize(*v1);
    Vector3 v = *v1;
    length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f / length;
    v1->x *= ilength;
    v1->y *= ilength;
    v1->z *= ilength;

    // Vector3CrossProduct(*v1, *v2)
    Vector3 vn1 = { v1->y * v2->z - v1->z * v2->y, v1->z * v2->x - v1->x * v2->z, v1->x * v2->y - v1->y * v2->x };

    // Vector3Normalize(vn1);
    v = vn1;
    length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f / length;
    vn1.x *= ilength;
    vn1.y *= ilength;
    vn1.z *= ilength;

    // Vector3CrossProduct(vn1, *v1)
    Vector3 vn2 = { vn1.y * v1->z - vn1.z * v1->y, vn1.z * v1->x - vn1.x * v1->z, vn1.x * v1->y - vn1.y * v1->x };

    *v2 = vn2;
}

// Transforms a Vector3 by a given Matrix
RMAPI Vector3 Multiply(Vector3 v, Matrix mat)
{
    Vector3 result = { 0 };

    float x = v.x;
    float y = v.y;
    float z = v.z;

    result.x = mat.m0 * x + mat.m4 * y + mat.m8 * z + mat.m12;
    result.y = mat.m1 * x + mat.m5 * y + mat.m9 * z + mat.m13;
    result.z = mat.m2 * x + mat.m6 * y + mat.m10 * z + mat.m14;

    return result;
}

// Transform a vector by quaternion rotation
RMAPI Vector3 Rotate(Vector3 v, Quaternion q)
{
    Vector3 result = { 0 };

    result.x = v.x * (q.x * q.x + q.w * q.w - q.y * q.y - q.z * q.z) + v.y * (2 * q.x * q.y - 2 * q.w * q.z) + v.z * (2 * q.x * q.z + 2 * q.w * q.y);
    result.y = v.x * (2 * q.w * q.z + 2 * q.x * q.y) + v.y * (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z) + v.z * (-2 * q.w * q.x + 2 * q.y * q.z);
    result.z = v.x * (-2 * q.w * q.y + 2 * q.x * q.z) + v.y * (2 * q.w * q.x + 2 * q.y * q.z) + v.z * (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);

    return result;
}

// Rotates a vector around an axis
RMAPI Vector3 Rotate(Vector3 v, Vector3 axis, float angle)
{
    // Using Euler-Rodrigues Formula
    // Ref.: https://en.wikipedia.org/w/index.php?title=Euler%E2%80%93Rodrigues_formula

    Vector3 result = v;

    // Vector3Normalize(axis);
    float length = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f / length;
    axis.x *= ilength;
    axis.y *= ilength;
    axis.z *= ilength;

    angle /= 2.0f;
    float a = sinf(angle);
    float b = axis.x * a;
    float c = axis.y * a;
    float d = axis.z * a;
    a = cosf(angle);
    Vector3 w = { b, c, d };

    // Vector3CrossProduct(w, v)
    Vector3 wv = { w.y * v.z - w.z * v.y, w.z * v.x - w.x * v.z, w.x * v.y - w.y * v.x };

    // Vector3CrossProduct(w, wv)
    Vector3 wwv = { w.y * wv.z - w.z * wv.y, w.z * wv.x - w.x * wv.z, w.x * wv.y - w.y * wv.x };

    // Vector3Scale(wv, 2 * a)
    a *= 2;
    wv.x *= a;
    wv.y *= a;
    wv.z *= a;

    // Vector3Scale(wwv, 2)
    wwv.x *= 2;
    wwv.y *= 2;
    wwv.z *= 2;

    result.x += wv.x;
    result.y += wv.y;
    result.z += wv.z;

    result.x += wwv.x;
    result.y += wwv.y;
    result.z += wwv.z;

    return result;
}

// Calculate linear interpolation between two vectors
RMAPI Vector3 Lerp(Vector3 v1, Vector3 v2, float amount)
{
    Vector3 result = { 0 };

    result.x = v1.x + amount * (v2.x - v1.x);
    result.y = v1.y + amount * (v2.y - v1.y);
    result.z = v1.z + amount * (v2.z - v1.z);

    return result;
}

// 3d tri-linear interpolation
RMAPI Vector3 Terp(Vector3 A, Vector3 B, Vector3 C, Vector3 t)
{
    return A * t.x + B * t.y + C * t.z;
}

// Calculate reflected vector to normal
RMAPI Vector3 Reflect(Vector3 v, Vector3 normal)
{
    Vector3 result = { 0 };

    // I is the original vector
    // N is the normal of the incident plane
    // R = I - (2*N*(DotProduct[I, N]))

    float dotProduct = (v.x * normal.x + v.y * normal.y + v.z * normal.z);

    result.x = v.x - (2.0f * normal.x) * dotProduct;
    result.y = v.y - (2.0f * normal.y) * dotProduct;
    result.z = v.z - (2.0f * normal.z) * dotProduct;

    return result;
}

// Get min value for each pair of components
RMAPI Vector3 Min(Vector3 v1, Vector3 v2)
{
    Vector3 result = { 0 };

    result.x = fminf(v1.x, v2.x);
    result.y = fminf(v1.y, v2.y);
    result.z = fminf(v1.z, v2.z);

    return result;
}

// Get max value for each pair of components
RMAPI Vector3 Max(Vector3 v1, Vector3 v2)
{
    Vector3 result = { 0 };

    result.x = fmaxf(v1.x, v2.x);
    result.y = fmaxf(v1.y, v2.y);
    result.z = fmaxf(v1.z, v2.z);

    return result;
}

// Compute barycenter coordinates (u, v, w) for point p with respect to triangle (a, b, c)
// NOTE: Assumes P is on the plane of the triangle
RMAPI Vector3 Barycenter(Vector3 p, Vector3 a, Vector3 b, Vector3 c)
{
    Vector3 result = { 0 };

    Vector3 v0 = { b.x - a.x, b.y - a.y, b.z - a.z };   // Vector3Subtract(b, a)
    Vector3 v1 = { c.x - a.x, c.y - a.y, c.z - a.z };   // Vector3Subtract(c, a)
    Vector3 v2 = { p.x - a.x, p.y - a.y, p.z - a.z };   // Vector3Subtract(p, a)
    float d00 = (v0.x * v0.x + v0.y * v0.y + v0.z * v0.z);    // Vector3DotProduct(v0, v0)
    float d01 = (v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);    // Vector3DotProduct(v0, v1)
    float d11 = (v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);    // Vector3DotProduct(v1, v1)
    float d20 = (v2.x * v0.x + v2.y * v0.y + v2.z * v0.z);    // Vector3DotProduct(v2, v0)
    float d21 = (v2.x * v1.x + v2.y * v1.y + v2.z * v1.z);    // Vector3DotProduct(v2, v1)

    float denom = d00 * d11 - d01 * d01;

    result.y = (d11 * d20 - d01 * d21) / denom;
    result.z = (d00 * d21 - d01 * d20) / denom;
    result.x = 1.0f - (result.z + result.y);

    return result;
}

// Projects a Vector3 from screen space into object space
// NOTE: We are avoiding calling other raymath functions despite available
RMAPI Vector3 Unproject(Vector3 source, Matrix projection, Matrix view)
{
    Vector3 result = { 0 };

    // Calculate unprojected matrix (multiply view matrix by projection matrix) and invert it
    Matrix matViewProj = {      // MatrixMultiply(view, projection);
        view.m0 * projection.m0 + view.m1 * projection.m4 + view.m2 * projection.m8 + view.m3 * projection.m12,
        view.m0 * projection.m1 + view.m1 * projection.m5 + view.m2 * projection.m9 + view.m3 * projection.m13,
        view.m0 * projection.m2 + view.m1 * projection.m6 + view.m2 * projection.m10 + view.m3 * projection.m14,
        view.m0 * projection.m3 + view.m1 * projection.m7 + view.m2 * projection.m11 + view.m3 * projection.m15,
        view.m4 * projection.m0 + view.m5 * projection.m4 + view.m6 * projection.m8 + view.m7 * projection.m12,
        view.m4 * projection.m1 + view.m5 * projection.m5 + view.m6 * projection.m9 + view.m7 * projection.m13,
        view.m4 * projection.m2 + view.m5 * projection.m6 + view.m6 * projection.m10 + view.m7 * projection.m14,
        view.m4 * projection.m3 + view.m5 * projection.m7 + view.m6 * projection.m11 + view.m7 * projection.m15,
        view.m8 * projection.m0 + view.m9 * projection.m4 + view.m10 * projection.m8 + view.m11 * projection.m12,
        view.m8 * projection.m1 + view.m9 * projection.m5 + view.m10 * projection.m9 + view.m11 * projection.m13,
        view.m8 * projection.m2 + view.m9 * projection.m6 + view.m10 * projection.m10 + view.m11 * projection.m14,
        view.m8 * projection.m3 + view.m9 * projection.m7 + view.m10 * projection.m11 + view.m11 * projection.m15,
        view.m12 * projection.m0 + view.m13 * projection.m4 + view.m14 * projection.m8 + view.m15 * projection.m12,
        view.m12 * projection.m1 + view.m13 * projection.m5 + view.m14 * projection.m9 + view.m15 * projection.m13,
        view.m12 * projection.m2 + view.m13 * projection.m6 + view.m14 * projection.m10 + view.m15 * projection.m14,
        view.m12 * projection.m3 + view.m13 * projection.m7 + view.m14 * projection.m11 + view.m15 * projection.m15 };

    // Calculate inverted matrix -> MatrixInvert(matViewProj);
    // Cache the matrix values (speed optimization)
    float a00 = matViewProj.m0, a01 = matViewProj.m1, a02 = matViewProj.m2, a03 = matViewProj.m3;
    float a10 = matViewProj.m4, a11 = matViewProj.m5, a12 = matViewProj.m6, a13 = matViewProj.m7;
    float a20 = matViewProj.m8, a21 = matViewProj.m9, a22 = matViewProj.m10, a23 = matViewProj.m11;
    float a30 = matViewProj.m12, a31 = matViewProj.m13, a32 = matViewProj.m14, a33 = matViewProj.m15;

    float b00 = a00 * a11 - a01 * a10;
    float b01 = a00 * a12 - a02 * a10;
    float b02 = a00 * a13 - a03 * a10;
    float b03 = a01 * a12 - a02 * a11;
    float b04 = a01 * a13 - a03 * a11;
    float b05 = a02 * a13 - a03 * a12;
    float b06 = a20 * a31 - a21 * a30;
    float b07 = a20 * a32 - a22 * a30;
    float b08 = a20 * a33 - a23 * a30;
    float b09 = a21 * a32 - a22 * a31;
    float b10 = a21 * a33 - a23 * a31;
    float b11 = a22 * a33 - a23 * a32;

    // Calculate the invert determinant (inlined to avoid double-caching)
    float invDet = 1.0f / (b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06);

    Matrix matViewProjInv = {
        (a11 * b11 - a12 * b10 + a13 * b09) * invDet,
        (-a01 * b11 + a02 * b10 - a03 * b09) * invDet,
        (a31 * b05 - a32 * b04 + a33 * b03) * invDet,
        (-a21 * b05 + a22 * b04 - a23 * b03) * invDet,
        (-a10 * b11 + a12 * b08 - a13 * b07) * invDet,
        (a00 * b11 - a02 * b08 + a03 * b07) * invDet,
        (-a30 * b05 + a32 * b02 - a33 * b01) * invDet,
        (a20 * b05 - a22 * b02 + a23 * b01) * invDet,
        (a10 * b10 - a11 * b08 + a13 * b06) * invDet,
        (-a00 * b10 + a01 * b08 - a03 * b06) * invDet,
        (a30 * b04 - a31 * b02 + a33 * b00) * invDet,
        (-a20 * b04 + a21 * b02 - a23 * b00) * invDet,
        (-a10 * b09 + a11 * b07 - a12 * b06) * invDet,
        (a00 * b09 - a01 * b07 + a02 * b06) * invDet,
        (-a30 * b03 + a31 * b01 - a32 * b00) * invDet,
        (a20 * b03 - a21 * b01 + a22 * b00) * invDet };

    // Create quaternion from source point
    Quaternion quat = { source.x, source.y, source.z, 1.0f };

    // Multiply quat point by unprojecte matrix
    Quaternion qtransformed = {     // QuaternionTransform(quat, matViewProjInv)
        matViewProjInv.m0 * quat.x + matViewProjInv.m4 * quat.y + matViewProjInv.m8 * quat.z + matViewProjInv.m12 * quat.w,
        matViewProjInv.m1 * quat.x + matViewProjInv.m5 * quat.y + matViewProjInv.m9 * quat.z + matViewProjInv.m13 * quat.w,
        matViewProjInv.m2 * quat.x + matViewProjInv.m6 * quat.y + matViewProjInv.m10 * quat.z + matViewProjInv.m14 * quat.w,
        matViewProjInv.m3 * quat.x + matViewProjInv.m7 * quat.y + matViewProjInv.m11 * quat.z + matViewProjInv.m15 * quat.w };

    // Normalized world points in vectors
    result.x = qtransformed.x / qtransformed.w;
    result.y = qtransformed.y / qtransformed.w;
    result.z = qtransformed.z / qtransformed.w;

    return result;
}

// Invert the given vector
RMAPI Vector3 Invert(Vector3 v)
{
    Vector3 result = { 1.0f / v.x, 1.0f / v.y, 1.0f / v.z };

    return result;
}

// Clamp the components of the vector between
// min and max values specified by the given vectors
RMAPI Vector3 Clamp(Vector3 v, Vector3 min, Vector3 max)
{
    Vector3 result = { 0 };

    result.x = fminf(max.x, fmaxf(min.x, v.x));
    result.y = fminf(max.y, fmaxf(min.y, v.y));
    result.z = fminf(max.z, fmaxf(min.z, v.z));

    return result;
}

// Clamp the magnitude of the vector between two values
RMAPI Vector3 Clamp(Vector3 v, float min, float max)
{
    Vector3 result = v;

    float length = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
    if (length > 0.0f)
    {
        length = sqrtf(length);

        if (length < min)
        {
            float scale = min / length;
            result.x = v.x * scale;
            result.y = v.y * scale;
            result.z = v.z * scale;
        }
        else if (length > max)
        {
            float scale = max / length;
            result.x = v.x * scale;
            result.y = v.y * scale;
            result.z = v.z * scale;
        }
    }

    return result;
}

// Check whether two given vectors are almost equal
RMAPI int Equals(Vector3 p, Vector3 q)
{
    int result = ((fabsf(p.x - q.x)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
        ((fabsf(p.y - q.y)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
        ((fabsf(p.z - q.z)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z)))));

    return result;
}

// Compute the direction of a refracted ray where v specifies the
// normalized direction of the incoming ray, n specifies the
// normalized normal vector of the interface of two optical media,
// and r specifies the ratio of the refractive index of the medium
// from where the ray comes to the refractive index of the medium
// on the other side of the surface
RMAPI Vector3 Refract(Vector3 v, Vector3 n, float r)
{
    Vector3 result = { 0 };

    float dot = v.x * n.x + v.y * n.y + v.z * n.z;
    float d = 1.0f - r * r * (1.0f - dot * dot);

    if (d >= 0.0f)
    {
        d = sqrtf(d);
        v.x = r * v.x - (r * dot + d) * n.x;
        v.y = r * v.y - (r * dot + d) * n.y;
        v.z = r * v.z - (r * dot + d) * n.z;

        result = v;
    }

    return result;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Matrix math
//----------------------------------------------------------------------------------

// Compute matrix determinant
RMAPI float Determinant(Matrix mat)
{
    float result = 0.0f;

    // Cache the matrix values (speed optimization)
    float a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
    float a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
    float a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
    float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

    result = a30 * a21 * a12 * a03 - a20 * a31 * a12 * a03 - a30 * a11 * a22 * a03 + a10 * a31 * a22 * a03 +
        a20 * a11 * a32 * a03 - a10 * a21 * a32 * a03 - a30 * a21 * a02 * a13 + a20 * a31 * a02 * a13 +
        a30 * a01 * a22 * a13 - a00 * a31 * a22 * a13 - a20 * a01 * a32 * a13 + a00 * a21 * a32 * a13 +
        a30 * a11 * a02 * a23 - a10 * a31 * a02 * a23 - a30 * a01 * a12 * a23 + a00 * a31 * a12 * a23 +
        a10 * a01 * a32 * a23 - a00 * a11 * a32 * a23 - a20 * a11 * a02 * a33 + a10 * a21 * a02 * a33 +
        a20 * a01 * a12 * a33 - a00 * a21 * a12 * a33 - a10 * a01 * a22 * a33 + a00 * a11 * a22 * a33;

    return result;
}

// Get the trace of the matrix (sum of the values along the diagonal)
RMAPI float Trace(Matrix mat)
{
    float result = (mat.m0 + mat.m5 + mat.m10 + mat.m15);

    return result;
}

// Transposes provided matrix
RMAPI Matrix Transpose(Matrix mat)
{
    Matrix result = { 0 };

    result.m0 = mat.m0;
    result.m1 = mat.m4;
    result.m2 = mat.m8;
    result.m3 = mat.m12;
    result.m4 = mat.m1;
    result.m5 = mat.m5;
    result.m6 = mat.m9;
    result.m7 = mat.m13;
    result.m8 = mat.m2;
    result.m9 = mat.m6;
    result.m10 = mat.m10;
    result.m11 = mat.m14;
    result.m12 = mat.m3;
    result.m13 = mat.m7;
    result.m14 = mat.m11;
    result.m15 = mat.m15;

    return result;
}

// Invert provided matrix
RMAPI Matrix Invert(Matrix mat)
{
    Matrix result = { 0 };

    // Cache the matrix values (speed optimization)
    float a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
    float a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
    float a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
    float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

    float b00 = a00 * a11 - a01 * a10;
    float b01 = a00 * a12 - a02 * a10;
    float b02 = a00 * a13 - a03 * a10;
    float b03 = a01 * a12 - a02 * a11;
    float b04 = a01 * a13 - a03 * a11;
    float b05 = a02 * a13 - a03 * a12;
    float b06 = a20 * a31 - a21 * a30;
    float b07 = a20 * a32 - a22 * a30;
    float b08 = a20 * a33 - a23 * a30;
    float b09 = a21 * a32 - a22 * a31;
    float b10 = a21 * a33 - a23 * a31;
    float b11 = a22 * a33 - a23 * a32;

    // Calculate the invert determinant (inlined to avoid double-caching)
    float invDet = 1.0f / (b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06);

    result.m0 = (a11 * b11 - a12 * b10 + a13 * b09) * invDet;
    result.m1 = (-a01 * b11 + a02 * b10 - a03 * b09) * invDet;
    result.m2 = (a31 * b05 - a32 * b04 + a33 * b03) * invDet;
    result.m3 = (-a21 * b05 + a22 * b04 - a23 * b03) * invDet;
    result.m4 = (-a10 * b11 + a12 * b08 - a13 * b07) * invDet;
    result.m5 = (a00 * b11 - a02 * b08 + a03 * b07) * invDet;
    result.m6 = (-a30 * b05 + a32 * b02 - a33 * b01) * invDet;
    result.m7 = (a20 * b05 - a22 * b02 + a23 * b01) * invDet;
    result.m8 = (a10 * b10 - a11 * b08 + a13 * b06) * invDet;
    result.m9 = (-a00 * b10 + a01 * b08 - a03 * b06) * invDet;
    result.m10 = (a30 * b04 - a31 * b02 + a33 * b00) * invDet;
    result.m11 = (-a20 * b04 + a21 * b02 - a23 * b00) * invDet;
    result.m12 = (-a10 * b09 + a11 * b07 - a12 * b06) * invDet;
    result.m13 = (a00 * b09 - a01 * b07 + a02 * b06) * invDet;
    result.m14 = (-a30 * b03 + a31 * b01 - a32 * b00) * invDet;
    result.m15 = (a20 * b03 - a21 * b01 + a22 * b00) * invDet;

    return result;
}

// Get identity matrix
RMAPI Matrix MatrixIdentity(void)
{
    Matrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Add two matrices
RMAPI Matrix Add(Matrix left, Matrix right)
{
    Matrix result = { 0 };

    result.m0 = left.m0 + right.m0;
    result.m1 = left.m1 + right.m1;
    result.m2 = left.m2 + right.m2;
    result.m3 = left.m3 + right.m3;
    result.m4 = left.m4 + right.m4;
    result.m5 = left.m5 + right.m5;
    result.m6 = left.m6 + right.m6;
    result.m7 = left.m7 + right.m7;
    result.m8 = left.m8 + right.m8;
    result.m9 = left.m9 + right.m9;
    result.m10 = left.m10 + right.m10;
    result.m11 = left.m11 + right.m11;
    result.m12 = left.m12 + right.m12;
    result.m13 = left.m13 + right.m13;
    result.m14 = left.m14 + right.m14;
    result.m15 = left.m15 + right.m15;

    return result;
}

// Subtract two matrices (left - right)
RMAPI Matrix Subtract(Matrix left, Matrix right)
{
    Matrix result = { 0 };

    result.m0 = left.m0 - right.m0;
    result.m1 = left.m1 - right.m1;
    result.m2 = left.m2 - right.m2;
    result.m3 = left.m3 - right.m3;
    result.m4 = left.m4 - right.m4;
    result.m5 = left.m5 - right.m5;
    result.m6 = left.m6 - right.m6;
    result.m7 = left.m7 - right.m7;
    result.m8 = left.m8 - right.m8;
    result.m9 = left.m9 - right.m9;
    result.m10 = left.m10 - right.m10;
    result.m11 = left.m11 - right.m11;
    result.m12 = left.m12 - right.m12;
    result.m13 = left.m13 - right.m13;
    result.m14 = left.m14 - right.m14;
    result.m15 = left.m15 - right.m15;

    return result;
}

// Get two matrix multiplication
// NOTE: When multiplying matrices... the order matters!
RMAPI Matrix Multiply(Matrix left, Matrix right)
{
    Matrix result = { 0 };

    result.m0 = left.m0 * right.m0 + left.m1 * right.m4 + left.m2 * right.m8 + left.m3 * right.m12;
    result.m1 = left.m0 * right.m1 + left.m1 * right.m5 + left.m2 * right.m9 + left.m3 * right.m13;
    result.m2 = left.m0 * right.m2 + left.m1 * right.m6 + left.m2 * right.m10 + left.m3 * right.m14;
    result.m3 = left.m0 * right.m3 + left.m1 * right.m7 + left.m2 * right.m11 + left.m3 * right.m15;
    result.m4 = left.m4 * right.m0 + left.m5 * right.m4 + left.m6 * right.m8 + left.m7 * right.m12;
    result.m5 = left.m4 * right.m1 + left.m5 * right.m5 + left.m6 * right.m9 + left.m7 * right.m13;
    result.m6 = left.m4 * right.m2 + left.m5 * right.m6 + left.m6 * right.m10 + left.m7 * right.m14;
    result.m7 = left.m4 * right.m3 + left.m5 * right.m7 + left.m6 * right.m11 + left.m7 * right.m15;
    result.m8 = left.m8 * right.m0 + left.m9 * right.m4 + left.m10 * right.m8 + left.m11 * right.m12;
    result.m9 = left.m8 * right.m1 + left.m9 * right.m5 + left.m10 * right.m9 + left.m11 * right.m13;
    result.m10 = left.m8 * right.m2 + left.m9 * right.m6 + left.m10 * right.m10 + left.m11 * right.m14;
    result.m11 = left.m8 * right.m3 + left.m9 * right.m7 + left.m10 * right.m11 + left.m11 * right.m15;
    result.m12 = left.m12 * right.m0 + left.m13 * right.m4 + left.m14 * right.m8 + left.m15 * right.m12;
    result.m13 = left.m12 * right.m1 + left.m13 * right.m5 + left.m14 * right.m9 + left.m15 * right.m13;
    result.m14 = left.m12 * right.m2 + left.m13 * right.m6 + left.m14 * right.m10 + left.m15 * right.m14;
    result.m15 = left.m12 * right.m3 + left.m13 * right.m7 + left.m14 * right.m11 + left.m15 * right.m15;

    return result;
}

// Get translation matrix
RMAPI Matrix Translate(float x, float y, float z)
{
    Matrix result = { 1.0f, 0.0f, 0.0f, x,
                      0.0f, 1.0f, 0.0f, y,
                      0.0f, 0.0f, 1.0f, z,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Create rotation matrix from axis and angle
// NOTE: Angle should be provided in radians
RMAPI Matrix Rotate(Vector3 axis, float angle)
{
    Matrix result = { 0 };

    float x = axis.x, y = axis.y, z = axis.z;

    float lengthSquared = x * x + y * y + z * z;

    if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f))
    {
        float ilength = 1.0f / sqrtf(lengthSquared);
        x *= ilength;
        y *= ilength;
        z *= ilength;
    }

    float sinres = sinf(angle);
    float cosres = cosf(angle);
    float t = 1.0f - cosres;

    result.m0 = x * x * t + cosres;
    result.m1 = y * x * t + z * sinres;
    result.m2 = z * x * t - y * sinres;
    result.m3 = 0.0f;

    result.m4 = x * y * t - z * sinres;
    result.m5 = y * y * t + cosres;
    result.m6 = z * y * t + x * sinres;
    result.m7 = 0.0f;

    result.m8 = x * z * t + y * sinres;
    result.m9 = y * z * t - x * sinres;
    result.m10 = z * z * t + cosres;
    result.m11 = 0.0f;

    result.m12 = 0.0f;
    result.m13 = 0.0f;
    result.m14 = 0.0f;
    result.m15 = 1.0f;

    return result;
}

// Get x-rotation matrix
// NOTE: Angle must be provided in radians
RMAPI Matrix RotateX(float angle)
{
    Matrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }; // MatrixIdentity()

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.m5 = cosres;
    result.m6 = sinres;
    result.m9 = -sinres;
    result.m10 = cosres;

    return result;
}

// Get y-rotation matrix
// NOTE: Angle must be provided in radians
RMAPI Matrix RotateY(float angle)
{
    Matrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }; // MatrixIdentity()

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.m0 = cosres;
    result.m2 = -sinres;
    result.m8 = sinres;
    result.m10 = cosres;

    return result;
}

// Get z-rotation matrix
// NOTE: Angle must be provided in radians
RMAPI Matrix RotateZ(float angle)
{
    Matrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }; // MatrixIdentity()

    float cosres = cosf(angle);
    float sinres = sinf(angle);

    result.m0 = cosres;
    result.m1 = sinres;
    result.m4 = -sinres;
    result.m5 = cosres;

    return result;
}


// Get xyz-rotation matrix
// NOTE: Angle must be provided in radians
RMAPI Matrix RotateXYZ(Vector3 angle)
{
    Matrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }; // MatrixIdentity()

    float cosz = cosf(-angle.z);
    float sinz = sinf(-angle.z);
    float cosy = cosf(-angle.y);
    float siny = sinf(-angle.y);
    float cosx = cosf(-angle.x);
    float sinx = sinf(-angle.x);

    result.m0 = cosz * cosy;
    result.m1 = (cosz * siny * sinx) - (sinz * cosx);
    result.m2 = (cosz * siny * cosx) + (sinz * sinx);

    result.m4 = sinz * cosy;
    result.m5 = (sinz * siny * sinx) + (cosz * cosx);
    result.m6 = (sinz * siny * cosx) - (cosz * sinx);

    result.m8 = -siny;
    result.m9 = cosy * sinx;
    result.m10 = cosy * cosx;

    return result;
}

// Get zyx-rotation matrix
// NOTE: Angle must be provided in radians
RMAPI Matrix RotateZYX(Vector3 angle)
{
    Matrix result = { 0 };

    float cz = cosf(angle.z);
    float sz = sinf(angle.z);
    float cy = cosf(angle.y);
    float sy = sinf(angle.y);
    float cx = cosf(angle.x);
    float sx = sinf(angle.x);

    result.m0 = cz * cy;
    result.m4 = cz * sy * sx - cx * sz;
    result.m8 = sz * sx + cz * cx * sy;
    result.m12 = 0;

    result.m1 = cy * sz;
    result.m5 = cz * cx + sz * sy * sx;
    result.m9 = cx * sz * sy - cz * sx;
    result.m13 = 0;

    result.m2 = -sy;
    result.m6 = cy * sx;
    result.m10 = cy * cx;
    result.m14 = 0;

    result.m3 = 0;
    result.m7 = 0;
    result.m11 = 0;
    result.m15 = 1;

    return result;
}

// Get scaling matrix
RMAPI Matrix Scale(float x, float y, float z)
{
    Matrix result = { x, 0.0f, 0.0f, 0.0f,
                      0.0f, y, 0.0f, 0.0f,
                      0.0f, 0.0f, z, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Get perspective projection matrix
RMAPI Matrix Frustum(double left, double right, double bottom, double top, double near, double far)
{
    Matrix result = { 0 };

    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(far - near);

    result.m0 = ((float)near * 2.0f) / rl;
    result.m1 = 0.0f;
    result.m2 = 0.0f;
    result.m3 = 0.0f;

    result.m4 = 0.0f;
    result.m5 = ((float)near * 2.0f) / tb;
    result.m6 = 0.0f;
    result.m7 = 0.0f;

    result.m8 = ((float)right + (float)left) / rl;
    result.m9 = ((float)top + (float)bottom) / tb;
    result.m10 = -((float)far + (float)near) / fn;
    result.m11 = -1.0f;

    result.m12 = 0.0f;
    result.m13 = 0.0f;
    result.m14 = -((float)far * (float)near * 2.0f) / fn;
    result.m15 = 0.0f;

    return result;
}

// Get perspective projection matrix
// NOTE: Fovy angle must be provided in radians
RMAPI Matrix Perspective(double fovy, double aspect, double near, double far)
{
    Matrix result = { 0 };

    double top = near * tan(fovy * 0.5);
    double bottom = -top;
    double right = top * aspect;
    double left = -right;

    // MatrixFrustum(-right, right, -top, top, near, far);
    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(far - near);

    result.m0 = ((float)near * 2.0f) / rl;
    result.m5 = ((float)near * 2.0f) / tb;
    result.m8 = ((float)right + (float)left) / rl;
    result.m9 = ((float)top + (float)bottom) / tb;
    result.m10 = -((float)far + (float)near) / fn;
    result.m11 = -1.0f;
    result.m14 = -((float)far * (float)near * 2.0f) / fn;

    return result;
}

// Get orthographic projection matrix
RMAPI Matrix Ortho(double left, double right, double bottom, double top, double near, double far)
{
    Matrix result = { 0 };

    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float fn = (float)(far - near);

    result.m0 = 2.0f / rl;
    result.m1 = 0.0f;
    result.m2 = 0.0f;
    result.m3 = 0.0f;
    result.m4 = 0.0f;
    result.m5 = 2.0f / tb;
    result.m6 = 0.0f;
    result.m7 = 0.0f;
    result.m8 = 0.0f;
    result.m9 = 0.0f;
    result.m10 = -2.0f / fn;
    result.m11 = 0.0f;
    result.m12 = -((float)left + (float)right) / rl;
    result.m13 = -((float)top + (float)bottom) / tb;
    result.m14 = -((float)far + (float)near) / fn;
    result.m15 = 1.0f;

    return result;
}

// Get camera look-at matrix (view matrix)
RMAPI Matrix LookAt(Vector3 eye, Vector3 target, Vector3 up)
{
    Matrix result = { 0 };

    float length = 0.0f;
    float ilength = 0.0f;

    // Vector3Subtract(eye, target)
    Vector3 vz = { eye.x - target.x, eye.y - target.y, eye.z - target.z };

    // Vector3Normalize(vz)
    Vector3 v = vz;
    length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f / length;
    vz.x *= ilength;
    vz.y *= ilength;
    vz.z *= ilength;

    // Vector3CrossProduct(up, vz)
    Vector3 vx = { up.y * vz.z - up.z * vz.y, up.z * vz.x - up.x * vz.z, up.x * vz.y - up.y * vz.x };

    // Vector3Normalize(x)
    v = vx;
    length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0.0f) length = 1.0f;
    ilength = 1.0f / length;
    vx.x *= ilength;
    vx.y *= ilength;
    vx.z *= ilength;

    // Vector3CrossProduct(vz, vx)
    Vector3 vy = { vz.y * vx.z - vz.z * vx.y, vz.z * vx.x - vz.x * vx.z, vz.x * vx.y - vz.y * vx.x };

    result.m0 = vx.x;
    result.m1 = vy.x;
    result.m2 = vz.x;
    result.m3 = 0.0f;
    result.m4 = vx.y;
    result.m5 = vy.y;
    result.m6 = vz.y;
    result.m7 = 0.0f;
    result.m8 = vx.z;
    result.m9 = vy.z;
    result.m10 = vz.z;
    result.m11 = 0.0f;
    result.m12 = -(vx.x * eye.x + vx.y * eye.y + vx.z * eye.z);   // Vector3DotProduct(vx, eye)
    result.m13 = -(vy.x * eye.x + vy.y * eye.y + vy.z * eye.z);   // Vector3DotProduct(vy, eye)
    result.m14 = -(vz.x * eye.x + vz.y * eye.y + vz.z * eye.z);   // Vector3DotProduct(vz, eye)
    result.m15 = 1.0f;

    return result;
}

// Extract rotation from world matrix
inline Matrix NormalMatrix(Matrix world)
{
    Matrix normal = world;
    normal.m12 = normal.m13 = normal.m14 = 0.0f;
    normal = Transpose(Invert(normal));
    return normal;
}

// Convert from object-space to normalized-device-coordinates
inline Vector3 Clip(Matrix m, Vector3 v)
{
    Vector4 clip;
    clip.x = v.x;
    clip.y = v.y;
    clip.z = v.z;
    clip.w = 1.0f;

    clip = m * clip;
    clip /= clip.w;

    return { clip.x, clip.y, clip.z };
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Quaternion math
//----------------------------------------------------------------------------------

// Add two quaternions
RMAPI Quaternion Add(Quaternion q1, Quaternion q2)
{
    Quaternion result = { q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w };

    return result;
}

// Add quaternion and float value
RMAPI Quaternion Add(Quaternion q, float add)
{
    Quaternion result = { q.x + add, q.y + add, q.z + add, q.w + add };

    return result;
}

// Subtract two quaternions
RMAPI Quaternion Subtract(Quaternion q1, Quaternion q2)
{
    Quaternion result = { q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w };

    return result;
}

// Subtract quaternion and float value
RMAPI Quaternion Subtract(Quaternion q, float sub)
{
    Quaternion result = { q.x - sub, q.y - sub, q.z - sub, q.w - sub };

    return result;
}

// Get identity quaternion
RMAPI Quaternion QuaternionIdentity(void)
{
    Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };

    return result;
}

// Computes the length of a quaternion
RMAPI float Length(Quaternion q)
{
    float result = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);

    return result;
}

// Normalize provided quaternion
RMAPI Quaternion Normalize(Quaternion q)
{
    Quaternion result = { 0 };

    float length = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f / length;

    result.x = q.x * ilength;
    result.y = q.y * ilength;
    result.z = q.z * ilength;
    result.w = q.w * ilength;

    return result;
}

// Invert provided quaternion
RMAPI Quaternion Invert(Quaternion q)
{
    Quaternion result = q;

    float lengthSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;

    if (lengthSq != 0.0f)
    {
        float invLength = 1.0f / lengthSq;

        result.x *= -invLength;
        result.y *= -invLength;
        result.z *= -invLength;
        result.w *= invLength;
    }

    return result;
}

// Calculate two quaternion multiplication
RMAPI Quaternion Multiply(Quaternion q1, Quaternion q2)
{
    Quaternion result = { 0 };

    float qax = q1.x, qay = q1.y, qaz = q1.z, qaw = q1.w;
    float qbx = q2.x, qby = q2.y, qbz = q2.z, qbw = q2.w;

    result.x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
    result.y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
    result.z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
    result.w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

    return result;
}

// Scale quaternion by float value
RMAPI Quaternion Scale(Quaternion q, float mul)
{
    Quaternion result = { 0 };

    result.x = q.x * mul;
    result.y = q.y * mul;
    result.z = q.z * mul;
    result.w = q.w * mul;

    return result;
}

// Divide two quaternions
RMAPI Quaternion Divide(Quaternion q1, Quaternion q2)
{
    Quaternion result = { q1.x / q2.x, q1.y / q2.y, q1.z / q2.z, q1.w / q2.w };

    return result;
}

// Calculate linear interpolation between two quaternions
RMAPI Quaternion Lerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = { 0 };

    result.x = q1.x + amount * (q2.x - q1.x);
    result.y = q1.y + amount * (q2.y - q1.y);
    result.z = q1.z + amount * (q2.z - q1.z);
    result.w = q1.w + amount * (q2.w - q1.w);

    return result;
}

// Calculate slerp-optimized interpolation between two quaternions
RMAPI Quaternion Nlerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = { 0 };

    // QuaternionLerp(q1, q2, amount)
    result.x = q1.x + amount * (q2.x - q1.x);
    result.y = q1.y + amount * (q2.y - q1.y);
    result.z = q1.z + amount * (q2.z - q1.z);
    result.w = q1.w + amount * (q2.w - q1.w);

    // QuaternionNormalize(q);
    Quaternion q = result;
    float length = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f / length;

    result.x = q.x * ilength;
    result.y = q.y * ilength;
    result.z = q.z * ilength;
    result.w = q.w * ilength;

    return result;
}

// Calculates spherical linear interpolation between two quaternions
RMAPI Quaternion Slerp(Quaternion q1, Quaternion q2, float amount)
{
    Quaternion result = { 0 };

    float cosHalfTheta = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

    if (cosHalfTheta < 0)
    {
        q2.x = -q2.x; q2.y = -q2.y; q2.z = -q2.z; q2.w = -q2.w;
        cosHalfTheta = -cosHalfTheta;
    }

    if (fabsf(cosHalfTheta) >= 1.0f) result = q1;
    else if (cosHalfTheta > 0.95f) result = Nlerp(q1, q2, amount);
    else
    {
        float halfTheta = acosf(cosHalfTheta);
        float sinHalfTheta = sqrtf(1.0f - cosHalfTheta * cosHalfTheta);

        if (fabsf(sinHalfTheta) < 0.001f)
        {
            result.x = (q1.x * 0.5f + q2.x * 0.5f);
            result.y = (q1.y * 0.5f + q2.y * 0.5f);
            result.z = (q1.z * 0.5f + q2.z * 0.5f);
            result.w = (q1.w * 0.5f + q2.w * 0.5f);
        }
        else
        {
            float ratioA = sinf((1 - amount) * halfTheta) / sinHalfTheta;
            float ratioB = sinf(amount * halfTheta) / sinHalfTheta;

            result.x = (q1.x * ratioA + q2.x * ratioB);
            result.y = (q1.y * ratioA + q2.y * ratioB);
            result.z = (q1.z * ratioA + q2.z * ratioB);
            result.w = (q1.w * ratioA + q2.w * ratioB);
        }
    }

    return result;
}

// Calculate quaternion based on the rotation from one vector to another
RMAPI Quaternion FromTo(Vector3 from, Vector3 to)
{
    Quaternion result = { 0 };

    float cos2Theta = (from.x * to.x + from.y * to.y + from.z * to.z);    // Vector3DotProduct(from, to)
    Vector3 cross = { from.y * to.z - from.z * to.y, from.z * to.x - from.x * to.z, from.x * to.y - from.y * to.x }; // Vector3CrossProduct(from, to)

    result.x = cross.x;
    result.y = cross.y;
    result.z = cross.z;
    result.w = 1.0f + cos2Theta;

    // QuaternionNormalize(q);
    // NOTE: Normalize to essentially nlerp the original and identity to 0.5
    Quaternion q = result;
    float length = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    if (length == 0.0f) length = 1.0f;
    float ilength = 1.0f / length;

    result.x = q.x * ilength;
    result.y = q.y * ilength;
    result.z = q.z * ilength;
    result.w = q.w * ilength;

    return result;
}

// Get a quaternion for a given rotation matrix
RMAPI Quaternion FromMatrix(Matrix mat)
{
    Quaternion result = { 0 };

    float fourWSquaredMinus1 = mat.m0 + mat.m5 + mat.m10;
    float fourXSquaredMinus1 = mat.m0 - mat.m5 - mat.m10;
    float fourYSquaredMinus1 = mat.m5 - mat.m0 - mat.m10;
    float fourZSquaredMinus1 = mat.m10 - mat.m0 - mat.m5;

    int biggestIndex = 0;
    float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
    if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourXSquaredMinus1;
        biggestIndex = 1;
    }

    if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourYSquaredMinus1;
        biggestIndex = 2;
    }

    if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
    {
        fourBiggestSquaredMinus1 = fourZSquaredMinus1;
        biggestIndex = 3;
    }

    float biggestVal = sqrtf(fourBiggestSquaredMinus1 + 1.0f) * 0.5f;
    float mult = 0.25f / biggestVal;

    switch (biggestIndex)
    {
    case 0:
        result.w = biggestVal;
        result.x = (mat.m6 - mat.m9) * mult;
        result.y = (mat.m8 - mat.m2) * mult;
        result.z = (mat.m1 - mat.m4) * mult;
        break;
    case 1:
        result.x = biggestVal;
        result.w = (mat.m6 - mat.m9) * mult;
        result.y = (mat.m1 + mat.m4) * mult;
        result.z = (mat.m8 + mat.m2) * mult;
        break;
    case 2:
        result.y = biggestVal;
        result.w = (mat.m8 - mat.m2) * mult;
        result.x = (mat.m1 + mat.m4) * mult;
        result.z = (mat.m6 + mat.m9) * mult;
        break;
    case 3:
        result.z = biggestVal;
        result.w = (mat.m1 - mat.m4) * mult;
        result.x = (mat.m8 + mat.m2) * mult;
        result.y = (mat.m6 + mat.m9) * mult;
        break;
    }

    return result;
}

// Get a matrix for a given quaternion
RMAPI Matrix ToMatrix(Quaternion q)
{
    Matrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f }; // MatrixIdentity()

    float a2 = q.x * q.x;
    float b2 = q.y * q.y;
    float c2 = q.z * q.z;
    float ac = q.x * q.z;
    float ab = q.x * q.y;
    float bc = q.y * q.z;
    float ad = q.w * q.x;
    float bd = q.w * q.y;
    float cd = q.w * q.z;

    result.m0 = 1 - 2 * (b2 + c2);
    result.m1 = 2 * (ab + cd);
    result.m2 = 2 * (ac - bd);

    result.m4 = 2 * (ab - cd);
    result.m5 = 1 - 2 * (a2 + c2);
    result.m6 = 2 * (bc + ad);

    result.m8 = 2 * (ac + bd);
    result.m9 = 2 * (bc - ad);
    result.m10 = 1 - 2 * (a2 + b2);

    return result;
}

// Get rotation quaternion for an angle and axis
// NOTE: Angle must be provided in radians
RMAPI Quaternion FromAxisAngle(Vector3 axis, float angle)
{
    Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };

    float axisLength = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

    if (axisLength != 0.0f)
    {
        angle *= 0.5f;

        float length = 0.0f;
        float ilength = 0.0f;

        // Vector3Normalize(axis)
        Vector3 v = axis;
        length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
        if (length == 0.0f) length = 1.0f;
        ilength = 1.0f / length;
        axis.x *= ilength;
        axis.y *= ilength;
        axis.z *= ilength;

        float sinres = sinf(angle);
        float cosres = cosf(angle);

        result.x = axis.x * sinres;
        result.y = axis.y * sinres;
        result.z = axis.z * sinres;
        result.w = cosres;

        // QuaternionNormalize(q);
        Quaternion q = result;
        length = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
        if (length == 0.0f) length = 1.0f;
        ilength = 1.0f / length;
        result.x = q.x * ilength;
        result.y = q.y * ilength;
        result.z = q.z * ilength;
        result.w = q.w * ilength;
    }

    return result;
}

// Get the rotation angle and axis for a given quaternion
RMAPI void ToAxisAngle(Quaternion q, Vector3* outAxis, float* outAngle)
{
    if (fabsf(q.w) > 1.0f)
    {
        // QuaternionNormalize(q);
        float length = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
        if (length == 0.0f) length = 1.0f;
        float ilength = 1.0f / length;

        q.x = q.x * ilength;
        q.y = q.y * ilength;
        q.z = q.z * ilength;
        q.w = q.w * ilength;
    }

    Vector3 resAxis = { 0.0f, 0.0f, 0.0f };
    float resAngle = 2.0f * acosf(q.w);
    float den = sqrtf(1.0f - q.w * q.w);

    if (den > 0.0001f)
    {
        resAxis.x = q.x / den;
        resAxis.y = q.y / den;
        resAxis.z = q.z / den;
    }
    else
    {
        // This occurs when the angle is zero.
        // Not a problem: just set an arbitrary normalized axis.
        resAxis.x = 1.0f;
    }

    *outAxis = resAxis;
    *outAngle = resAngle;
}

// Get the quaternion equivalent to Euler angles
// NOTE: Rotation order is ZYX
RMAPI Quaternion FromEuler(float pitch, float yaw, float roll)
{
    Quaternion result = { 0 };

    float x0 = cosf(pitch * 0.5f);
    float x1 = sinf(pitch * 0.5f);
    float y0 = cosf(yaw * 0.5f);
    float y1 = sinf(yaw * 0.5f);
    float z0 = cosf(roll * 0.5f);
    float z1 = sinf(roll * 0.5f);

    result.x = x1 * y0 * z0 - x0 * y1 * z1;
    result.y = x0 * y1 * z0 + x1 * y0 * z1;
    result.z = x0 * y0 * z1 - x1 * y1 * z0;
    result.w = x0 * y0 * z0 + x1 * y1 * z1;

    return result;
}

// Get the Euler angles equivalent to quaternion (roll, pitch, yaw)
// NOTE: Angles are returned in a Vector3 struct in radians
RMAPI Vector3 ToEuler(Quaternion q)
{
    Vector3 result = { 0 };

    // Roll (x-axis rotation)
    float x0 = 2.0f * (q.w * q.x + q.y * q.z);
    float x1 = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    result.x = atan2f(x0, x1);

    // Pitch (y-axis rotation)
    float y0 = 2.0f * (q.w * q.y - q.z * q.x);
    y0 = y0 > 1.0f ? 1.0f : y0;
    y0 = y0 < -1.0f ? -1.0f : y0;
    result.y = asinf(y0);

    // Yaw (z-axis rotation)
    float z0 = 2.0f * (q.w * q.z + q.x * q.y);
    float z1 = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    result.z = atan2f(z0, z1);

    return result;
}

// Transform a quaternion given a transformation matrix
RMAPI Quaternion Multiply(Quaternion q, Matrix mat)
{
    Quaternion result = { 0 };

    result.x = mat.m0 * q.x + mat.m4 * q.y + mat.m8 * q.z + mat.m12 * q.w;
    result.y = mat.m1 * q.x + mat.m5 * q.y + mat.m9 * q.z + mat.m13 * q.w;
    result.z = mat.m2 * q.x + mat.m6 * q.y + mat.m10 * q.z + mat.m14 * q.w;
    result.w = mat.m3 * q.x + mat.m7 * q.y + mat.m11 * q.z + mat.m15 * q.w;

    return result;
}

// Check whether two given quaternions are almost equal
RMAPI int Equals(Quaternion p, Quaternion q)
{
    int result = (((fabsf(p.x - q.x)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
        ((fabsf(p.y - q.y)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
        ((fabsf(p.z - q.z)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z))))) &&
        ((fabsf(p.w - q.w)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.w), fabsf(q.w)))))) ||
        (((fabsf(p.x + q.x)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.x), fabsf(q.x))))) &&
            ((fabsf(p.y + q.y)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.y), fabsf(q.y))))) &&
            ((fabsf(p.z + q.z)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.z), fabsf(q.z))))) &&
            ((fabsf(p.w + q.w)) <= (EPSILON * fmaxf(1.0f, fmaxf(fabsf(p.w), fabsf(q.w))))));

    return result;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Vector & Matrix helpers
//----------------------------------------------------------------------------------

RMAPI Vector3 Forward(Matrix m)
{
    return { m.m8, m.m9, m.m10 };
}

RMAPI Vector3 Right(Matrix m)
{
    return { m.m0, m.m1, m.m2 };
}

RMAPI Vector3 Up(Matrix m)
{
    return { m.m4, m.m5, m.m6 };
}

RMAPI Vector3 Translation(Matrix m)
{
    return { m.m12, m.m13, m.m14 };
}

RMAPI Matrix Translate(Vector3 v)
{
    return Translate(v.x, v.y, v.z);
}

RMAPI Quaternion FromEuler(Vector3 v)
{
    return FromEuler(v.x, v.y, v.z);
}

RMAPI Matrix Scale(Vector3 v)
{
    return Scale(v.x, v.y, v.z);
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Global operator overloads
//----------------------------------------------------------------------------------

RMAPI Vector2 operator+(Vector2 a, Vector2 b)
{
    return Add(a, b);
}

RMAPI Vector2 operator-(Vector2 a, Vector2 b)
{
    return Subtract(a, b);
}

RMAPI Vector2 operator*(Vector2 a, Vector2 b)
{
    return Multiply(a, b);
}

RMAPI Vector2 operator/(Vector2 a, Vector2 b)
{
    return Divide(a, b);
}

RMAPI Vector2 operator+(Vector2 a, float b)
{
    return Add(a, b);
}

RMAPI Vector2 operator-(Vector2 a, float b)
{
    return Subtract(a, b);
}

RMAPI Vector2 operator*(Vector2 a, float b)
{
    return Scale(a, b);
}

RMAPI Vector2 operator/(Vector2 a, float b)
{
    return Scale(a, 1.0f / b);
}

RMAPI Vector3 operator+(Vector3 a, Vector3 b)
{
    return Add(a, b);
}

RMAPI Vector3 operator-(Vector3 a, Vector3 b)
{
    return Subtract(a, b);
}

RMAPI Vector3 operator*(Vector3 a, Vector3 b)
{
    return Multiply(a, b);
}

RMAPI Vector3 operator/(Vector3 a, Vector3 b)
{
    return Divide(a, b);
}

RMAPI Vector3 operator+(Vector3 a, float b)
{
    return Add(a, b);
}

RMAPI Vector3 operator-(Vector3 a, float b)
{
    return Subtract(a, b);
}

RMAPI Vector3 operator*(Vector3 a, float b)
{
    return Scale(a, b);
}

RMAPI Vector3 operator/(Vector3 a, float b)
{
    return Scale(a, 1.0f / b);
}

RMAPI Vector4 operator+(Vector4 a,  Vector4 b)
{
    return Add(a, b);
}

RMAPI Vector4 operator-(Vector4 a,  Vector4 b)
{
    return Subtract(a, b);
}

RMAPI Vector4 operator*(Vector4 a,  Vector4 b)
{
    return Multiply(a, b);
}

RMAPI Vector4 operator/(Vector4 a,  Vector4 b)
{
    return Divide(a, b);
}

RMAPI Vector4 operator+(Vector4 a, float b)
{
    return Add(a, b);
}

RMAPI Vector4 operator-(Vector4 a, float b)
{
    return Subtract(a, b);
}

RMAPI Vector4 operator*(Vector4 a, float b)
{
    return Scale(a, b);
}

RMAPI Vector4 operator/(Vector4 a, float b)
{
    return Scale(a, 1.0f / b);
}

RMAPI Matrix operator+(Matrix a, Matrix b)
{
    return Add(a, b);
}

RMAPI Matrix operator-(Matrix a, Matrix b)
{
    return Subtract(a, b);
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Matrix multiplication overloads
//----------------------------------------------------------------------------------

RMAPI Matrix operator*(Matrix a, Matrix b)
{
    return Multiply(a, b);
}

RMAPI Vector4 operator*(Matrix m, Vector4 v)
{
    return Multiply(v, m);
}

RMAPI Vector3 operator*(Matrix m, Vector3 v)
{
    return Multiply(v, m);
}

RMAPI Vector2 operator*(Matrix m, Vector2 v)
{
    return Multiply(v, m);
}

RMAPI Vector3 operator*(Quaternion a, Vector3 b)
{
    // Not part of raylib but uses ToMatrix which is part of raylib
    return Multiply(b, ToMatrix(a));
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Member operator overloads
//----------------------------------------------------------------------------------

RMAPI Vector2 Vector2::operator+=(Vector2 v)
{
    x += v.x;
    y += v.y;
    return *this;
}

RMAPI Vector2 Vector2::operator-=(Vector2 v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

RMAPI Vector2 Vector2::operator*=(Vector2 v)
{
    x *= v.x;
    y *= v.y;
    return *this;
}

RMAPI Vector2 Vector2::operator/=(Vector2 v)
{
    x /= v.x;
    y /= v.y;
    return *this;
}

RMAPI Vector2 Vector2::operator+=(float f)
{
    x += f;
    y += f;
    return *this;
}

RMAPI Vector2 Vector2::operator-=(float f)
{
    x -= f;
    y -= f;
    return *this;
}

RMAPI Vector2 Vector2::operator*=(float f)
{
    x *= f;
    y *= f;
    return *this;
}

RMAPI Vector2 Vector2::operator/=(float f)
{
    x /= f;
    y /= f;
    return *this;
}

RMAPI Vector3 Vector3::operator+=(Vector3 v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

RMAPI Vector3 Vector3::operator-=(Vector3 v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

RMAPI Vector3 Vector3::operator*=(Vector3 v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}

RMAPI Vector3 Vector3::operator/=(Vector3 v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}

RMAPI Vector3 Vector3::operator+=(float f)
{
    x += f;
    y += f;
    z += f;
    return *this;
}

RMAPI Vector3 Vector3::operator-=(float f)
{
    x -= f;
    y -= f;
    z -= f;
    return *this;
}

RMAPI Vector3 Vector3::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

RMAPI Vector3 Vector3::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

RMAPI Vector4 Vector4::operator+=(Vector4 v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

RMAPI Vector4 Vector4::operator-=(Vector4 v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}

RMAPI Vector4 Vector4::operator*=(Vector4 v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}

RMAPI Vector4 Vector4::operator/=(Vector4 v)
{
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}

RMAPI Vector4 Vector4::operator+=(float f)
{
    x += f;
    y += f;
    z += f;
    w += f;
    return *this;
}

RMAPI Vector4 Vector4::operator-=(float f)
{
    x -= f;
    y -= f;
    z -= f;
    w -= f;
    return *this;
}

RMAPI Vector4 Vector4::operator*=(float f)
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
    return *this;
}

RMAPI Vector4 Vector4::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    w /= f;
    return *this;
}

RMAPI Vector2::operator Vector3() const
{
    return { x, y, 0.0f };
}

RMAPI Vector3::operator Vector2() const
{
    return { x, y };
}

RMAPI Vector3::operator Vector4() const
{
    return { x, y, z, 1.0f };
}

RMAPI Vector4::operator Vector3() const
{
    return { x, y, z };
}
