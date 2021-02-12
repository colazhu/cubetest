#include "gestureObject.h"

GestureObject::GestureObject(GESTURE_TYPE gtype):
mState(GESTURE_STATE_INVALID),
mType(gtype)
{
}

GestureObject::~GestureObject()
{
}

void GestureObject::clearData()
{
	mState = GESTURE_STATE_INVALID;
}
