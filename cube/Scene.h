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
    virtual void onGesture(const GestureObject& ev_const);
    virtual void onTimeOut(int id);

    void updateArcball();

    bool isEnable();
    void setLayout(int layout);

    enum GYROMODE_TYPE {
         GYROMODE_ONE,
         GYROMODE_TWO
    };
    void setGyroMode(int mode);
    int longclickScene();
    void flickCube(bool run = true);
    void rotateScene(float degrees, float ms = -1.0f);
    void setCubePlaneTexture(int planeid, int txtid);
    void addCubeCallback(CubeCallback* cb);
    int popSelectedTexture();
    int popEvent();
protected:    
    void disableInTime(float ms);
    void gyroCube(int mode, bool forward, float ms);
    void zoomCamera(float from, float to, float ms);
    Arcball* m_arcball;
    int m_gyroMode;
    bool m_enable;
    bool m_isCubeMode;
    bool m_flicking;
    float m_flickSpeed;
    Vector3 m_flickAxis;
    Quaternion m_flickPreQuat;
    Matrix m_rotateMat;
    CubeCallback* m_cb;
    int m_txtIdSelected;
    int m_eventId;
};

#endif // SCENE_H
