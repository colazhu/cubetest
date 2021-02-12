#ifndef MATHUTIL_H_
#define MATHUTIL_H_

#include "Base.h"


class Vector3;
/**
 * Defines a math utility class.
 *
 * This is primarily used for optimized internal math operations.
 */
class MathUtil
{
    friend class Matrix;
    friend class Vector3;

public:

    /**
     * Updates the given scalar towards the given target using a smoothing function.
     * The given response time determines the amount of smoothing (lag). A longer
     * response time yields a smoother result and more lag. To force the scalar to
     * follow the target closely, provide a response time that is very small relative
     * to the given elapsed time.
     *
     * @param x the scalar to update.
     * @param target target value.
     * @param elapsedTime elapsed time between calls.
     * @param responseTime response time (in the same units as elapsedTime).
     */
    static void smooth(float* x, float target, float elapsedTime, float responseTime);

    /**
     * Updates the given scalar towards the given target using a smoothing function.
     * The given rise and fall times determine the amount of smoothing (lag). Longer
     * rise and fall times yield a smoother result and more lag. To force the scalar to
     * follow the target closely, provide rise and fall times that are very small relative
     * to the given elapsed time.
     *
     * @param x the scalar to update.
     * @param target target value.
     * @param elapsedTime elapsed time between calls.
     * @param riseTime response time for rising slope (in the same units as elapsedTime).
     * @param fallTime response time for falling slope (in the same units as elapsedTime).
     */
    static void smooth(float* x, float target, float elapsedTime, float riseTime, float fallTime);

    static bool intersectTriangle( const Vector3& orig, const Vector3& dir,
                                     Vector3& v0, Vector3& v1, Vector3& v2,
                                     float* t, float* u, float* v);
private:

    inline static void addMatrix(const float* m, float scalar, float* dst);

    inline static void addMatrix(const float* m1, const float* m2, float* dst);

    inline static void subtractMatrix(const float* m1, const float* m2, float* dst);

    inline static void multiplyMatrix(const float* m, float scalar, float* dst);

    inline static void multiplyMatrix(const float* m1, const float* m2, float* dst);

    inline static void negateMatrix(const float* m, float* dst);

    inline static void transposeMatrix(const float* m, float* dst);

    inline static void transformVector4(const float* m, float x, float y, float z, float w, float* dst);

    inline static void transformVector4(const float* m, const float* v, float* dst);

    inline static void crossVector3(const float* v1, const float* v2, float* dst);

    MathUtil();
};

// Math
#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)
#define MATH_RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f)      // Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.
#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f
#define MATH_E                      2.71828182845904523536f
#define MATH_LOG10E                 0.4342944819032518f
#define MATH_LOG2E                  1.442695040888963387f
#define MATH_PI                     3.14159265358979323846f
#define MATH_PIOVER2                1.57079632679489661923f
#define MATH_PIOVER4                0.785398163397448309616f
#define MATH_PIX2                   6.28318530717958647693f
#define MATH_EPSILON                0.000001f
#define MATH_CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))
#define MATH_MIN_FLOAT              0.00001f
#define MATH_MIN_DOUBLE             0.000000000001

#ifndef M_1_PI
#define M_1_PI                      0.31830988618379067154
#endif

inline bool RealEqual(float v1, float v2)
{
    return v1 -v2 <= MATH_MIN_FLOAT  && v1 -v2 >= -MATH_MIN_FLOAT;
}

inline bool RealEqual(double v1, double v2)
{
    return v1 -v2 <= MATH_MIN_DOUBLE && v1 -v2 >= -MATH_MIN_DOUBLE;
}

inline bool RealGreater(float v1, float v2)
{
    return !RealEqual(v1, v2) && v1 > v2;
}

inline bool RealGreater(double v1, double v2)
{
    return !RealEqual(v1, v2) && v1 > v2;
}

inline bool RealLess(float v1, float v2)
{
    return !RealEqual(v1, v2) && v1 < v2;
}

inline bool RealLess(double v1, double v2)
{
    return !RealEqual(v1, v2) && v1 < v2;
}

inline bool RealEqualGreater(float v1, float v2)
{
    return RealEqual(v1, v2) || v1 > v2;
}

inline bool RealEqualGreater(double v1, double v2)
{
    return RealEqual(v1, v2) || v1 > v2;
}

inline bool RealEqualLess(float v1, float v2)
{
    return RealEqual(v1, v2) || v1 < v2;
}

inline bool RealEqualLess(double v1, double v2)
{
    return RealEqual(v1, v2) || v1 < v2;
}

template <typename T> inline T MATH_ABS(const T &t) { return t >= 0 ? t : -t; }

template <typename T>
inline T MATH_MAX(const T& t1, const T& t2)
{
    if (t1 > t2) {
        return t1;
    }
    return t2;
}

template <>
inline float MATH_MAX<float> (const float& v1, const float& v2)
{
    if (RealGreater(v1, v2)) {
        return v1;
    }
    return v2;
}

template <>
inline double MATH_MAX<double> (const double& v1, const double& v2)
{
    if (RealGreater(v1, v2)) {
        return v1;
    }
    return v2;
}

template <typename T>
inline T MATH_MIN(const T& t1, const T& t2)
{
    if (t1 < t2) {
        return t1;
    }
    return t2;
}

template <>
inline float MATH_MIN<float> (const float& v1, const float& v2)
{
    if (RealGreater(v2, v1)) {
        return v1;
    }
    return v2;
}

template <>
inline double MATH_MIN<double> (const double& v1, const double& v2)
{
    if (RealGreater(v2, v1)) {
        return v1;
    }
    return v2;
}

inline bool MATH_IsEqual(float p1, float p2)
{
    return (MATH_ABS(p1 - p2) <= 0.00001f * std::min(MATH_ABS(p1), MATH_ABS(p2)));
}

template <typename T>
inline T MATH_MAX(const T& t1, const T& t2, const T& t3)
{
    return MATH_MAX(MATH_MAX(t1, t2), t3);
}

template <typename T>
inline T MATH_MIN(const T &t1, const T &t2, const T& t3)
{
    return MATH_MIN(MATH_MIN(t1, t2), t3);
}

template <typename T>
inline T MATH_Bound(const T &min, const T &val, const T &max)
{
    return MATH_MAX(min, MATH_MIN(max, val));
}

inline int MATH_ROUND(double d)
{
    return d >= 0.0 ? int(d + 0.5) : int(d -  0.5);
}
inline int MATH_ROUND(float d)
{
    return d >= 0.0f ? int(d + 0.5) : int(d -  0.5);
}

template<typename T, typename G>
inline  T MATH_ROUND(G d)
{
    return d >= 0.0? T(d + 0.5) : T(d - 0.5);
}

template<typename T, typename G>
inline T MATH_FLOAT2INT(G d)
{
    return d + MATH_MIN_FLOAT;
}

template<typename T>
inline T MATH_FLOAT2INT(double d)
{
    return d + MATH_MIN_DOUBLE;
}

#define MATRIX_SIZE ( sizeof(float) * 16)

#include "MathUtil.inl"

#endif
