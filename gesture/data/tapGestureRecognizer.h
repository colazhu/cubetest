/**
 * Copyright @ 2012 - 2013 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 */

#ifndef TAPGESTURERECOGNIZER_H
#define TAPGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

class TapGestureRecognizer: public GestureRecognizer
{
public:
	TapGestureRecognizer();
	virtual ~TapGestureRecognizer();

	virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
		struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);

private:
	enum { TapGestureRadius = 30 };
	enum { TapGestureInterval = 200 };
};

#endif /* TAPGESTURERECOGNIZER_H */
