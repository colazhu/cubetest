/**
 * Copyright @ 2012 - 2013 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 */

#ifndef SCRATCHGESTURERECOGNIZER_H
#define SCRATCHGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

#include <vector>

class ScratchGestureRecognizer: public GestureRecognizer
{
private:
	#ifndef RADIAN_TO_DEGREE
	#define RADIAN_TO_DEGREE(x) (x) < 0 ? (360 + 57.2958*(x)) : (57.2958*(x))
	#endif
	#ifndef SCRATCH_GESTURE_MAX_POINTS
	#define SCRATCH_GESTURE_MAX_POINTS 5
	#endif
	struct deserialize_point_info {
		int index;
		int x;
		int y;
		int angle;
	};

private:
	bool vertexDeserialize(int n, struct MultiTouchPoint* p, std::vector<struct deserialize_point_info>& outVector);
	inline int cross(const deserialize_point_info &A, const deserialize_point_info &B, const deserialize_point_info &C);
	bool insidePloygen(int x, int y, std::vector<struct deserialize_point_info> inVector);
	int calcPolygonArea(std::vector<struct deserialize_point_info> inVector);
	bool isMultiDrag(std::vector<struct deserialize_point_info> inVector);
	inline float interectionAngle(int x1, int y1, int x2, int y2);
private:
	std::vector<struct deserialize_point_info> mStartInfoVector;
	std::vector<struct deserialize_point_info> mLastInfoVector;
	int mStartPolygonArea;
	int mLastPolygonArea;
	int mStrachNum;

public:
	ScratchGestureRecognizer();
	virtual ~ScratchGestureRecognizer();
	virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
		struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);

private:
	enum { ScratchGestureRecognizeValidTime = 200000 };	//millisecond
	bool mIsNewSentence;
};

#endif /* SCRATCHGESTURERECOGNIZER_H */
