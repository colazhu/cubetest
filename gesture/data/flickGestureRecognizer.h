#ifndef FLICKGESTURERECOGNIZER_H
#define FLICKGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

#ifndef FLICKGESTURE_H
#include "flickGesture.h"
#endif

class FlickGestureRecognizer: public GestureRecognizer
{
    friend class flickGesture;
public:
    FlickGestureRecognizer();
    virtual ~FlickGestureRecognizer();

    virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);

private:
    enum { FlickGestureActionSpeed = 10 };
    enum { FlickGesturePointerUpInterval = 800000 };//800ms
};
#endif // FLICKGESTURERECOGNIZER_H
