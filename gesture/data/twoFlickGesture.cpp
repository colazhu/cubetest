#include "twoFlickGesture.h"
#include <string.h>
#include <math.h>

class TwoFlickGestureTrace {
public:
	TwoFlickGestureTrace(int x, int y, unsigned long time);
	int getX() const { return mCenter.x;}
	int getY() const { return mCenter.y;}
	unsigned long getTime() const { return mTime;}
private:
	GesturePoint mCenter;
	unsigned long mTime;
};

TwoFlickGestureTrace::TwoFlickGestureTrace(int x, int y, unsigned long time)
{
	mCenter.x = x;
	mCenter.y = y;
	mTime = time;
}

TwoFlickGesture::TwoFlickGesture():
GestureObject(GESTURE_TYPE_2FLICK),
mSpeed(0),
mAngle(0)
{
	memset(&mPoint[0], 0, sizeof(mPoint[0]));
	memset(&mPoint[1], 0, sizeof(mPoint[1]));
	memset(&mCenter, 0, sizeof(mCenter));
    memset(&mPointerUpTime, 0, sizeof(mPointerUpTime));
	mTraceList.clear();
}

TwoFlickGesture::~TwoFlickGesture()
{
}

void TwoFlickGesture::setPoint(int index, int x, int y)
{
	if (0 == index || 1 == index) {
		mPoint[index].x = x;
		mPoint[index].y = y;
	}
}

GesturePoint& TwoFlickGesture::getPoint(int index)
{
	if (0 == index || 1 == index) {
		return mPoint[index];
	}

	return mPoint[0];
}

void TwoFlickGesture::setCenter(int x, int y)
{
	mCenter.x = x;
	mCenter.y = y;
}

GesturePoint& TwoFlickGesture::getCenter()
{
	return mCenter;
}

void TwoFlickGesture::setSpeed(unsigned int speed)
{
	mSpeed = speed;
}

unsigned int TwoFlickGesture::getSpeed()
{
	return mSpeed;
}

void TwoFlickGesture::setAngle(float angle)
{
	mAngle = angle;
}

float& TwoFlickGesture::getAngle()
{
	return mAngle;
}

void TwoFlickGesture::updatePointerUpTime(unsigned long time)
{
    mPointerUpTime = time;
}

bool TwoFlickGesture::copyData(GestureObject* object)
{
	if (!object) {
		return false;
	}
	
	if (object->getType() != getType()) {
		return false;
	}

	TwoFlickGesture* ges = static_cast<TwoFlickGesture*>(object);
	mState = ges->mState;
	
	mPoint[0] = ges->mPoint[0];
	mPoint[1] = ges->mPoint[1];
	
	mCenter = ges->mCenter;
	mSpeed = ges->mSpeed;
	mAngle = ges->mAngle;
    mPointerUpTime = ges->mPointerUpTime;

	return true;
}

void TwoFlickGesture::clearData()
{
	memset(&mPoint[0], 0, sizeof(mPoint[0]));
	memset(&mPoint[1], 0, sizeof(mPoint[1]));
	memset(&mCenter, 0, sizeof(mCenter));
    memset(&mPointerUpTime, 0, sizeof(mPointerUpTime));

	mSpeed = 0;
	mAngle = 0;
	mTraceList.clear();

    GestureObject::clearData();
}

unsigned long TwoFlickGesture::pointerUpInterval(unsigned long time)
{
    return (0 == time - mPointerUpTime) ? 1 : (time - mPointerUpTime);
}


void TwoFlickGesture::updateTraceArray(int x, int y, unsigned long time)
{

	mTraceList.push_front(TwoFlickGestureTrace(x, y, time));
	unsigned long interval = time - mTraceList.back().getTime();
	while (interval > TWOFLICKGESTURETRACEINTREVAL) {
		mTraceList.pop_back();
		interval = time - mTraceList.back().getTime();
	}

	if (0 == interval) {
		return;
	}

	mSpeed = 100 * hypot(mTraceList.front().getX() - mTraceList.back().getX(), mTraceList.front().getY() - mTraceList.back().getY()) / 
			(mTraceList.front().getTime() - mTraceList.back().getTime());

	mAngle = atan2(mTraceList.back().getY() - mTraceList.front().getY(), mTraceList.back().getX() - mTraceList.front().getX());

}

void TwoFlickGesture::resetTraceArray(int x, int y, unsigned long time)
{
	mTraceList.clear();
	mSpeed = 0;
	mAngle = 0;
	mTraceList.push_front(TwoFlickGestureTrace(x, y, time));
}


