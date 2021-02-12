#include "Arcball.h"
#include "Log.h"
Arcball::Arcball(float w, float h, float radius)
    : m_mode(MODE_FREE),
      m_dragged(false),
      m_radius(1.0f),
      m_prepoint(Vector3(0, 0, 0)),
      m_curpoint(Vector3(0, 0, 0)),
      m_oldpoint(Vector3(0, 0, 0)),
      m_prequaternion(Quaternion(0, 0, 0, 1)),
      m_curquaternion(Quaternion(0, 0, 0, 1)),
      m_rotation_increament(Quaternion(0, 0, 0, 1)),
      m_pretime(0),
      m_curtime(0)
{
    m_rotatematrix = Matrix::identity();
    setWindow(w, h) ;
}

Arcball::~Arcball()
{
}

void Arcball::reset(const Quaternion& q)
{
    m_prequaternion = q;
    m_curquaternion = q;
    m_rotation_increament.identity();
    Matrix::createRotation(q, &m_rotatematrix);
    m_dragged = false;
    m_radius = 1.0f;
    m_pretime = m_curtime = 0;
}

void Arcball::onBegin(float mouse_x, float mouse_y)
{
    // enter drag state only if user click the window's client area
    if(mouse_x >= 0 && mouse_x <= m_winwidth
       && mouse_y >= 0 && mouse_y < m_winheight)
    {
        m_dragged = true ; // begin drag state
        m_prequaternion = m_curquaternion ;
        m_prepoint = screenToVector(mouse_x, mouse_y) ;
        m_oldpoint = m_prepoint ;
        m_pretime = m_curtime = Log::tickCount();
    }
}

void Arcball::onMove(float mouse_x, float mouse_y)
{
    if(m_dragged)
    {
        m_curpoint = screenToVector(mouse_x, mouse_y) ;
        m_rotation_increament = quatFromBallPoints(m_oldpoint, m_curpoint );
        m_curquaternion = m_prequaternion * quatFromBallPoints( m_prepoint, m_curpoint ) ;
        m_oldpoint = m_curpoint;
        m_pretime = m_curtime;
        m_curtime = Log::tickCount();
    }
}

void Arcball::onEnd()
{
    m_dragged = false ;
}

void Arcball::setWindow(float w, float h, float arcball_radius)
{
     m_winwidth  = w;
     m_winheight = h;
     m_radius   = arcball_radius;
}

Matrix Arcball::getRotationMatrix()
{
    Matrix::createRotation(m_curquaternion,&m_rotatematrix);
    return m_rotatematrix;
}

Quaternion Arcball::getRotationQuatIncreament()
{
    return m_rotation_increament ;
}

int Arcball::getTimeIncreament()
{
    return m_curtime - m_pretime;
}

Quaternion Arcball::quatFromBallPoints(Vector3& start_point, Vector3& end_point)
{
    // Calculate rotate angle
    float angle = Vector3::dot(start_point,end_point);

    // Calculate rotate axis
    Vector3 axis;
    switch (m_mode) {
    case MODE_HORIZONTAL:
        Vector3::cross(Vector3(start_point.x,0,start_point.z), Vector3(end_point.x,0,end_point.z),&axis);
        break;
    case MODE_VERTICAL:
        Vector3::cross(Vector3(0,start_point.y,start_point.z),Vector3(0,end_point.y,end_point.z),&axis);
        break;
    case MODE_HORIZONTAL_AND_VERTICAL:
        if(Vector3::angle(Vector3(start_point.x,0,start_point.z),Vector3(end_point.x,0,end_point.z)) > Vector3::angle(Vector3(0,start_point.y,start_point.z),Vector3(0,end_point.y,end_point.z))) {
            Vector3::cross(Vector3(start_point.x,0,start_point.z), Vector3(end_point.x,0,end_point.z),&axis);
        } else {
            Vector3::cross(Vector3(0,start_point.y,start_point.z),Vector3(0,end_point.y,end_point.z),&axis);
        }
        break;
    case MODE_FREE:
    default:
        Vector3::cross(start_point, end_point, &axis);
        break;
    }

//    if(Vector3::angle(Vector3(start_point.x,0,start_point.z),Vector3(end_point.x,0,end_point.z)) > Vector3::angle(Vector3(0,start_point.y,start_point.z),Vector3(0,end_point.y,end_point.z))) {
//        Vector3::cross(Vector3(start_point.x,0,start_point.z), Vector3(end_point.x,0,end_point.z),&axis);
//    } else {
//        Vector3::cross(Vector3(0,start_point.y,start_point.z),Vector3(0,end_point.y,end_point.z),&axis);
//    }
//    // or
//    Vector3::cross( start_point, end_point,&axis);

    // Build and Normalize the Quaternion
    Quaternion quat(axis.x, axis.y, axis.z, angle);
    quat.normalize();
    return quat;
}

Vector3 Arcball::screenToVector(float screen_x, float screen_y)
{
    // Scale to screen
    float x = -(screen_x - m_winwidth / 2) / (m_radius * m_winwidth / 2);
    float y = (screen_y - m_winheight / 2) / (m_radius * m_winheight / 2);

    float z = 0.0f;
    float mag = x * x + y * y;

    if(mag > 1.0f)
    {
        float scale = 1.0f / sqrtf(mag);
        x *= scale;
        y *= scale;
    }
    else {
        z = sqrtf(1.0f - mag);
    }
    return Vector3(x, y, z);
}
