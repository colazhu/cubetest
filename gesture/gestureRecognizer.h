#ifndef GESTURERECOGNIZER_H
#define GESTURERECOGNIZER_H
#include "gestureDef.h"
#include "gesturePublicDef.h"
#include "gestureObject.h"

typedef unsigned int RecognizeResultFlag;

class GestureRecognizer
{
public:
	enum ResultFlag
    {
        ResultFlag_Ignore               = 0x0000,
        ResultFlag_Trigger              = 0x0001,
        ResultFlag_StartAndFinishTrigger= 0x0002,
    };

	GestureRecognizer(GESTURE_TYPE gtype);
	virtual ~GestureRecognizer();    
    virtual ResultFlag recognize(GestureObject *state,
                                 int motionEventAction,
                                 struct MultiTouchPoint *mtPoints,
                                 int pointCount,
                                 GestureRegion* validRegion,
                                 void *data) = 0;
    virtual ResultFlag onTimer(GestureObject *state) {return ResultFlag_Ignore;}

    GESTURE_TYPE getType() { return mType; }
    inline bool isIgnore() {return mIgnore;}
    inline void setIgnore(bool ignore) {mIgnore = ignore;}
    inline void setPressTime(unsigned long time) {mPressTime = time;}
    inline unsigned long getPressTime() {return mPressTime;}
    inline void setCurEventTime(unsigned long time) {mCurEventTime = time;}
    inline unsigned long getCurEventTime() {return mCurEventTime;}
	unsigned long getCurTime();

protected:
    bool            mIgnore;
    GESTURE_TYPE    mType;
    unsigned long   mPressTime;
    unsigned long   mCurEventTime;
};

#endif /* GESTURERECOGNIZER_H */
