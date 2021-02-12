/**
 * Copyright @ 2012 - 2013 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 */
#ifndef LONGPRESSGESTURE_H
#define LONGPRESSGESTURE_H

#include "gestureObject.h"

#include <sys/time.h>
#include <time.h>

class LongPressGesture:public GestureObject
{
public:
	LongPressGesture();
	virtual ~LongPressGesture();
	void setPoint(GesturePoint& point);
	void setPoint(int x, int y);
	GesturePoint& getPoint();
	virtual bool copyData(GestureObject* object);
	virtual void clearData();

	void updatePressTime(struct timeval& time);
	struct timeval& getPressTime();
	void setActionInterval(int actionInterval);
	int getActionInterval();

private:
	GesturePoint mPoint;
	struct timeval mPressTime;
	int mActionInterval;
};

#endif /* LONGPRESSGESTURE_H */
