#include "pinchGesture.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

PinchGesture::PinchGesture()
    :GestureObject(GESTURE_TYPE_PINCH),
      mTotalScaleFactor(1), mLastScaleFactor(1), mScaleFactor(1),
      mTotalRotationAngle(1), mLastRotationAngle(0), mRotationAngle(0)
{
    memset(mPoints, 0, sizeof(mPoints));
	memset(mLastPoints, 0, sizeof(mLastPoints));
	memset(mStartPoints, 0, sizeof(mStartPoints));
	memset(&mCenterPoint, 0, sizeof(mCenterPoint));
	memset(&mLastCenterPoint, 0, sizeof(mLastCenterPoint));
	memset(&mStartCenterPoint, 0, sizeof(mStartCenterPoint));
}

PinchGesture::~PinchGesture()
{
}

GesturePoint* PinchGesture::getPoints()
{
    return mPoints;
}

void PinchGesture::setPoints(int x1, int y1, int x2, int y2)
{
    mPoints[0].x = x1;
    mPoints[1].x = x2;

	mPoints[0].y = y1;
    mPoints[1].y = y2;
}

void PinchGesture::setLastPoints(int x1, int y1, int x2, int y2)
{
	mLastPoints[0].x = x1;
    mLastPoints[1].x = x2;

	mLastPoints[0].y = y1;
    mLastPoints[1].y = y2;
}

void PinchGesture::setStartPoints(int x1, int y1, int x2, int y2)
{
	mStartPoints[0].x = x1;
    mStartPoints[1].x = x2;

	mStartPoints[0].y = y1;
    mStartPoints[1].y = y2;
}

float PinchGesture::calcScaleFactor()
{
	float len1 = hypot(abs(mLastPoints[0].x - mLastPoints[1].x),  abs(mLastPoints[0].y - mLastPoints[1].y));
	float len2 = hypot(abs(mPoints[0].x - mPoints[1].x),  abs(mPoints[0].y - mPoints[1].y));

	if (0 == len1) {
		return 1.0;
	}
	
	return len2 / len1;
}

float PinchGesture::calcRotationAngle()
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

float PinchGesture::calcIntersectAngle(int x, int y, int x1, int y1)
{
	return RADIAN_TO_DEGREE(acos((x * x1 + y * y1) / (hypot(x, y) * hypot(x1, y1))));
}

bool PinchGesture::copyData(GestureObject* object)
{
    if (!object) {
        return false;
    }

    if (object->getType() != getType()) {
        return false;
    }

    PinchGesture* ges = static_cast<PinchGesture*>(object);
    mState = ges->mState;
    for (int i = 0; i < 2; ++i) {
        mPoints[i] = ges->mPoints[i];
		mLastPoints[i] = ges->mLastPoints[i];
		mStartPoints[i] = ges->mStartPoints[i];
    }

    mStartCenterPoint = ges->mStartCenterPoint;
    mLastCenterPoint = ges->mLastCenterPoint;
    mCenterPoint = ges->mCenterPoint;
    mTotalScaleFactor = ges->mTotalScaleFactor;
    mLastScaleFactor = ges->mLastScaleFactor;
    mScaleFactor = ges->mScaleFactor;
    mTotalRotationAngle = ges->mTotalRotationAngle;
    mLastRotationAngle = ges->mLastRotationAngle;
    mRotationAngle = ges->mRotationAngle;

    return true;
}

void PinchGesture::clearData() {
    GestureObject::clearData();
    for (int i = 0; i < 2; ++i) {
        memset(&mPoints[i], 0 , sizeof(mPoints[i]));
		memset(&mStartPoints[i], 0 , sizeof(mStartPoints[i]));
		memset(&mLastPoints[i], 0 , sizeof(mLastPoints[i]));
    }

    mStartCenterPoint.x = 0;
    mStartCenterPoint.y = 0;
    mLastCenterPoint.x = 0;
    mLastCenterPoint.y = 0;
    mCenterPoint.x = 0;
    mCenterPoint.y = 0;
    mTotalScaleFactor = 1;
    mLastScaleFactor = 1;
    mScaleFactor = 1;
    mTotalRotationAngle = 0;
    mLastRotationAngle = 0;
    mRotationAngle = 0;
}

