#include "MathUtil.h"
#include "Vector3.h"
#include "Vector2.h"

void MathUtil::smooth(float* x, float target, float elapsedTime, float responseTime)
{
    ASSERT(x);

    if (elapsedTime > 0)
    {
        *x += (target - *x) * elapsedTime / (elapsedTime + responseTime);
    }
}

void MathUtil::smooth(float* x, float target, float elapsedTime, float riseTime, float fallTime)
{
    ASSERT(x);
    
    if (elapsedTime > 0)
    {
        float delta = target - *x;
        *x += delta * elapsedTime / (elapsedTime + (delta > 0 ? riseTime : fallTime));
    }
}


bool MathUtil::intersectTriangle( const Vector3& orig, const Vector3& dir,
                                 Vector3& v0, Vector3& v1, Vector3& v2,
                                 float* t, float* u, float* v )
{

    // Find vectors for two edges sharing vert0
    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    // Begin calculating determinant - also used to calculate U parameter
    Vector3 pvec;
    Vector3::cross(dir, edge2 ,&pvec);

    // If determinant is near zero, ray lies in plane of triangle
    float det = Vector3::dot(edge1, pvec);

    Vector3 tvec;
    if( det > 0 )
    {
        tvec = orig - v0;
    }
    else
    {
        tvec = v0 - orig;
        det = -det;
    }

    if( det < 0.0001f )
        return false;

    // Calculate U parameter and test bounds
    *u = Vector3::dot( tvec, pvec );
    if( *u < 0.0f || *u > det )
        return false;

    // Prepare to test V parameter
    Vector3 qvec;
    Vector3::cross(tvec, edge1,&qvec);

    // Calculate V parameter and test bounds
    *v = Vector3::dot( dir, qvec);
    if( *v < 0.0f || *u + *v > det)
        return false;

    // Calculate t, scale parameters, ray intersects triangle
    *t = Vector3::dot(edge2, qvec);
    float fInvDet = 1.0f / det;
    *t *= fInvDet;
    *u *= fInvDet;
    *v *= fInvDet;

    return true;
}
