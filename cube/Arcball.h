#ifndef ARCBALL_H
#define ARCBALL_H

#include "MathInc.h"

enum ARCBALL_MODE {
    MODE_FREE,
    MODE_HORIZONTAL,
    MODE_VERTICAL,
    MODE_HORIZONTAL_AND_VERTICAL,
};

class Arcball
{
public:
    explicit Arcball(float w = 0.0f, float h = 0.0f,float radius = 1.0f);
    virtual ~Arcball();

    void setMode(enum ARCBALL_MODE mode) { m_mode = mode;}
    void reset(const Quaternion& q = Quaternion::identity()) ;
    void onBegin(float x, float y) ;
    void onMove(float x, float y) ;
    void onEnd();


    Quaternion quatFromBallPoints(Vector3& start_point, Vector3& end_point);
    Matrix getRotationMatrix() ;
    Quaternion getRotationQuatIncreament();
    int getTimeIncreament();
    void setWindow(float w, float h, float radius = 1.0f) ;
    Quaternion getRotationQuaternion() {return m_curquaternion;}
    void setPreQuaternion(const Quaternion& q) {m_prequaternion = q;}
private:
    ARCBALL_MODE m_mode;

    bool        m_dragged;              // whether the arc ball is dragged

    float		m_winwidth;             // arc ball's window width
    float		m_winheight;            // arc ball's window height
    float       m_radius;               // arc ball's radius in screen coordinates

    Quaternion	m_prequaternion;        // quaternion before mouse down
    Quaternion	m_curquaternion;        // current quaternion
    Quaternion	m_rotation_increament;	// rotation increment    

    Vector3		m_prepoint;             // starting point of arc ball rotate
    Vector3		m_curpoint;             // current point of arc ball rotate
    Vector3		m_oldpoint;             // old point
    Matrix		m_rotatematrix;			// rotation matrix

    unsigned long m_pretime;
    unsigned long m_curtime;
    // Convert scree point to arcball point(vector)
    Vector3 screenToVector(float screen_x, float screen_y) ;
};

#endif // ARCBALL_H
