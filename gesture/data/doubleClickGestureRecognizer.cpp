#include "doubleClickGestureRecognizer.h"
#include "doubleClickGesture.h"
#include "gestureCommonFun.h"
#include <stdlib.h>

DoubleClickGestureRecognizer::DoubleClickGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_DOUBLECLICK)
{
}

DoubleClickGestureRecognizer::~DoubleClickGestureRecognizer()
{
}

GestureRecognizer::ResultFlag DoubleClickGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
	ResultFlag retFlag = ResultFlag_Ignore;
	DoubleClickGesture *ges = static_cast<DoubleClickGesture *>(state);
	if (!ges) {
		return retFlag;
	}

    GESTURE_STATE oldState = ges->getState();
	if (isIgnore()) {
		if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
			ges->setState(GESTURE_STATE_FINISHED);
			return ResultFlag_Trigger;
		}
		else if (GESTURE_STATE_MAYBE == oldState) {
			ges->clearData();
			return retFlag;
		}
		else {
			return retFlag;
		}
	}

	if (!mtPoints || 1 != pointCount) {
		ges->clearData();
		return retFlag;
	}

	if (!GestureCommonFun::insideRegion(pointCount, mtPoints, validRegion)) {
		ges->clearData();
		return retFlag;
	}

    int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
	switch (touchType) {
    case MOTION_EVENT_ACTION_DOWN:
		{
			if (GESTURE_STATE_MAYBE == oldState) {
				 GesturePoint p1 = ges->getPoint1();
				 unsigned int manhattanLength = abs(p1.x - mtPoints[0].coords.x) + abs(p1.y - mtPoints[0].coords.y);				 
				 unsigned long interval = getCurEventTime() - ges->getTime1();

				 if (DoubleClickGestureRadius < manhattanLength || DoubleClickGestureInterval < interval) {
				 	ges->setPoint1(mtPoints[0].coords.x, mtPoints[0].coords.y);
					ges->updateTime1(getCurEventTime());
				 	ges->setState(GESTURE_STATE_MAYBE);
					retFlag = ResultFlag_Ignore;
				 }
				 else {
				 	ges->setPoint2(mtPoints[0].coords.x, mtPoints[0].coords.y);
					ges->updateTime2(getCurEventTime());
					ges->setState(GESTURE_STATE_FINISHED);
					retFlag = ResultFlag_Trigger;
				 }
			}
			else {
				ges->setPoint1(mtPoints[0].coords.x, mtPoints[0].coords.y);
				ges->updateTime1(getCurEventTime());
				ges->setState(GESTURE_STATE_MAYBE);
				retFlag = ResultFlag_Ignore;
			}
		}
		break;
								
    case MOTION_EVENT_ACTION_MOVE:
    case MOTION_EVENT_ACTION_UP:
		{
			if (GESTURE_STATE_MAYBE == oldState) {
				GesturePoint p1 = ges->getPoint1();
				unsigned int manhattanLength = abs(p1.x - mtPoints[0].coords.x) + abs(p1.y - mtPoints[0].coords.y);				 
				unsigned long interval = getCurEventTime() - ges->getTime1();

				if (DoubleClickGestureRadius < manhattanLength || DoubleClickGestureInterval < interval) {
					ges->clearData();
					retFlag = ResultFlag_Ignore;
				}
			}
		}
		break;	
    case MOTION_EVENT_ACTION_POINTER_DOWN:
    case MOTION_EVENT_ACTION_POINTER_UP:
	default:
		break;
	}

	return retFlag;
}

