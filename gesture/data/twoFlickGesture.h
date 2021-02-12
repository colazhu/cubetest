#ifndef TWOFLICKGESTURE_H
#define TWOFLICKGESTURE_H

#include "gestureObject.h"

#include <sys/time.h>
#include <time.h>
#include <list>

#define TWOFLICKGESTURETRACEINTREVAL  50

class TwoFlickGestureTrace;
class TwoFlickGesture:public GestureObject
{
public:
	TwoFlickGesture();
	virtual ~TwoFlickGesture();
	
	void setPoint(int index, int x, int y);
	GesturePoint& getPoint(int index);

	void setCenter(int x, int y);
	GesturePoint& getCenter();

	void setSpeed(unsigned int speed);
	unsigned int getSpeed();

	void setAngle(float angle);
	float& getAngle();

	void updateActionTime(unsigned long time);
    void updatePointerUpTime(unsigned long time);

	virtual bool copyData(GestureObject* object);
	virtual void clearData();

    unsigned long interval(unsigned long time);
    unsigned long pointerUpInterval(unsigned long time);

	void updateTraceArray(int x, int y, unsigned long time);
	void resetTraceArray(int x, int y, unsigned long time);

private:
	GesturePoint mPoint[2];
	GesturePoint mCenter;
	unsigned int mSpeed;
	float mAngle;
    unsigned long mPointerUpTime;
	std::list<TwoFlickGestureTrace> mTraceList;
};

#endif /* TWOFLICKGESTURE_H */
