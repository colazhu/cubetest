#ifndef ROTATEGESTURERECOGNIZER_H
#define ROTATEGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

#ifndef ROTATEGESTURE_H
#include "rotateGesture.h"
#endif

class RotateGestureRecognizer: public GestureRecognizer
{
    friend class RotateGesture;
public:
    RotateGestureRecognizer();
    virtual ~RotateGestureRecognizer();

    virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);

private:
    enum {
        RotateGestureTriggerAngleThreshold = 23
    };
};
#endif // ROTATEGESTURERECOGNIZER_H
