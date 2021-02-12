#include "doubleClickGesture.h"
#include <string.h>

DoubleClickGesture::DoubleClickGesture():
GestureObject(GESTURE_TYPE_DOUBLECLICK)
{
	memset(&mPoint1, 0 , sizeof(mPoint1));
	memset(&mPoint2, 0 , sizeof(mPoint2));
	memset(&mTime1, 0 , sizeof(mTime1));
	memset(&mTime2, 0 , sizeof(mTime2));
}

DoubleClickGesture::~DoubleClickGesture()
{
}

void DoubleClickGesture::clearData()
{
    memset(&mPoint1, 0, sizeof(mPoint1));
    memset(&mPoint2, 0, sizeof(mPoint2));
    memset(&mTime1, 0, sizeof(mTime1));
    memset(&mTime2, 0, sizeof(mTime2));
	
	GestureObject::clearData();
}

bool DoubleClickGesture::copyData(GestureObject* object)
{
	if (!object) {
		return false;
	}
	
	if (object->getType() != getType()) {
		return false;
	}

	DoubleClickGesture* ges = static_cast<DoubleClickGesture*>(object);
	mState = ges->mState;
	mPoint1 = ges->mPoint1;
	mPoint2 = ges->mPoint2;
	mTime1 = ges->mTime1;
	mTime2 = ges->mTime2;

	return true;
}



