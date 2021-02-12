#ifndef TWOTAPGESTURERECOGNIZER_H
#define TWOTAPGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

class GestureTimer;
class TwoTapGestureRecognizer: public GestureRecognizer
{
public:
    TwoTapGestureRecognizer();
    virtual ~TwoTapGestureRecognizer();

    virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);
    virtual GestureRecognizer::ResultFlag onTimer(GestureObject *state, void *data);

private:
    enum { TwoTapGestureLength = 10,
           TwoTapGestureInterval = 200
         };
};

#endif /* TWOTAPGESTURERECOGNIZER_H */
