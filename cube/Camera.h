#ifndef CAMERA_H
#define CAMERA_H

// #include "Base.h"
#include "MathInc.h"
#include "Node.h"

class Camera : public Node
{
    friend class CameraTransitionRoll;
    friend class CameraTransitionAngle;
    friend class CameraTransitionRotate;
public:
    static Camera* createCamera2D(const std::string& name,
                                  const Rect& viewport,
                                  float zNear = 0.3f,
                                  float zFar = 100.0f);

    static Camera* createCamera3D(const std::string& name,
                                    const Vector3& eye,
                                    const Vector3& dir,
                                    const Rect& viewport,
                                    float fieldOfView = 60.0f,
                                    float zNear = 0.3f,
                                    float zFar = 100.0f);


    /* get 3d projection matrix */
    Matrix projection()
    {
        updateTransform();
        return m_matProjection;
    }

    /* get Perspective matrix */
    Matrix perspective()
    {
        updateTransform();
        return m_matPerspective;
    }

    /* get lookat matrix */
    Matrix lookat()
    {
        updateTransform();
        return m_matLookat;
    }

    /* return the ViewPort of the Camera */
    inline Rect viewPort() const
    {
        return m_viewport;
    }

    /* return the degree of view field */
    inline float fieldOfView() const
    {
        return m_fieldOfView;
    }

    /* set the Eye value of the Camera */
    void setEye(const Vector3& eye);
    void setEye(float x, float y, float z);

    /* return the Eye value of the Camera */
    inline Vector3 eye() const
    {
        return m_eye;
    }

    /* set the Direction value of the Camera */
    void setDirection(const Vector3& dir);
    void setDirection(float x, float y, float z);

    /* return the Direction value of the Camera */
    inline Vector3 direction() const
    {
        return m_dir;
    }

    void setUp(const Vector3& dir);
    void setUp(float x, float y, float z);
    inline Vector3 up() const { return m_up; }

    void moveLeft(float step = 1.0f);
    void moveRight(float step = 1.0f);
    void moveForward(float step = 1.0f);
    void moveBackward(float step = 1.0f);
    void moveUp(float step = 1.0f);
    void moveDown(float step = 1.0f);
    /* degrees > 0 roll clockwise  */
    void roll(float degrees, float ms = -1.0f);
    void restore();

    void notifyCameraChanged();
    // NGSIGNAL() cameraChanged;

protected:
    enum PROJECT_TYPE {
        PROJECT_2D,
        PROJECT_3D
    };

    Camera(const std::string& name = "");

    bool init(const Rect& viewport,
              float zNear = 0.3f,
              float zFar = 100.0f);

    bool init(const Vector3& eye,
              const Vector3& dir,
              const Rect& viewport,
              float fieldOfView = 60.0f,
              float zNear = 0.3f,
              float zFar = 100.0f);

    void updateTransform();

    int m_cameraType;
    float m_fieldOfView;
    Vector3 m_dir;
    Vector3 m_eye;
    Vector3 m_up;
    Rect m_viewport;

    bool m_transformDirty;
    Matrix m_matPerspective;
    Matrix m_matLookat;
    Matrix m_matProjection;
    float m_step;

    friend class Director;
};

#endif // CAMERA_H
