#ifndef CUBEPLANE_H
#define CUBEPLANE_H

#include "Cube.h"
class GyroZoomInTransition;
class CubePlane : public Cube
{
    friend class GyroZoomInTransition;
    friend class DelayTransition;
public:
    CubePlane(const std::string& name = "", Node *parent = 0);
    virtual ~CubePlane();

    virtual void onDraw();
    virtual void onTimeOut(int id);
    void setTarget(Cube* cube) { m_target = cube;}

    void syncTarget(Cube* cube);
    void doGyro();
    void doBack();
protected:
    void doDelayTrasition();
    void doGyroZoomInTransition();

    Cube* m_target;
    GyroZoomInTransition* m_transition;
};

#endif // CUBEPLANE_H
