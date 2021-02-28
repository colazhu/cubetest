#ifndef GESTUREOBJECT_H
#define GESTUREOBJECT_H

#include "gestureDef.h"
#include "gesturePublicDef.h"
#include "Log.h"

#define PI 3.1415926

class GestureRegion
{
public:
    GestureRegion(int x = 0,
                  int y = 0,
                  unsigned int  width = 0,
                  unsigned int  height = 0):
        mX(x),
        mY(y),
        mWidth(width),
        mHeight(height)
    {

    }

    virtual ~GestureRegion(){}

    bool contain(int x, int y) {
        if ((x >= mX && x < mX+mWidth)
                && (y >= mY && y < mY+mHeight)) {
            return true;
        }
        else {
            return false;
        }
    }

    int mX;
    int mY;
    unsigned int  mWidth;
    unsigned int  mHeight;
};


class GestureObject
{
public:
    GestureObject(GESTURE_TYPE gtype);
	virtual ~GestureObject();
    inline GESTURE_TYPE getType() { return mType; }
    inline GESTURE_STATE getState() { return mState; }
    inline void setState(GESTURE_STATE state) { mState = state; }
	virtual bool copyData(GestureObject* object) = 0;
	virtual void clearData();

protected:
    GESTURE_STATE mState;
    GESTURE_TYPE  mType;
};

#endif /* GESTURERECOGNIZER_H */
