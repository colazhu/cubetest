#include "tapGesture.h"
#include <string.h>

TapGesture::TapGesture():
GestureObject(GESTURE_TYPE_TAP)
{
	memset(&mPoint, 0, sizeof(mPoint));
	memset(&mStartPoint, 0, sizeof(mStartPoint));
	memset(&mTime, 0 , sizeof(mTime));
}

TapGesture::~TapGesture()
{
}

void TapGesture::setPoint(GesturePoint& point)
{
	mPoint = point;
}

void TapGesture::setPoint(int x, int y)
{
	mPoint.x = x;
	mPoint.y = y;
}

GesturePoint& TapGesture::getPoint()
{
	return mPoint;
}

void TapGesture::setStartPoint(int x, int y)
{
	mStartPoint.x = x;
	mStartPoint.y = y;
}

GesturePoint& TapGesture::getStartPoint()
{
	return mStartPoint;
}

void TapGesture::updateTime(unsigned long time)
{
	mTime = time;
}

unsigned long TapGesture::getTime()
{
	return mTime;
}

bool TapGesture::copyData(GestureObject* object)
{
	if (!object) {
		return false;
	}
	
	if (object->getType() != getType()) {
		return false;
	}

	TapGesture* ges = static_cast<TapGesture*>(object);
	mState = ges->mState;
	mPoint = ges->mPoint;
	mStartPoint = ges->mStartPoint;
	mTime = ges->mTime;

	return true;
}

void TapGesture::clearData()
{
	memset(&mPoint, 0, sizeof(mPoint));
	memset(&mStartPoint, 0, sizeof(mStartPoint));
	memset(&mTime, 0 , sizeof(mTime));
	GestureObject::clearData();
}



