/**
 * Copyright @ 2012 - 2013 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 */
#ifndef THREEFLICKGESTURERECOGNIZER_H
#define THREEFLICKGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

#define INVALID_ANGLE -1
#include <vector>

class ThreeFlickGestureRecognizer: public GestureRecognizer
{
private:
	#ifndef RADIAN_TO_DEGREE
	#define RADIAN_TO_DEGREE(x) (x) < 0 ? (360 + 57.2958*(x)) : (57.2958*(x))
	#endif
	#ifndef INTERSECTION_ANGLE 
	#define INTERSECTION_ANGLE(x) (x) < 180 ? (x) : (360 - (x))
	#endif
	
	struct threeflick_point_info {
		float distance;
		float angle;
	};
public:
	ThreeFlickGestureRecognizer();
	virtual ~ThreeFlickGestureRecognizer();

	virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
		struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);
	
	inline float intersect(int x1, int y1, int x2, int y2);

private:
	float intersectBetweenFingers(GestureObject* ges, struct MultiTouchPoint *mtPoints, unsigned int pointCount);
private:
	std::vector<struct threeflick_point_info> mStartInfoVector;
	enum { ThreeFlickGestureStartRadius = 15 };
	enum { ThreeFlickGestureAngleThreshold = 60 };
	enum { ThreeFlickGestureRecognizeValidTime = 200000 };	//millisecond
	bool mIsNewSentence;
};

#endif /* THREEFLICKGESTURERECOGNIZER_H */
