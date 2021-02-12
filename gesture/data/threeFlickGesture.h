#ifndef THREEFLICKGESTURE_H
#define THREEFLICKGESTURE_H

#include "gestureObject.h"

#define THREE_FLICK_GESTURE_MAX_POINTS 4
#define THREE_FLICK_GESTURE_MIN_POINTS 3

struct MultiTouchPoint;
class ThreeFlickGesture:public GestureObject
{
public:
	ThreeFlickGesture();
	virtual ~ThreeFlickGesture();

	void setPointNum(int num);
	int getPointNum();

	void setPoint(int index, int x, int y);
	GesturePoint& getPoint(int index);

	void setStartPoint(int index, int x, int y);
	GesturePoint& getStartPoint(int index);

	void setCenter(int x, int y);
	GesturePoint& getCenter();

	void setLastCenter(int x, int y);
	GesturePoint& getLastCenter();

	void setStartCenter(int x, int y);
	GesturePoint& getStartCenter();

	void setOffset(int x, int y);
	GesturePoint& getOffset();

	void setLastOffset(int x, int y);
	GesturePoint& getLastOffset();

	void setStartOffset(int x, int y);
	GesturePoint& getStartOffset();

	virtual bool copyData(GestureObject* object);
	virtual void clearData();

	void initializeGestureData(struct MultiTouchPoint *mtPoints, unsigned int pointCount);
	void updateGestureData(struct MultiTouchPoint *mtPoints, unsigned int pointCount);

private:
	int mPointNum;
	GesturePoint mPoint[THREE_FLICK_GESTURE_MAX_POINTS];
	GesturePoint mStartPoint[THREE_FLICK_GESTURE_MAX_POINTS];
	GesturePoint mCenter;
	GesturePoint mLastCenter;
	GesturePoint mStartCenter;
	GesturePoint mOffset;
	GesturePoint mLastOffset;
	GesturePoint mStartOffset;
};

#endif /* THREEFLICKGESTURE_H */
