#include "twoTapGesture.h"
#include <string.h>

TwoTapGesture::TwoTapGesture():
GestureObject(GESTURE_TYPE_2TAP)
{
    memset(&mPoint[0], 0, sizeof(mPoint[0]));
    memset(&mPoint[1], 0, sizeof(mPoint[1]));

    memset(&mCenter, 0, sizeof(mCenter));
    memset(&mLastCenter, 0, sizeof(mLastCenter));
    memset(&mStartCenter, 0, sizeof(mStartCenter));

    memset(&mOffset, 0, sizeof(mOffset));
    memset(&mLastOffset, 0, sizeof(mLastOffset));
    memset(&mStartOffset, 0, sizeof(mStartOffset));
}

TwoTapGesture::~TwoTapGesture()
{
}

void TwoTapGesture::setPoint(int index, int x, int y)
{
    if (0 == index || 1 == index) {
        mPoint[index].x = x;
        mPoint[index].y = y;
    }
}

GesturePoint& TwoTapGesture::getPoint(int index)
{
    if (0 == index || 1 == index) {
        return mPoint[index];
    }

    return mPoint[0];
}

void TwoTapGesture::setCenter(int x, int y)
{
    mCenter.x = x;
    mCenter.y = y;
}

GesturePoint& TwoTapGesture::getCenter()
{
    return mCenter;
}

void TwoTapGesture::setLastCenter(int x, int y)
{
    mLastCenter.x = x;
    mLastCenter.y = y;
}

GesturePoint& TwoTapGesture::getLastCenter()
{
    return mLastCenter;
}

void TwoTapGesture::setStartCenter(int x, int y)
{
    mStartCenter.x = x;
    mStartCenter.y = y;
}

GesturePoint& TwoTapGesture::getStartCenter()
{
    return mStartCenter;
}

void TwoTapGesture::setOffset(int x, int y)
{
    mOffset.x = x;
    mOffset.y = y;
}

GesturePoint& TwoTapGesture::getOffset()
{
    return mOffset;
}

void TwoTapGesture::setLastOffset(int x, int y)
{
    mLastOffset.x = x;
    mLastOffset.y = y;
}

GesturePoint& TwoTapGesture::getLastOffset()
{
    return mLastOffset;
}

void TwoTapGesture::setStartOffset(int x, int y)
{
    mStartOffset.x = x;
    mStartOffset.y = y;
}

GesturePoint& TwoTapGesture::getStartOffset()
{
    return mStartOffset;
}

bool TwoTapGesture::copyData(GestureObject* object)
{
    if (!object) {
        return false;
    }
    
    if (object->getType() != getType()) {
        return false;
    }

    TwoTapGesture* ges = static_cast<TwoTapGesture*>(object);
    mState = ges->mState;
    
    mPoint[0] = ges->mPoint[0];
    mPoint[1] = ges->mPoint[1];
    
    mCenter = ges->mCenter;
    mLastCenter = ges->mLastCenter;
    mStartCenter = ges->mStartCenter;
    
    mOffset = ges->mOffset;
    mLastOffset = ges->mLastOffset;
    mStartOffset = ges->mStartOffset;

    return true;
}

void TwoTapGesture::clearData()
{
    memset(&mPoint[0], 0, sizeof(mPoint[0]));
    memset(&mPoint[1], 0, sizeof(mPoint[1]));

    memset(&mCenter, 0, sizeof(mCenter));
    memset(&mLastCenter, 0, sizeof(mLastCenter));
    memset(&mStartCenter, 0, sizeof(mStartCenter));

    memset(&mOffset, 0, sizeof(mOffset));
    memset(&mLastOffset, 0, sizeof(mLastOffset));
    memset(&mStartOffset, 0, sizeof(mStartOffset));
    
    GestureObject::clearData();
}


