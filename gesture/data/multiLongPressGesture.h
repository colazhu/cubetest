#ifndef MULTILONGPRESSGESTURE_H
#define MULTILONGPRESSGESTURE_H

#include "gestureObject.h"

#include <sys/time.h>
#include <time.h>

#define  MULTI_LONGPRESS_GESTURE_MAX_POINTS 5

class MultiLongPressGesture:public GestureObject
{
public:
    MultiLongPressGesture();
    virtual ~MultiLongPressGesture();
    void setPoint(int index, int x, int y);
    GesturePoint& getPoint(int index);
    void getPoints(GesturePoint* points, int maxNum, int* num);

    void setPointNum(int num);
    int getPointNum();

    virtual bool copyData(GestureObject* object);
    virtual void clearData();

    void updatePressTime(struct timeval& time);
    struct timeval& getPressTime();
    void setActionInterval(int actionInterval);
    int getActionInterval();

private:
    GesturePoint mPoint[5];
    struct timeval mPressTime;
    int mActionInterval;
    int mPointNum;
};

#endif // MULTILONGPRESSGESTURE_H
