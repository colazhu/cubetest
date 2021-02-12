#include "scratchGesture.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

ScratchGesture::ScratchGesture():
GestureObject(GESTURE_TYPE_SCRATCH),
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

ScratchGesture::~ScratchGesture()
{
}

void ScratchGesture::setPoint(int index, int x, int y)
{
	if (SCRATCH_GESTURE_MAX_POINTS > index && 0 <= index) {
		mPoint[index].x = x;
		mPoint[index].y = y;
	}
}

void ScratchGesture::setStartPoint(int index, int x, int y)
{
    if (SCRATCH_GESTURE_MAX_POINTS > index && 0 <= index) {
        mStartPoint[index].x = x;
        mStartPoint[index].y = y;
    }
}

GesturePoint& ScratchGesture::getPoint(int index)
{
	if (SCRATCH_GESTURE_MAX_POINTS > index && 0 <= index) {
		return mPoint[index];
	}

	return mPoint[0];
}

GesturePoint& ScratchGesture::getStartPoint(int index)
{
    if (SCRATCH_GESTURE_MAX_POINTS > index && 0 <= index) {
        return mStartPoint[index];
    }

    return mStartPoint[0];
}

void ScratchGesture::getPoints(GesturePoint* points, int maxNum, int* num)
{
    *num = maxNum < mPointNum? maxNum:mPointNum;
    for (int i = 0; i < *num; i++) {
        points[i] = mPoint[i];
    }
}

void ScratchGesture::setPointNum(int num)
{
	mPointNum = num;
}

int ScratchGesture::getPointNum()
{
	return mPointNum;
}

void ScratchGesture::setCenter(int x, int y)
{
	mCenter.x = x;
	mCenter.y = y;
}

GesturePoint& ScratchGesture::getCenter()
{
	return mCenter;
}

void ScratchGesture::setLastCenter(int x, int y)
{
	mLastCenter.x = x;
	mLastCenter.y = y;
}

GesturePoint& ScratchGesture::getLastCenter()
{
	return mLastCenter;
}

void ScratchGesture::setStartCenter(int x, int y)
{
	mStartCenter.x = x;
	mStartCenter.y = y;
}

GesturePoint& ScratchGesture::getStartCenter()
{
	return mStartCenter;
}

void ScratchGesture::setOffset(int x, int y)
{
	mOffset.x = x;
	mOffset.y = y;
}

GesturePoint& ScratchGesture::getOffset()
{
	return mOffset;
}

void ScratchGesture::setLastOffset(int x, int y)
{
	mLastOffset.x = x;
	mLastOffset.y = y;
}

GesturePoint& ScratchGesture::getLastOffset()
{
	return mLastOffset;
}

void ScratchGesture::setStartOffset(int x, int y)
{
	mStartOffset.x = x;
	mStartOffset.y = y;
}

GesturePoint& ScratchGesture::getStartOffset()
{
	return mStartOffset;
}

bool ScratchGesture::copyData(GestureObject* object)
{
	if (!object) {
		return false;
	}
	
	if (object->getType() != getType()) {
		return false;
	}

	ScratchGesture* ges = static_cast<ScratchGesture*>(object);
	mState = ges->mState;
	for (int i = 0; i < SCRATCH_GESTURE_MAX_POINTS; i++) {
		mPoint[i] = ges->mPoint[i];
        mStartPoint[i] = ges->mStartPoint[i];
	}

	mPointNum = ges->mPointNum;
	mCenter = ges->mCenter;
	mLastCenter = ges->mLastCenter;
	mStartCenter = ges->mStartCenter;
	mOffset = ges->mOffset;
	mLastOffset = ges->mLastOffset;
	mStartOffset = ges->mStartOffset;
	return true;
}

void ScratchGesture::clearData()
{
	memset(mPoint, 0, sizeof(mPoint));
    memset(mStartPoint, 0 , sizeof(mStartPoint));
	memset(&mCenter, 0, sizeof(mCenter));
	memset(&mLastCenter, 0, sizeof(mLastCenter));
	memset(&mStartCenter, 0, sizeof(mStartCenter));
	memset(&mOffset, 0, sizeof(mOffset));
	memset(&mLastOffset, 0, sizeof(mLastOffset));
	memset(&mStartOffset, 0, sizeof(mStartOffset));
	mPointNum = 0;
    GestureObject::clearData();
}

void ScratchGesture::updateGestureData(int pointCount, struct MultiTouchPoint *mtPoints, bool isFirstInitialize)
{
	if(!mtPoints) {
		return;
	}

	int num = pointCount > SCRATCH_GESTURE_MAX_POINTS ? SCRATCH_GESTURE_MAX_POINTS : pointCount;
		setPointNum(num);
		setLastCenter(getCenter().x, getCenter().y);
		setLastOffset(getOffset().x, getOffset().y);

		GesturePoint center;
		memset(&center, 0, sizeof(center));
		for (int i = 0; i < num; i++) {
			setPoint(i, mtPoints[i].coords.x, mtPoints[i].coords.y);
			center.x += mtPoints[i].coords.x;
			center.y += mtPoints[i].coords.y;
		}

		center.x /= num;
		center.y /= num;
		setCenter(center.x, center.y);
		setOffset(getLastCenter().x - getCenter().x, getLastCenter().y - getCenter().y);
		setStartOffset(getStartCenter().x - getCenter().x, getStartCenter().y - getCenter().y);
		if(isFirstInitialize) {
			for (int i = 0; i < num; i++) {
				setStartPoint(i, mtPoints[i].coords.x, mtPoints[i].coords.y);
			}

			setStartCenter(center.x, center.y);
		}
}


