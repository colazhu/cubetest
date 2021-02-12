#include "dragGesture.h"
#include <string.h>

DragGesture::DragGesture():
    GestureObject(GESTURE_TYPE_DRAG)
{
    memset(&mPoint, 0, sizeof(mPoint));
    memset(&mLastPoint, 0, sizeof(mLastPoint));
    memset(&mStartPoint, 0, sizeof(mStartPoint));
}

DragGesture::~DragGesture()
{
}

bool DragGesture::copyData(GestureObject* object)
{
    if (!object) {
        return false;
    }

    if (object->getType() != getType()) {
        return false;
    }

    DragGesture* ges = static_cast<DragGesture*>(object);
    mState = ges->mState;
    mPoint = ges->mPoint;
    mStartPoint = ges->mStartPoint;
    mLastPoint = ges->mLastPoint;

    return true;
}

void DragGesture::clearData() {
    memset(&mPoint, 0 , sizeof(mPoint));
    GestureObject::clearData();
}

