#ifndef SCENE_H
#define SCENE_H

#include "Node.h"

class Arcball;
class Scene : public Node
{
    friend class Director;
public:
    Scene(const std::string& name);
    virtual ~Scene();

    void init();
    void deinit();

    virtual void onUpdate(float dt);
    virtual void onTouch(TouchAction touchevent, float x, float y);
    virtual void onGesture(const GestureEvent& ev);
    virtual void onTimeOut(int id);

    void updateArcball();

    void setLayout(int layout);

    enum GYROMODE_TYPE {
         GYROMODE_ONE,
         GYROMODE_TWO
    };
    void setGyroMode(int mode);
    void flickCube(bool run = true);
    void rotateScene(float degrees, float ms = -1.0f);
    void setCubePlaneTexture(int planeid, int txtid);

protected:    
    void disableInTime(float ms);
    void gyroCube(int mode, bool forward, float ms);
    Arcball* m_arcball;
    int m_gyroMode;
    bool m_enable;
    bool m_isCubeMode;
    bool m_flicking;
    float m_flickSpeed;
    Vector3 m_flickAxis;
    Quaternion m_flickPreQuat;
    Matrix m_rotateMat;
};

#endif // SCENE_H
