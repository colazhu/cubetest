#ifndef TWOLONGPRESSGESTURERECOGNIZER_H
#define TWOLONGPRESSGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

class GestureTimer;
class TwoLongPressGestureRecognizer: public GestureRecognizer
{
public:
    TwoLongPressGestureRecognizer();
    virtual ~TwoLongPressGestureRecognizer();

    virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);
    virtual GestureRecognizer::ResultFlag onTimer(GestureObject *state);

private:
    enum { TwoLongPressGestureRadius = 30 }; 	//pixel
    enum { TwoLongPressGestureInterval = 1000 };	//millisecond

    GestureTimer* mTimer;
    bool isNewSentence;
};

#endif // TWOLONGPRESSGESTURERECOGNIZER_H
