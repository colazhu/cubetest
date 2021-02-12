#ifndef DRAGGESTURERECOGNIZER_H
#define DRAGGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

class DragGestureRecognizer: public GestureRecognizer
{
public:
    DragGestureRecognizer();
    virtual ~DragGestureRecognizer();

    virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion,  void *data);

private:
    enum { DragGestureTriggerOffset = 20};
};

#endif // PANGESTURERECOGNIZER_H
