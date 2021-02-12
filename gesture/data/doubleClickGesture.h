#ifndef DOUBLECLICKGESTURE_H
#define DOUBLECLICKGESTURE_H

#include "gestureObject.h"

class DoubleClickGesture:public GestureObject
{
public:
	DoubleClickGesture();
	virtual ~DoubleClickGesture();

    virtual bool copyData(GestureObject* object);
    virtual void clearData();

    inline void setPoint1(int x, int y) {mPoint1.x = x; mPoint1.y = y;}
    inline void setPoint2(int x, int y) {mPoint2.x = x; mPoint2.y = y;}
    inline GesturePoint& getPoint1() {return mPoint1;}
    inline GesturePoint& getPoint2() {return mPoint2;}
    inline void updateTime1(unsigned long time) { mTime1 = time; }
    inline void updateTime2(unsigned long time) { mTime2 = time; }
    inline unsigned long getTime1() { return mTime1; }
    inline unsigned long getTime2() { return mTime2; }

private:
	GesturePoint mPoint1;
	GesturePoint mPoint2;
	unsigned long mTime1;
	unsigned long mTime2;
};

#endif /* DOUBLECLICKGESTURE_H */
