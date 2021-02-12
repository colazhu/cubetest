#include "rotateGesture.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

RotateGesture::RotateGesture()
    :GestureObject(GESTURE_TYPE_2ROTARY),
      mTotalRotationAngle(0), mLastRotationAngle(0), 
      mRotationAngle(0), mStartRotationAngle(0)
{
    memset(mPoints, 0, sizeof(mPoints));
    memset(mLastPoints, 0, sizeof(mLastPoints));
    memset(mStartPoints, 0, sizeof(mStartPoints));
	memset(&mCenterPoint, 0, sizeof(mCenterPoint));
	memset(&mLastCenterPoint, 0, sizeof(mLastCenterPoint));
	memset(&mStartCenterPoint, 0, sizeof(mStartCenterPoint));
}

RotateGesture::~RotateGesture()
{
}

GesturePoint* RotateGesture::getPoints()
{
    return mPoints;
}

void RotateGesture::setPoints(int x1, int y1, int x2, int y2)
{
	mPoints[0].x = x1;
	mPoints[1].x = x2;

	mPoints[0].y = y1;
	mPoints[1].y = y2;
}

GesturePoint* RotateGesture::getLastPoints()
{
    return mLastPoints;
}

void RotateGesture::setLastPoints(int x1, int y1, int x2, int y2)
{
    mLastPoints[0].x = x1;
    mLastPoints[1].x = x2;

    mLastPoints[0].y = y1;
    mLastPoints[1].y = y2;
}

GesturePoint* RotateGesture::getStartPoints()
{
    return mStartPoints;
}

void RotateGesture::setStartPoints(int x1, int y1, int x2, int y2)
{
    mStartPoints[0].x = x1;
    mStartPoints[1].x = x2;

    mStartPoints[0].y = y1;
    mStartPoints[1].y = y2;
}

float RotateGesture::calcRotationAngle()
{
    float f1 = atan2(mPoints[1].y - mPoints[0].y, mPoints[1].x - mPoints[0].x);
    float f2 = atan2(mLastPoints[1].y - mLastPoints[0].y, mLastPoints[1].x - mLastPoints[0].x);
    if(abs(f1 - f2) > PI) {
        if(f1 < 0) {
            return 2 * PI + f1 - f2;
        }
        else {
            return  -1 * (2 * PI + f2 - f1);
        }
    }
    else {
        return f1 - f2;
    }
}

float RotateGesture::calcStartRotationAngle()
{
    float f1 = atan2(mPoints[1].y - mPoints[0].y, mPoints[1].x - mPoints[0].x);
    float f2 = atan2(mStartPoints[1].y - mStartPoints[0].y, mStartPoints[1].x - mStartPoints[0].x);
    if(abs(f1 - f2) > PI) {
        if(f1 < 0) {
            return 2 * PI + f1 - f2;
        }
        else {
            return  -1 * (2 * PI + f2 - f1);
        }
    }
    else {
        return f1 - f2;
    }
}

bool RotateGesture::copyData(GestureObject* object)
{
    if (!object) {
        return false;
    }

    if (object->getType() != getType()) {
        return false;
    }

    RotateGesture* ges = static_cast<RotateGesture*>(object);
    mState = ges->mState;
    for (int i = 0; i < 2; ++i) {
        mPoints[i] = ges->mPoints[i];
        mStartPoints[i] = ges->mStartPoints[i];
        mLastPoints[i] = ges->mLastPoints[i];
    }

    mStartCenterPoint = ges->mStartCenterPoint;
    mLastCenterPoint = ges->mLastCenterPoint;
    mCenterPoint = ges->mCenterPoint;
    mTotalRotationAngle = ges->mTotalRotationAngle;
    mLastRotationAngle = ges->mLastRotationAngle;
    mRotationAngle = ges->mRotationAngle;
	mStartRotationAngle = ges->mStartRotationAngle;

    return true;
}

void RotateGesture::clearData() {
    GestureObject::clearData();
    for (int i = 0; i < 2; ++i) {
        memset(&mPoints[i], 0 , sizeof(mPoints[i]));
        memset(&mStartPoints[i], 0, sizeof(mStartPoints[i]));
        memset(&mLastPoints[i], 0, sizeof(mLastPoints[i]));
    }

    mStartCenterPoint.x = 0;
    mStartCenterPoint.y = 0;
    mLastCenterPoint.x = 0;
    mLastCenterPoint.y = 0;
    mCenterPoint.x = 0;
    mCenterPoint.y = 0;
    mTotalRotationAngle = 0;
    mLastRotationAngle = 0;
    mRotationAngle = 0;
	mStartRotationAngle = 0;
}

