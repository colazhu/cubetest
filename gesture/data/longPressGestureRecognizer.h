/**
 * Copyright @ 2012 - 2013 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 */

#ifndef LONGPRESSGESTURERECOGNIZER_H
#define LONGPRESSGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

class GestureTimer;
class LongPressGestureRecognizer: public GestureRecognizer
{
public:
	LongPressGestureRecognizer();
	virtual ~LongPressGestureRecognizer();

	virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
		struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);
	virtual GestureRecognizer::ResultFlag onTimer(GestureObject *state);

private:
	enum { LongPressGestureRadius = 30 }; 	//pixel
	enum { LongPressGestureInterval = 1000 };	//millisecond

	GestureTimer* mTimer;
};

#endif /* LONGPRESSGESTURERECOGNIZER_H */
