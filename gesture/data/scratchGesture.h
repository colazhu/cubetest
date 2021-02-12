#ifndef SCRATCHGESTURE_H
#define SCRATCHGESTURE_H

#include "gestureObject.h"

#ifndef SCRATCHGESTURERECOGNIZER_H
#   include "scratchGestureRecognizer.h"
#endif

#define  SCRATCH_GESTURE_MAX_POINTS 5
#define  SCRATCH_GESTURE_MIN_POINTS 4

struct MultiTouchPoint;
class ScratchGesture : public GestureObject
{
	friend class ScratchGestureRecognizer;
public:
	ScratchGesture();
	virtual ~ScratchGesture();

	void setPoint(int index, int x, int y);
	GesturePoint& getPoint(int index);
	void getPoints(GesturePoint* points, int maxNum, int* num);

    void setStartPoint(int index, int x, int y);
    GesturePoint& getStartPoint(int index);

	void setPointNum(int num);
	int getPointNum();
	
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

private:
	void updateGestureData(int pointCount, struct MultiTouchPoint *mtPoints, bool isFirstInitialize);

private:
	int mPointNum;
	GesturePoint mPoint[SCRATCH_GESTURE_MAX_POINTS];
    GesturePoint mStartPoint[SCRATCH_GESTURE_MAX_POINTS];
	GesturePoint mCenter;
	GesturePoint mLastCenter;
	GesturePoint mStartCenter;
	GesturePoint mOffset;
	GesturePoint mLastOffset;
	GesturePoint mStartOffset;
};

#endif /* SCRATCHGESTURE_H */
