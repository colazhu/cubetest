#ifndef TWOLONGPRESSGESTURE_H
#define TWOLONGPRESSGESTURE_H

#include "gestureObject.h"

#include <sys/time.h>
#include <time.h>

class TwoLongPressGesture:public GestureObject
{
public:
    TwoLongPressGesture();
    virtual ~TwoLongPressGesture();
    void setPoint(int index, int x, int y);
    GesturePoint& getPoint(int index);
    virtual bool copyData(GestureObject* object);
    virtual void clearData();

    void updatePressTime(struct timeval& time);
    struct timeval& getPressTime();
    void setActionInterval(int actionInterval);
    int getActionInterval();

private:
    GesturePoint mPoint[2];
    struct timeval mPressTime;
    int mActionInterval;
};


#endif // TWOLONGPRESSGESTURE_H
