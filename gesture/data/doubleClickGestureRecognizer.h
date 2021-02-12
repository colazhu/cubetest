#ifndef DOUBLECLICKGESTURERECOGNIZER_H
#define DOUBLECLICKGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

class DoubleClickGestureRecognizer: public GestureRecognizer
{
public:
	DoubleClickGestureRecognizer();
	virtual ~DoubleClickGestureRecognizer();

	virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);

private:
    enum {
        DoubleClickGestureRadius = 40, //pixel
        DoubleClickGestureInterval = 500 //millisecond
    };
};

#endif /* DOUBLECLICKGESTURERECOGNIZER_H */
