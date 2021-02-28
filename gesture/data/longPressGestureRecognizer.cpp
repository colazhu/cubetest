#include "longPressGestureRecognizer.h"
#include "longPressGesture.h"
#include "gestureCommonFun.h"
#include "gestureTimer.h"
#include <stdlib.h>

LongPressGestureRecognizer::LongPressGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_LONGPRESS),
mTimer(0)
{
	mTimer = new GestureTimer(this, 0);
}

LongPressGestureRecognizer::~LongPressGestureRecognizer()
{
	delete mTimer;
}

GestureRecognizer::ResultFlag LongPressGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
	ResultFlag retFlag = ResultFlag_Ignore;
	LongPressGesture *ges = static_cast<LongPressGesture *>(state);
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
			mTimer->stop();
			ges->clearData();
			return retFlag;
		}
		else {
			return retFlag;
		}
	}

	if (!mtPoints || 1 != pointCount) {
		if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
			ges->setState(GESTURE_STATE_FINISHED);
			retFlag = ResultFlag_Trigger;
			return retFlag;
		}
		else if (GESTURE_STATE_MAYBE == oldState) {
			mTimer->stop();
		}

		ges->clearData();
		return retFlag;
	}

	if (!GestureCommonFun::insideRegion(pointCount, mtPoints, validRegion)) {
		if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
			ges->setState(GESTURE_STATE_FINISHED);
			retFlag = ResultFlag_Trigger;
			return retFlag;
		}
		else if (GESTURE_STATE_MAYBE == oldState) {
			mTimer->stop();
		}
		
		ges->clearData();
		return retFlag;
	}

	int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
	switch (touchType) {
	case MOTION_EVENT_ACTION_DOWN:
		{
            timeval timeCur;
            gettimeofday(&timeCur, NULL);
			ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
			ges->updatePressTime(timeCur);
			ges->setActionInterval(LongPressGestureInterval);
			ges->setState(GESTURE_STATE_MAYBE);
			retFlag = ResultFlag_Ignore;
			
			mTimer->setInterval(LongPressGestureInterval);
			mTimer->setUserData(data);
			mTimer->start();
            LOG_BASE("LongPressGestureRecognizer::TimerStart");
		}
		break;
								
	case MOTION_EVENT_ACTION_MOVE:
		{
			if (GESTURE_STATE_MAYBE == oldState || GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
				GesturePoint p1 = ges->getPoint();
				unsigned int manhattanLength = abs(p1.x - mtPoints[0].coords.x) + abs(p1.y - mtPoints[0].coords.y);				 

				if (LongPressGestureRadius < manhattanLength) {
					if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
						ges->setState(GESTURE_STATE_FINISHED);
						retFlag = ResultFlag_Trigger;
					}
					else {
						mTimer->stop();
						ges->clearData();
						retFlag = ResultFlag_Ignore;
					}
				}
			}
		}
		break;
		
	case MOTION_EVENT_ACTION_UP:	
	case MOTION_EVENT_ACTION_POINTER_DOWN:
		{
			if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
				ges->setState(GESTURE_STATE_FINISHED);
				retFlag = ResultFlag_Trigger;
			}
			else if (GESTURE_STATE_MAYBE == oldState) {
				mTimer->stop();
				ges->clearData();
				retFlag = ResultFlag_Ignore;
			}
		}
		break;
		
	case MOTION_EVENT_ACTION_POINTER_UP:
	default:
		break;
	}

	return retFlag;
}

GestureRecognizer::ResultFlag LongPressGestureRecognizer::onTimer(GestureObject *state)
{
    LOG_BASE("LongPressGestureRecognizer::onTimer");
	ResultFlag retFlag = ResultFlag_Ignore;
	LongPressGesture *ges = static_cast<LongPressGesture *>(state);
	if (!ges) {
		return retFlag;
	}

	GESTURE_STATE oldState = ges->getState();
	if (GESTURE_STATE_MAYBE == oldState) {
		ges->setState(GESTURE_STATE_STARTED);
		retFlag = ResultFlag_Trigger;
	}

	return retFlag;
}


