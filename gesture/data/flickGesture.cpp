#include "flickGesture.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

class FlickGestureTrace {
public:
	FlickGestureTrace(int x, int y, unsigned long time);
	int getX() const { return mPoint.x;}
	int getY() const { return mPoint.y;}
	unsigned long getTime() const { return mTime;}
private:
	GesturePoint mPoint;
	unsigned long mTime;
};

FlickGestureTrace::FlickGestureTrace(int x, int y, unsigned long time)
{
	mPoint.x = x;
	mPoint.y = y;
	mTime = time;
}

FlickGesture::FlickGesture():
GestureObject(GESTURE_TYPE_FLICK),
mAngle(0),
mSpeed(0)
{
    memset(&mPoint, 0, sizeof(mPoint));
    mPointerUpTime = 0;
    mTraceList.clear();
}

FlickGesture::~FlickGesture()
{
}

void FlickGesture::setPoint(GesturePoint& point)
{
    mPoint = point;
}

void FlickGesture::setPoint(int x, int y)
{
    mPoint.x = x;
    mPoint.y = y;
}

unsigned long FlickGesture::pointerUpInterval(unsigned long curTime)
{
	return (0 == curTime - mPointerUpTime) ? 1 : (curTime - mPointerUpTime);
}

void FlickGesture::setAngle(const GesturePoint& point)
{
	mAngle = atan2(point.y - mPoint.y, point.y - mPoint.y);
}

void FlickGesture::updatePointerUpTime(unsigned long time)
{
	mPointerUpTime = time;
}

bool FlickGesture::copyData(GestureObject* object)
{
    if (!object) {
        return false;
    }

    if (object->getType() != getType()) {
        return false;
    }

    FlickGesture* ges = static_cast<FlickGesture*>(object);
    mState = ges->mState;
    mPoint = ges->mPoint;
	mPointerUpTime = ges->mPointerUpTime;
    mAngle = ges->mAngle;
    mSpeed = ges->mSpeed;

    return true;
}

void FlickGesture::clearData() {
    mAngle = 0;
    mSpeed = 0;
	mPointerUpTime = 0;
	mTraceList.clear();
	
    memset(&mPoint, 0 , sizeof(mPoint));
    GestureObject::clearData();
}

void FlickGesture::updateTraceArray(int x, int y, unsigned long time)
{
	mTraceList.push_front(FlickGestureTrace(x, y, time));
	unsigned long interval = time - mTraceList.back().getTime();
	while (interval > FLICKGESTURETRACEINTERVAL) {
		mTraceList.pop_back();
		interval = time - mTraceList.back().getTime();
	}

	if (0 == interval) {
		return;
	}

	mSpeed = 100 * hypot(mTraceList.front().getX() - mTraceList.back().getX(), mTraceList.front().getY() - mTraceList.back().getY()) / 
			(mTraceList.front().getTime() - mTraceList.back().getTime());

	mAngle = atan2( mTraceList.back().getY() - mTraceList.front().getY(), mTraceList.back().getX() - mTraceList.front().getX());
}

void FlickGesture::resetTraceArray(int x, int y, unsigned long time)
{
	mTraceList.clear();
	mSpeed = 0;
	mAngle = 0;
	mTraceList.push_front(FlickGestureTrace(x, y, time));
}
