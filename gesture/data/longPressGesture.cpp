#include "longPressGesture.h"
#include <string.h>

LongPressGesture::LongPressGesture():
GestureObject(GESTURE_TYPE_LONGPRESS),
mActionInterval(0)
{
	memset(&mPoint, 0, sizeof(mPoint));
	memset(&mPressTime, 0, sizeof(mPressTime));
}

LongPressGesture::~LongPressGesture()
{
}

void LongPressGesture::setPoint(GesturePoint& point)
{
	mPoint = point;
}

void LongPressGesture::setPoint(int x, int y)
{
	mPoint.x = x;
	mPoint.y = y;
}

GesturePoint& LongPressGesture::getPoint()
{
	return mPoint;
}

bool LongPressGesture::copyData(GestureObject* object)
{
	if (!object) {
		return false;
	}
	
	if (object->getType() != getType()) {
		return false;
	}

	LongPressGesture* ges = static_cast<LongPressGesture*>(object);
	mState = ges->mState;
	mPoint = ges->mPoint;
	mPressTime = ges->mPressTime;
	mActionInterval = ges->mActionInterval;

	return true;
}

void LongPressGesture::clearData()
{
	memset(&mPoint, 0, sizeof(mPoint));
	memset(&mPressTime, 0, sizeof(mPressTime));
	mActionInterval = 0;
	GestureObject::clearData();
}

void LongPressGesture::updatePressTime(struct timeval& time)
{
	mPressTime = time;
}

struct timeval& LongPressGesture::getPressTime()
{
	return mPressTime;
}

void LongPressGesture::setActionInterval(int actionInterval)
{
	mActionInterval = actionInterval;
}

int LongPressGesture::getActionInterval()
{
	return mActionInterval;
}

