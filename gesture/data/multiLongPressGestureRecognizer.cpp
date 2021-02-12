#include "multiLongPressGestureRecognizer.h"
#include "multiLongPressGesture.h"
#include "gestureCommonFun.h"
#include "gestureTimer.h"
#include <stdlib.h>

MultiLongPressGestureRecognizer::MultiLongPressGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_MULTILONGPRESS),
mTimer(0),
mIsNewSentence(true)
{
    mTimer = new GestureTimer(this, 0);
}

MultiLongPressGestureRecognizer::~MultiLongPressGestureRecognizer()
{
    delete mTimer;
}

GestureRecognizer::ResultFlag MultiLongPressGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
    ResultFlag retFlag = ResultFlag_Ignore;
    MultiLongPressGesture *ges = static_cast<MultiLongPressGesture *>(state);
    if (!ges) {
        return retFlag;
    }

    GESTURE_STATE oldState = ges->getState();
	if ((GESTURE_STATE_INVALID == oldState 
		|| GESTURE_STATE_CANCECLED == oldState
		|| GESTURE_STATE_FINISHED == oldState)
		&& !isIgnore()) {
		if (MultiLongPressGestureRecognizeValidTime < (getCurTime() - getPressTime())) {
			setIgnore(true);
		}
	}
	
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

	if(MOTION_EVENT_ACTION_DOWN != touchType
		&& !mIsNewSentence) {
		return retFlag;
	}

    switch (touchType) {
    case MOTION_EVENT_ACTION_DOWN:
    case MOTION_EVENT_ACTION_POINTER_DOWN:
        {
			if(MOTION_EVENT_ACTION_DOWN == touchType) {
				ges->setState(GESTURE_STATE_INVALID);
				mIsNewSentence = true;
			}

            if(3 <= pointCount) {
                if(GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                    mIsNewSentence = false;
                    ges->setState(GESTURE_STATE_FINISHED);
                    retFlag = ResultFlag_Trigger;
                }
                else {
                    int num = pointCount > MULTI_LONGPRESS_GESTURE_MAX_POINTS ? MULTI_LONGPRESS_GESTURE_MAX_POINTS : pointCount;
                    ges->setPointNum(num);
                    for(int i = 0;i < num; ++i) {
                        ges->setPoint(i, mtPoints[i].coords.x, mtPoints[i].coords.y);
                    }

                    timeval timeCur;
                    gettimeofday(&timeCur, NULL);
                    ges->updatePressTime(timeCur);
                    ges->setActionInterval(MultiLongPressGestureInterval);
                    ges->setState(GESTURE_STATE_MAYBE);

                    retFlag = ResultFlag_Ignore;

                    mTimer->setInterval(MultiLongPressGestureInterval);
                    mTimer->setUserData(data);
                    mTimer->start();
                }
            }
        }
        break;

    case MOTION_EVENT_ACTION_MOVE:
        {
            if (3 <= pointCount) {
                bool isExceedRange = false;
                for(int i = 0;i < pointCount; ++i) {
                    if(MultiLongPressGestureRadius < abs(ges->getPoint(i).x - mtPoints[i].coords.x) + abs(ges->getPoint(i).y - mtPoints[i].coords.y)) {
                        isExceedRange = true;
                        break;
                    }
                }

                if(isExceedRange) {
                    if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                        ges->setState(GESTURE_STATE_FINISHED);
                        mIsNewSentence = false;
                        retFlag = ResultFlag_Trigger;
                    }
                    else if(GESTURE_STATE_MAYBE == oldState) {
                        mIsNewSentence = false;
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
            else if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                ges->setState(GESTURE_STATE_FINISHED);
                retFlag = ResultFlag_Trigger;
            }
            else {
                retFlag = ResultFlag_Ignore;
            }

            mIsNewSentence = true;
        }
        break;

    case MOTION_EVENT_ACTION_POINTER_UP:
        {
            int pressNum = 0;
            int num = pointCount > MULTI_LONGPRESS_GESTURE_MAX_POINTS ? MULTI_LONGPRESS_GESTURE_MAX_POINTS : pointCount;
            for(int i = 0;i < num; ++i) {
                if(TouchPointReleased != mtPoints[i].state) {
                    pressNum++;
                }
            }

            if(3 > pressNum) {
                if(GESTURE_STATE_MAYBE == oldState) {
                    mIsNewSentence = false;
                    mTimer->stop();
                    ges->clearData();
                    retFlag = ResultFlag_Ignore;
                }
                else if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                    mIsNewSentence = false;
                    ges->setState(GESTURE_STATE_FINISHED);
                    retFlag = ResultFlag_Trigger;
                }
                else {
                    retFlag = ResultFlag_Ignore;
                }
            }

            else {
                if(GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                    mIsNewSentence = false;
                    ges->setState(GESTURE_STATE_FINISHED);
                    retFlag = ResultFlag_Trigger;
                }
                else {
                    int index = 0;
                    for(int i = 0;i < num; ++i) {
                        if(TouchPointReleased != mtPoints[i].state) {
                            ges->setPoint(index++, mtPoints[i].coords.x, mtPoints[i].coords.y);
                        }
                    }

                    if(pressNum != index) {
                    }

                    ges->setPointNum(pressNum);
                    timeval timeCur;
                    gettimeofday(&timeCur, NULL);
                    ges->updatePressTime(timeCur);
                    ges->setActionInterval(MultiLongPressGestureInterval);
                    ges->setState(GESTURE_STATE_MAYBE);

                    retFlag = ResultFlag_Ignore;

                    mTimer->setInterval(MultiLongPressGestureInterval);
                    mTimer->setUserData(data);
                    mTimer->start();
                }
            }
        }
        break;

    default:
        break;
    }

    return retFlag;
}

GestureRecognizer::ResultFlag MultiLongPressGestureRecognizer::onTimer(GestureObject *state)
{
    ResultFlag retFlag = ResultFlag_Ignore;
    MultiLongPressGesture *ges = static_cast<MultiLongPressGesture *>(state);
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


