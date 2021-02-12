/**
 * Copyright @ 2012 - 2013 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 */
#ifndef TWODRAGGESTURERECOGNIZER_H
#define TWODRAGGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

class TwoDragGestureRecognizer: public GestureRecognizer
{
public:
	TwoDragGestureRecognizer();
	virtual ~TwoDragGestureRecognizer();

	virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
		struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);

private:
	enum { TwoFingersDistance = 800 };
	enum { TwoDragGestureTriggerOffset = 20 };
};

#endif /* TWODRAGGESTURERECOGNIZER_H */
