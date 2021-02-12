#include "flickGestureRecognizer.h"
#include "flickGesture.h"
#include "gestureCommonFun.h"

FlickGestureRecognizer::FlickGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_FLICK)
{
}

FlickGestureRecognizer::~FlickGestureRecognizer()
{
}

GestureRecognizer::ResultFlag FlickGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint * mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
    ResultFlag retFlag = ResultFlag_Ignore;
    FlickGesture *ges = static_cast<FlickGesture *>(state);
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

    if (!mtPoints) {
        if (GESTURE_STATE_MAYBE ==oldState) {
            ges->clearData();
        }
        retFlag = ResultFlag_Ignore;
        return retFlag;
    }

    if (!GestureCommonFun::insideRegion(pointCount, mtPoints, validRegion)) {
        if (GESTURE_STATE_MAYBE ==oldState) {
            ges->clearData();
        }
        retFlag = ResultFlag_Ignore;
        return retFlag;
    }

    int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
    switch (touchType) {
    case MOTION_EVENT_ACTION_DOWN:
        {
            if (1 == pointCount) {
                ges->setState(GESTURE_STATE_MAYBE);
				ges->resetTraceArray(mtPoints[0].coords.x, mtPoints[0].coords.y, getCurEventTime());
                retFlag = ResultFlag_Ignore;
            }
        }
        break;

    case MOTION_EVENT_ACTION_MOVE:
        {
            if (GESTURE_STATE_MAYBE == oldState) {
				ges->updateTraceArray(mtPoints[0].coords.x, mtPoints[0].coords.y, getCurEventTime());
            }
         }
         break;

    case MOTION_EVENT_ACTION_POINTER_DOWN:
        {
            if (GESTURE_STATE_MAYBE == oldState) {
                ges->clearData();
                retFlag = ResultFlag_Ignore;
            }
        }
        break;

    case MOTION_EVENT_ACTION_POINTER_UP:
        {
            int pressNum = 0;
            struct MultiTouchPoint* point;
            for (int i = 0; i < pointCount; i++) {
                if (TouchPointReleased != mtPoints[i].state) {
                    if (1 > pressNum) {
                        point = &(mtPoints[i]);
                    }
                    pressNum++;
                }
            }

            if (1 == pressNum) {
                ges->updatePointerUpTime(getCurTime());
                ges->setSpeed(0);
                ges->setAngle(0);
				ges->setState(GESTURE_STATE_MAYBE);
				ges->updateTraceArray(point->coords.x, point->coords.y, getCurEventTime());
                retFlag = ResultFlag_Ignore;
            }
        }
        break;

    case MOTION_EVENT_ACTION_UP:
        {
            if (GESTURE_STATE_MAYBE == oldState) {
                if (FlickGestureActionSpeed >= ges->getSpeed() ) {
                    ges->clearData();
                    retFlag = ResultFlag_Ignore;
                }
                else {
                    if (FlickGesturePointerUpInterval <= ges->pointerUpInterval(getCurTime())) {
						ges->setPoint(mtPoints[0].coords.x, mtPoints[0].coords.y);
						ges->setState(GESTURE_STATE_FINISHED);
                        retFlag = ResultFlag_Trigger;
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    return retFlag;
}
