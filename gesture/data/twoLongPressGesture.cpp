#include "twoLongPressGesture.h"
#include <string.h>

TwoLongPressGesture::TwoLongPressGesture():
GestureObject(GESTURE_TYPE_2LONGPRESS),
mActionInterval(0)
{
    memset(mPoint, 0, sizeof(mPoint));
    memset(&mPressTime, 0, sizeof(mPressTime));
}

TwoLongPressGesture::~TwoLongPressGesture()
{
}

void TwoLongPressGesture::setPoint(int index, int x, int y)
{
    if (2 > index && 0 <= index) {
        mPoint[index].x = x;
        mPoint[index].y = y;
    }
}

GesturePoint& TwoLongPressGesture::getPoint(int index)
{
    if (2 > index && 0 <= index) {
        return mPoint[index];
    }

    return mPoint[0];
}

bool TwoLongPressGesture::copyData(GestureObject* object)
{
    if (!object) {
        return false;
    }

    if (object->getType() != getType()) {
        return false;
    }

    TwoLongPressGesture* ges = static_cast<TwoLongPressGesture*>(object);
    mState = ges->mState;
    for(int i = 0;i < 2; ++i) {
        mPoint[i].x = ges->mPoint[i].x;
        mPoint[i].y = ges->mPoint[i].y;
    }

    mPressTime = ges->mPressTime;
    mActionInterval = ges->mActionInterval;

    return true;
}

void TwoLongPressGesture::clearData()
{
    memset(mPoint, 0, sizeof(mPoint));
    memset(&mPressTime, 0, sizeof(mPressTime));
    mActionInterval = 0;
    GestureObject::clearData();
}

void TwoLongPressGesture::updatePressTime(struct timeval& time)
{
    mPressTime = time;
}

struct timeval& TwoLongPressGesture::getPressTime()
{
    return mPressTime;
}

void TwoLongPressGesture::setActionInterval(int actionInterval)
{
    mActionInterval = actionInterval;
}

int TwoLongPressGesture::getActionInterval()
{
    return mActionInterval;
}

