#ifndef PINCHGESTURERECOGNIZER_H
#define PINCHGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

#ifndef PINCHGESTURE_H
#include "pinchGesture.h"
#endif

class PinchGestureRecognizer: public GestureRecognizer
{
    friend class PinchGesture;
public:
    PinchGestureRecognizer();
    virtual ~PinchGestureRecognizer();

    virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);

private:
    enum { PinchGestureFingerOffset = 10 };
	enum { PinchGestureTriggerAngleThreshold = 30 };
};
#endif // PINCHGESTURERECOGNIZER_H
