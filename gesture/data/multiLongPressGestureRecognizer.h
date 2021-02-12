/**
 * Copyright @ 2012 - 2013 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 */

#ifndef MULTILONGPRESSGESTURERECOGNIZER_H
#define MULTILONGPRESSGESTURERECOGNIZER_H

#include "gestureRecognizer.h"

class GestureTimer;
class MultiLongPressGestureRecognizer: public GestureRecognizer
{
public:
    MultiLongPressGestureRecognizer();
    virtual ~MultiLongPressGestureRecognizer();

    virtual GestureRecognizer::ResultFlag recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data);
    virtual GestureRecognizer::ResultFlag onTimer(GestureObject *state);

private:
    enum { MultiLongPressGestureRadius = 30 }; 	//pixel
    enum { MultiLongPressGestureInterval = 1000 };	//millisecond
    enum { MultiLongPressGestureRecognizeValidTime = 200000 };	//millisecond

    GestureTimer* mTimer;
    bool mIsNewSentence;
};


#endif // MULTILONGPRESSGESTURERECOGNIZER_H
