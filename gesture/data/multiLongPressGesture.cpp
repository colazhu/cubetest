#include "multiLongPressGesture.h"
#include <string.h>

MultiLongPressGesture::MultiLongPressGesture():
GestureObject(GESTURE_TYPE_MULTILONGPRESS),
mActionInterval(0),
mPointNum(0)
{
    memset(mPoint, 0, sizeof(mPoint));
    memset(&mPressTime, 0, sizeof(mPressTime));
}

MultiLongPressGesture::~MultiLongPressGesture()
{
}

void MultiLongPressGesture::setPoint(int index, int x, int y)
{
    if (MULTI_LONGPRESS_GESTURE_MAX_POINTS > index && 0 <= index) {
        mPoint[index].x = x;
        mPoint[index].y = y;
    }
}

GesturePoint& MultiLongPressGesture::getPoint(int index)
{
    if (MULTI_LONGPRESS_GESTURE_MAX_POINTS > index && 0 <= index) {
        return mPoint[index];
    }

    return mPoint[0];
}

void MultiLongPressGesture::getPoints(GesturePoint* points, int maxNum, int* num)
{
    *num = maxNum < mPointNum? maxNum:mPointNum;
    for (int i = 0; i < *num; i++) {
        mPoint[i] = points[i];
    }
}

void MultiLongPressGesture::setPointNum(int num)
{
    mPointNum = num;
}

int MultiLongPressGesture::getPointNum()
{
    return mPointNum;
}

bool MultiLongPressGesture::copyData(GestureObject* object)
{
    if (!object) {
        return false;
    }

    if (object->getType() != getType()) {
        return false;
    }

    MultiLongPressGesture* ges = static_cast<MultiLongPressGesture*>(object);
    mState = ges->mState;
    for(int i = 0;i < MULTI_LONGPRESS_GESTURE_MAX_POINTS; ++i) {
        mPoint[i].x = ges->mPoint[i].x;
        mPoint[i].y = ges->mPoint[i].y;
    }

    mPressTime = ges->mPressTime;
    mActionInterval = ges->mActionInterval;

    return true;
}

void MultiLongPressGesture::clearData()
{
    memset(mPoint, 0, sizeof(mPoint));
    memset(&mPressTime, 0, sizeof(mPressTime));
    mActionInterval = 0;
    GestureObject::clearData();
}

void MultiLongPressGesture::updatePressTime(struct timeval& time)
{
    mPressTime = time;
}

struct timeval& MultiLongPressGesture::getPressTime()
{
    return mPressTime;
}

void MultiLongPressGesture::setActionInterval(int actionInterval)
{
    mActionInterval = actionInterval;
}

int MultiLongPressGesture::getActionInterval()
{
    return mActionInterval;
}

