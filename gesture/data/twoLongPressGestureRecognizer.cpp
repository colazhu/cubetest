#include "twoLongPressGestureRecognizer.h"
#include "twoLongPressGesture.h"
#include "gestureCommonFun.h"
#include "gestureTimer.h"
#include <stdlib.h>


TwoLongPressGestureRecognizer::TwoLongPressGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_2LONGPRESS),
mTimer(0),
isNewSentence(true)
{
    mTimer = new GestureTimer(this, 0);
}

TwoLongPressGestureRecognizer::~TwoLongPressGestureRecognizer()
{
    delete mTimer;
}

GestureRecognizer::ResultFlag TwoLongPressGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
    ResultFlag retFlag = ResultFlag_Ignore;
    TwoLongPressGesture *ges = static_cast<TwoLongPressGesture *>(state);
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

    if (!mtPoints) {
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

    if(!isNewSentence) {
        if(MOTION_EVENT_ACTION_UP == touchType) {
            isNewSentence = true;
        }
        else {
            return retFlag;
        }
    }

    switch (touchType) {
    case MOTION_EVENT_ACTION_DOWN:
    case MOTION_EVENT_ACTION_POINTER_DOWN:
        {
            if(2 == pointCount) {
                timeval timeCur;
                gettimeofday(&timeCur, NULL);
                ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
                ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);
                ges->updatePressTime(timeCur);
                ges->setActionInterval(TwoLongPressGestureInterval);
                ges->setState(GESTURE_STATE_MAYBE);
                retFlag = ResultFlag_Ignore;

                mTimer->setInterval(TwoLongPressGestureInterval);
                mTimer->setUserData(data);
                mTimer->start();
            }
            else {
                if(GESTURE_STATE_MAYBE == oldState) {
                    mTimer->stop();
                    ges->clearData();
                    retFlag = ResultFlag_Ignore;
                    isNewSentence = false;
                }
                else if(GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                    ges->setState(GESTURE_STATE_FINISHED);
                    isNewSentence = false;
                    retFlag = ResultFlag_Trigger;
                }
            }
        }
        break;

    case MOTION_EVENT_ACTION_MOVE:
        {
            if (2 == pointCount) {
                GesturePoint p1 = ges->getPoint(0);
                unsigned int manhattanLength1 = abs(p1.x - mtPoints[0].coords.x) + abs(p1.y - mtPoints[0].coords.y);

                GesturePoint p2 = ges->getPoint(1);
                unsigned int manhattanLength2 = abs(p2.x - mtPoints[1].coords.x) + abs(p2.y - mtPoints[1].coords.y);
                if (TwoLongPressGestureRadius < manhattanLength1 || TwoLongPressGestureRadius < manhattanLength2) {
                    if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                        ges->setState(GESTURE_STATE_FINISHED);
                        isNewSentence = false;
                        retFlag = ResultFlag_Trigger;
                    }
                    else {
                        isNewSentence = false;
                        mTimer->stop();
                        ges->clearData();
                        retFlag = ResultFlag_Ignore;
                    }
                }
            }
        }
        break;

    case MOTION_EVENT_ACTION_UP:
        {
            if(GESTURE_STATE_MAYBE == oldState) {
                mTimer->stop();
                ges->clearData();
                retFlag = ResultFlag_Ignore;
            }
            else if(GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                ges->setState(GESTURE_STATE_FINISHED);
                retFlag = ResultFlag_Trigger;
            }

            isNewSentence = true;
        }
        break;

    case MOTION_EVENT_ACTION_POINTER_UP:
        {
            if(2 == pointCount) {
                if(GESTURE_STATE_MAYBE == oldState) {
                    isNewSentence = false;
                    mTimer->stop();
                    ges->clearData();
                    retFlag = ResultFlag_Ignore;
                }
                else if(GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                    ges->setState(GESTURE_STATE_FINISHED);
                    isNewSentence = false;
                    retFlag = ResultFlag_Trigger;
                }
            }
        }
        break;

    default:
        break;
    }

    return retFlag;
}

GestureRecognizer::ResultFlag TwoLongPressGestureRecognizer::onTimer(GestureObject *state)
{
    ResultFlag retFlag = ResultFlag_Ignore;
    TwoLongPressGesture *ges = static_cast<TwoLongPressGesture *>(state);
    if (!ges) {
        return retFlag;
    }

    GESTURE_STATE oldState = ges->getState();
    if (GESTURE_STATE_MAYBE == oldState) {
        ges->setState(GESTURE_STATE_STARTED);
        isNewSentence = true;
        retFlag = ResultFlag_Trigger;
    }

    return retFlag;
}


