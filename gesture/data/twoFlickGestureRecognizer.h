/**
 * Copyright @ 2012 - 2013 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 */
#ifndef TWOFLICKGESTURERECOGNIZER_H
#define TWOFLICKGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

class TwoFlickGestureRecognizer: public GestureRecognizer
{
public:
	TwoFlickGestureRecognizer();
	virtual ~TwoFlickGestureRecognizer();

	virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
		struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);

private:
	enum { TwoFingersDistance = 300 };	//pixel
    enum { TwoFlickGesturePointerUpInterval = 100000 };	//microsecond
	enum { TwoFlickGestureActionSpeed = 100 };	//pixel/100ms
};

#endif /* TWOFLICKGESTURERECOGNIZER_H */
