#ifndef FLICKGESTURE_H
#define FLICKGESTURE_H

#include "gestureObject.h"
#include <list>

#define FLICKGESTURETRACEINTERVAL  50

class FlickGestureTrace ;
class FlickGesture:public GestureObject
{
public:
    FlickGesture();
    virtual ~FlickGesture();

    virtual bool copyData(GestureObject* object);
    virtual void clearData();

    GesturePoint getPoint() {return mPoint;}
    float& getAngle() { return mAngle;}
    unsigned int getSpeed() { return mSpeed;}

    unsigned long pointerUpInterval(unsigned long curTime);

    void setPoint(GesturePoint& point);
    void setPoint(int x, int y);
    void setAngle(const GesturePoint& point);
    void setAngle(float value) {mAngle = value;}
    void setSpeed(unsigned int value) {mSpeed = value;}

	void updatePointerUpTime(unsigned long time);

	void updateTraceArray(int x, int y, unsigned long time);
	void resetTraceArray(int x, int y, unsigned long time);

private:
    float mAngle;
    unsigned int mSpeed;
    GesturePoint mPoint;
    unsigned long mPointerUpTime;
    std::list<FlickGestureTrace> mTraceList;
};

#endif // FLICKGESTURE_H
