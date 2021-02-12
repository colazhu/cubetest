#ifndef TAPGESTURE_H
#define TAPGESTURE_H

#include "gestureObject.h"

class TapGesture:public GestureObject
{
public:
	TapGesture();
	virtual ~TapGesture();
	void setPoint(GesturePoint& point);
	void setPoint(int x, int y);
	void setStartPoint(int x, int y);
	void updateTime(unsigned long time);
	GesturePoint& getStartPoint();
	GesturePoint& getPoint();
	unsigned long getTime();
	virtual bool copyData(GestureObject* object);
	virtual void clearData();

private:
	GesturePoint mPoint;
	GesturePoint mStartPoint;
	unsigned long mTime;
};

#endif /* TAPGESTURE_H */
