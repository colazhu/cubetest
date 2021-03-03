#include "Camera.h"
#include "Log.h"

Camera* Camera::createCamera2D(const std::string& name,
                               const Rect& viewport,
                               float zNear,
                               float zFar)
{
    Camera* camera = new Camera(name);
    if (!camera || !camera->init(viewport, zNear, zFar)) {
        SAFE_DELETE(camera)
    }
    return camera;
}

Camera* Camera::createCamera3D(const std::string& name,
                               const Vector3& eye,
                                const Vector3& dir,
                                const Rect& viewport,
                                float fieldOfView,
                                float zNear,
                                float zFar)
{
    Camera* camera = new Camera(name);
    if (!camera || !camera->init(eye, dir, viewport, fieldOfView, zNear, zFar)) {
        SAFE_DELETE(camera)
    }
    return camera;
}

Camera::Camera(const std::string& name)
        : Node(name),
          m_cameraType(PROJECT_2D),
          m_fieldOfView(60.0f),
          m_dir(0, 0, -1),
          m_eye(0, 0, 0),
          m_up(0, 1, 0),
          m_viewport(),
          m_transformDirty(true),
          m_matPerspective(),
          m_matLookat(),
          m_matProjection(),
          m_step(1.0f)
{
    init(m_viewport);
}

bool Camera::init(const Rect& viewport,
                  float zNear,
                  float zFar)
{
    m_cameraType = PROJECT_2D;
    m_viewport = viewport;
    m_matProjection.setIdentity();
    Matrix::createOrthographic(m_viewport.size.width,  m_viewport.size.height,
            zNear,
            zFar,
            &m_matProjection);
    notifyCameraChanged();
    return true;
}

bool Camera::init(const Vector3& eye,
                    const Vector3& dir,
                    const Rect& viewport,
                    float fieldOfView,
                    float zNear,
                    float zFar)
{
    m_cameraType = PROJECT_3D;
    m_viewport = viewport;
    m_fieldOfView = fieldOfView;
    m_matPerspective.setIdentity();
    Matrix::createPerspective(fieldOfView,
                              (m_viewport.size.width / m_viewport.size.height),
                              zNear,
                              zFar,
                              &m_matPerspective);
    m_eye = eye;
    m_dir = dir;
    m_dir.normalize();
    notifyCameraChanged();
    updateTransform();
    return true;
}

void Camera::restore()
{
    m_dir = Vector3(0, 0, -1);
    m_eye = Vector3(0, 0, 0);
    m_up = Vector3(0, 1, 0);
    notifyCameraChanged();
}

void Camera::setEye(const Vector3& eye)
{
    if (eye == m_eye) {
        return;
    }
    m_eye = eye;
    notifyCameraChanged();
}

void Camera::setEye(float x, float y, float z)
{
    if (Vector3(x, y, z) == m_eye) {
        return;
    }
    m_eye = Vector3(x, y, z);
    notifyCameraChanged();
}

void Camera::setDirection(const Vector3 &dir)
{
    if (dir == m_dir) {
        return;
    }
    m_dir = dir;
    notifyCameraChanged();
}

void Camera::setDirection(float x, float y, float z)
{
    if (Vector3(x, y, z) == m_dir) {
        return;
    }
    m_dir = Vector3(x, y, z);
    notifyCameraChanged();
}

void Camera::setUp(const Vector3& up)
{
    if (up == m_up) {
        return;
    }
    m_up = up;
    notifyCameraChanged();
}

void Camera::setUp(float x, float y, float z)
{
    if (Vector3(x, y, z) == m_up) {
        return;
    }
    m_up = Vector3(x, y, z);
    notifyCameraChanged();
}

void Camera::moveLeft(float step)
{
    Vector3 dirLeft;
    Vector3::cross(m_up, m_dir, &dirLeft);
    dirLeft.normalize();
    m_eye += dirLeft * step;
    notifyCameraChanged();
}

void Camera::moveRight(float step)
{
    moveLeft(-step);
}

void Camera::moveForward(float step)
{
    m_eye += m_dir * step;
    notifyCameraChanged();
}

void Camera::moveBackward(float step)
{
    moveForward(-step);
}

void Camera::moveUp(float step)
{
    m_eye += m_up * step;
    notifyCameraChanged();
}

void Camera::moveDown(float step)
{
    moveUp(-step);
}

class CameraTransitionRoll : public IntervalAction
{
public:
    CameraTransitionRoll(Camera* node, const Vector3& dir, const Vector3& up, float radius) :
        IntervalAction(node),
        m_dir(dir),
        m_up(up),
        m_radius(radius)
    {}

    ~CameraTransitionRoll() {}

    virtual void onUpdate(Node* target, float dt, float time) {

        Vector3 curUp;
        Matrix matrixRoll;
        Matrix::createRotation(m_dir, m_radius*time, &matrixRoll);
        matrixRoll.transformVector(m_up, &curUp);
        curUp.normalize();

        Camera* camera = static_cast<Camera*>(target);
        camera->m_up = curUp;
        // LOG_BASE("onUpdate:%p %.3f %.3f, up:%.2f %.2f %.2f", target, dt, time, curUp.x, curUp.y, curUp.z);
        camera->notifyCameraChanged();
    }

    Vector3 m_dir;
    Vector3 m_up;
    float m_radius;
};

void Camera::roll(float degrees, float ms)
{
    if (ms < 0.0f) {
        Matrix matrixRoll;
        Matrix::createRotation(m_dir, MATH_DEG_TO_RAD(degrees), &matrixRoll);
        matrixRoll.transformVector(&m_up);
        m_up.normalize();
        LOG_BASE("roll: up:%.2f %.2f %.2f", m_up.x, m_up.y, m_up.z);
        notifyCameraChanged();
    }
    else {
        CameraTransitionRoll* rolltrans = new CameraTransitionRoll(this, m_dir, m_up, MATH_DEG_TO_RAD(degrees));
        rolltrans->initTimeline(ms, true);
        runAction(rolltrans);
    }
}

void Camera::updateTransform()
{
    m_dirtys = 0;
    if (!m_transformDirty) {
        return;
    }
    m_transformDirty = false;
//    Vector3 dir_default(0, 0, -1);
//    m_dir.normalize();
//    if (m_dir != dir_default) {
//        Vector3 axis;
//        Vector3::cross(m_dir, dir_default, &axis);
//        Matrix matRotate;
//        float dx = dir_default.y * m_dir.z - dir_default.z * m_dir.y;
//        float dy = dir_default.z * m_dir.x - dir_default.x * m_dir.z;
//        float dz = dir_default.x * m_dir.y - dir_default.y * m_dir.x;
//        float angle = -atan2f(sqrt(dx * dx + dy * dy + dz * dz) + MATH_FLOAT_SMALL, Vector3::dot(dir_default, m_dir));
//        matRotate.rotate(Quaternion(axis, angle));
//        matRotate.transformVector(Vector3(0, 1, 0), &m_up);
//        m_up.normalize();
//    }
//    else {
//        m_up = Vector3(0, 1, 0);
//    }
    m_matLookat.setIdentity();
    Matrix::createLookAt(m_eye, m_eye + m_dir, m_up, &m_matLookat);
    m_matProjection = m_matPerspective * m_matLookat;
    LOG_BASE("[eye:x:%lf y:%lf z:%lf][dir:%lf %lf %lf]", m_eye.x, m_eye.y, m_eye.z, m_dir.x, m_dir.y, m_dir.z);
}

void Camera::notifyCameraChanged()
{
    m_transformDirty = true;
    invalidate(FLAGS_MATERIAL_DIRTY);
    // NGPOST(cameraChanged);
}
