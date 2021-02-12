#include "twoDragGesture.h"
#include <string.h>

TwoDragGesture::TwoDragGesture():
GestureObject(GESTURE_TYPE_2DRAG)
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

TwoDragGesture::~TwoDragGesture()
{
}

void TwoDragGesture::setPoint(int index, int x, int y)
{
	if (0 == index || 1 == index) {
		mPoint[index].x = x;
		mPoint[index].y = y;
	}
}

GesturePoint& TwoDragGesture::getPoint(int index)
{
	if (0 == index || 1 == index) {
		return mPoint[index];
	}

	return mPoint[0];
}

void TwoDragGesture::setCenter(int x, int y)
{
	mCenter.x = x;
	mCenter.y = y;
}

GesturePoint& TwoDragGesture::getCenter()
{
	return mCenter;
}

void TwoDragGesture::setLastCenter(int x, int y)
{
	mLastCenter.x = x;
	mLastCenter.y = y;
}

GesturePoint& TwoDragGesture::getLastCenter()
{
	return mLastCenter;
}

void TwoDragGesture::setStartCenter(int x, int y)
{
	mStartCenter.x = x;
	mStartCenter.y = y;
}

GesturePoint& TwoDragGesture::getStartCenter()
{
	return mStartCenter;
}

void TwoDragGesture::setOffset(int x, int y)
{
	mOffset.x = x;
	mOffset.y = y;
}

GesturePoint& TwoDragGesture::getOffset()
{
	return mOffset;
}

void TwoDragGesture::setLastOffset(int x, int y)
{
	mLastOffset.x = x;
	mLastOffset.y = y;
}

GesturePoint& TwoDragGesture::getLastOffset()
{
	return mLastOffset;
}

void TwoDragGesture::setStartOffset(int x, int y)
{
	mStartOffset.x = x;
	mStartOffset.y = y;
}

GesturePoint& TwoDragGesture::getStartOffset()
{
	return mStartOffset;
}

bool TwoDragGesture::copyData(GestureObject* object)
{
	if (!object) {
		return false;
	}
	
	if (object->getType() != getType()) {
		return false;
	}

	TwoDragGesture* ges = static_cast<TwoDragGesture*>(object);
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

void TwoDragGesture::clearData()
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

