#include "threeFlickGesture.h"
#include <string.h>

ThreeFlickGesture::ThreeFlickGesture():
GestureObject(GESTURE_TYPE_3FLICK),
mPointNum(0)
{
	memset(mPoint, 0, sizeof(mPoint));
	memset(mStartPoint, 0, sizeof(mStartPoint));
	memset(&mCenter, 0, sizeof(mCenter));
	memset(&mLastCenter, 0, sizeof(mLastCenter));
	memset(&mStartCenter, 0, sizeof(mStartCenter));
	memset(&mOffset, 0, sizeof(mOffset));
	memset(&mLastOffset, 0, sizeof(mLastOffset));
	memset(&mStartOffset, 0, sizeof(mStartOffset));
}

ThreeFlickGesture::~ThreeFlickGesture()
{
}

void ThreeFlickGesture::setPoint(int index, int x, int y)
{
	if (THREE_FLICK_GESTURE_MAX_POINTS > index && 0 <= index) {
		mPoint[index].x = x;
		mPoint[index].y = y;
	}
}

GesturePoint& ThreeFlickGesture::getPoint(int index)
{
	if (THREE_FLICK_GESTURE_MAX_POINTS > index && 0 <= index) {
		return mPoint[index];
	}

	return mPoint[0];
}

void ThreeFlickGesture::setStartPoint(int index, int x, int y)
{
	if (THREE_FLICK_GESTURE_MAX_POINTS > index && 0 <= index) {
		mStartPoint[index].x = x;
		mStartPoint[index].y = y;
	}
}

GesturePoint& ThreeFlickGesture::getStartPoint(int index)
{
	if (THREE_FLICK_GESTURE_MAX_POINTS > index && 0 <= index) {
		return mStartPoint[index];
	}

	return mStartPoint[0];
}

void ThreeFlickGesture::setPointNum(int num)
{
	mPointNum = num;
}

int ThreeFlickGesture::getPointNum()
{
	return mPointNum;
}

void ThreeFlickGesture::setCenter(int x, int y)
{
	mCenter.x = x;
	mCenter.y = y;
}

GesturePoint& ThreeFlickGesture::getCenter()
{
	return mCenter;
}

void ThreeFlickGesture::setLastCenter(int x, int y)
{
	mLastCenter.x = x;
	mLastCenter.y = y;
}

GesturePoint& ThreeFlickGesture::getLastCenter()
{
	return mLastCenter;
}

void ThreeFlickGesture::setStartCenter(int x, int y)
{
	mStartCenter.x = x;
	mStartCenter.y = y;
}

GesturePoint& ThreeFlickGesture::getStartCenter()
{
	return mStartCenter;
}

void ThreeFlickGesture::setOffset(int x, int y)
{
	mOffset.x = x;
	mOffset.y = y;
}

GesturePoint& ThreeFlickGesture::getOffset()
{
	return mOffset;
}

void ThreeFlickGesture::setLastOffset(int x, int y)
{
	mLastOffset.x = x;
	mLastOffset.y = y;
}

GesturePoint& ThreeFlickGesture::getLastOffset()
{
	return mLastOffset;
}

void ThreeFlickGesture::setStartOffset(int x, int y)
{
	mStartOffset.x = x;
	mStartOffset.y = y;
}

GesturePoint& ThreeFlickGesture::getStartOffset()
{
	return mStartOffset;
}

bool ThreeFlickGesture::copyData(GestureObject* object)
{
	if (!object) {
		return false;
	}
	
	if (object->getType() != getType()) {
		return false;
	}

	ThreeFlickGesture* ges = static_cast<ThreeFlickGesture*>(object);
	mState = ges->mState;

	mPointNum = ges->mPointNum;
	mCenter = ges->mCenter;
	mLastCenter = ges->mLastCenter;
	mStartCenter = ges->mStartCenter;
	mOffset = ges->mOffset;
	mLastOffset = ges->mLastOffset;
	mStartOffset = ges->mStartOffset;

	for (int i = 0; i < THREE_FLICK_GESTURE_MAX_POINTS; i++) {
		mPoint[i] = ges->mPoint[i];
		mStartPoint[i] = ges->mStartPoint[i];
	}

	return true;
}

void ThreeFlickGesture::clearData()
{
	memset(mPoint, 0, sizeof(mPoint));
	memset(mStartPoint, 0, sizeof(mStartPoint));
	memset(&mCenter, 0, sizeof(mCenter));
	memset(&mLastCenter, 0, sizeof(mLastCenter));
	memset(&mStartCenter, 0, sizeof(mStartCenter));
	memset(&mOffset, 0, sizeof(mOffset));
	memset(&mLastOffset, 0, sizeof(mLastOffset));
	memset(&mStartOffset, 0, sizeof(mStartOffset));

	mPointNum = 0;
	GestureObject::clearData();
}

void ThreeFlickGesture::initializeGestureData(struct MultiTouchPoint *mtPoints, unsigned int pointCount)
{
	if (!mtPoints) {
		return;
	}

	int num = pointCount > THREE_FLICK_GESTURE_MAX_POINTS ? THREE_FLICK_GESTURE_MAX_POINTS : pointCount;
	int pressNum = 0;
	GesturePoint center;
	memset(&center, 0, sizeof(center));
	for (int i = 0; i < num; i++) {
		if (TouchPointReleased != mtPoints[i].state) {
			setStartPoint(pressNum, mtPoints[i].coords.x, mtPoints[i].coords.y);
			setPoint(pressNum, mtPoints[i].coords.x, mtPoints[i].coords.y);
			center.x += mtPoints[i].coords.x;
			center.y += mtPoints[i].coords.y;
			++pressNum;
		}
	}

	center.x /= pressNum;
	center.y /= pressNum;
	setPointNum(pressNum);
	setLastCenter(center.x, center.y);
	setCenter(center.x, center.y);
	setStartCenter(center.x, center.y);
	setLastOffset(0, 0);
	setOffset(0, 0);
	setStartOffset(0, 0);
}

void ThreeFlickGesture::updateGestureData(struct MultiTouchPoint *mtPoints, unsigned int pointCount)
{
	if (!mtPoints) {
		return;
	}

	int num = pointCount > THREE_FLICK_GESTURE_MAX_POINTS ? THREE_FLICK_GESTURE_MAX_POINTS : pointCount;
	GesturePoint center;
	memset(&center, 0, sizeof(center));
	for (int i = 0; i < num; i++) {
		setPoint(i, mtPoints[i].coords.x, mtPoints[i].coords.y);
		center.x += mtPoints[i].coords.x;
		center.y += mtPoints[i].coords.y;
	}

	center.x /= num ;
	center.y /= num ;
	setPointNum(num);
	setLastCenter(getCenter().x, getCenter().y);
	setCenter(center.x, center.y);
	setLastOffset(getOffset().x, getOffset().y);
	setOffset(getLastCenter().x - getCenter().x, getLastCenter().y - getCenter().y);
	setStartOffset(getStartCenter().x - getCenter().x, getStartCenter().y - getCenter().y);
}
