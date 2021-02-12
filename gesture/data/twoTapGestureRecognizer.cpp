#include "twoTapGestureRecognizer.h"
#include "gestureCommonFun.h"
// #include "gestureTimer.h"
#include "twoTapGesture.h"
#include "twoDragGesture.h"
#include "rotateGesture.h"
#include "twoFlickGesture.h"
#include "pinchGesture.h"
#include <stdlib.h>

TwoTapGestureRecognizer::TwoTapGestureRecognizer():
GestureRecognizer(GESTURE_TYPE_2TAP)
{

}

TwoTapGestureRecognizer::~TwoTapGestureRecognizer()
{

}

GestureRecognizer::ResultFlag TwoTapGestureRecognizer::onTimer(GestureObject *state, void *data)
{
    ResultFlag retFlag = ResultFlag_Ignore;
    TwoTapGesture *ges = static_cast<TwoTapGesture *>(state);
    if (!ges) {
        return retFlag;
    }

    GESTURE_STATE oldState = ges->getState();
    if (GESTURE_STATE_FINISHED == oldState) {
        ges->setState(GESTURE_STATE_FINISHED);
        retFlag = ResultFlag_Trigger;
    }
    
    return retFlag;
}

GestureRecognizer::ResultFlag TwoTapGestureRecognizer::recognize(GestureObject *state, int motionEventAction,
        struct MultiTouchPoint *mtPoints, int pointCount, GestureRegion* validRegion, void *data)
{
    ResultFlag retFlag = ResultFlag_Ignore;
    
    TwoTapGesture *ges = static_cast<TwoTapGesture *>(state);
    if (!ges) {
        return retFlag;
    }

    GESTURE_STATE oldState = ges->getState();
    
    if (isIgnore()) {
        if (GESTURE_STATE_INVALID != oldState) {
            ges->clearData();
        }

        return retFlag;
    }

    int touchType = motionEventAction & MOTION_EVENT_ACTION_MASK;
    switch (touchType) {
    case MOTION_EVENT_ACTION_DOWN:
    case MOTION_EVENT_ACTION_POINTER_DOWN:
        {
            if (2 == pointCount){
                ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
                ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);
                    
                int centerX = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
                int centerY = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
                ges->setCenter(centerX, centerY);
                ges->setLastCenter(centerX, centerY);
                ges->setStartCenter(centerX, centerY);

                ges->setOffset(0, 0);
                ges->setLastOffset(0, 0);
                ges->setStartOffset(0, 0);

                ges->setState(GESTURE_STATE_MAYBE);
                retFlag = ResultFlag_Ignore;                
            }
            else {
                if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState) {
                    ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
                    ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);

                    ges->setLastCenter(ges->getCenter().x, ges->getCenter().y);
                    ges->setLastOffset(ges->getOffset().x, ges->getOffset().y);

                    int centerX = (mtPoints[0].coords.x + mtPoints[1].coords.x)/2;
                    int centerY = (mtPoints[0].coords.y + mtPoints[1].coords.y)/2;
                    ges->setCenter(centerX, centerY);
                    ges->setOffset(centerX - ges->getLastCenter().x, centerY - ges->getLastCenter().y);
                   
                    GesturePoint startOffset;
                    startOffset.x = centerX - ges->getStartCenter().x;
                    startOffset.y = centerY - ges->getStartCenter().y;
                    ges->setStartOffset(startOffset.x, startOffset.y);

                    ges->setState(GESTURE_STATE_FINISHED);
                    retFlag = ResultFlag_Trigger;
                }
                else if (GESTURE_STATE_MAYBE == oldState) {
                    ges->clearData();
                    retFlag = ResultFlag_Ignore;
                }
            }
        }
        break;
    case MOTION_EVENT_ACTION_MOVE:
        {
            if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState || GESTURE_STATE_MAYBE == oldState) {

                GesturePoint& point0 = ges->getPoint(0);
                unsigned int manhattanLength0 = abs(point0.x - mtPoints[0].coords.x) + abs(point0.y - mtPoints[0].coords.y);

                if (2 == pointCount) {
                    GesturePoint& point1 = ges->getPoint(1);
                    unsigned int manhattanLength1 = abs(point1.x - mtPoints[1].coords.x) + abs(point1.y - mtPoints[1].coords.y);
//                    LOG("TwoTapGestureRecognizer::recognize D1:%d manhattanLength0:%d manhattanLength1:%d T1:%d; CurEventTime:%d",
//                                TwoTapGestureLength, manhattanLength0, manhattanLength1,
//                                TwoTapGestureInterval ,getCurEventTime());

                    if (TwoTapGestureLength <= manhattanLength0 || TwoTapGestureLength <= manhattanLength1
                        || TwoTapGestureInterval <= (getCurEventTime() - getPressTime())) {
//                        LOG("TwoTapGestureRecognizer::recognize D1:%d manhattanLength0:%d manhattanLength1:%d T1:%d; CurEventTime:%d PressEventTime:%d",
//                                TwoTapGestureLength, manhattanLength0, manhattanLength1,
//                                TwoTapGestureInterval ,getCurEventTime(), getPressTime());
                            setIgnore(true);
                    }
                }
                else if (1 == pointCount) {
//                    LOG("TwoTapGestureRecognizer::recognize D1:%d manhattanLength0:%d T1:%d; CurEventTime:%d PressEventTime:%d",
//                                TwoTapGestureLength, manhattanLength0,
//                                TwoTapGestureInterval ,getCurEventTime(), getPressTime());
                    if (TwoTapGestureLength <= manhattanLength0 || TwoTapGestureInterval <= (getCurEventTime() - getPressTime())) {
//                            LOG("TwoTapGestureRecognizer::recognize D1:%d manhattanLength0:%d T1:%d; CurEventTime:%d PressEventTime:%d",
//                                TwoTapGestureLength, manhattanLength0,
//                                TwoTapGestureInterval ,getCurEventTime(), getPressTime());
                            setIgnore(true);
                    }
                }
            }

        }
        break;
    case MOTION_EVENT_ACTION_UP:

        if (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState || GESTURE_STATE_MAYBE == oldState) {

            if (2 == pointCount) {
                GesturePoint& point0 = ges->getPoint(0);
                unsigned int manhattanLength0 = abs(point0.x - mtPoints[0].coords.x) + abs(point0.y - mtPoints[0].coords.y);

                GesturePoint& point1 = ges->getPoint(1);
                unsigned int manhattanLength1 = abs(point1.x - mtPoints[1].coords.x) + abs(point1.y - mtPoints[1].coords.y);
                if (TwoTapGestureLength <= manhattanLength0 || TwoTapGestureLength <= manhattanLength1
                    || TwoTapGestureInterval <= (getCurEventTime() - getPressTime())) {
//                        LOG("TwoTapGestureRecognizer::recognize D1:%d manhattanLength0:%d manhattanLength1:%d T1:%d; CurEventTime:%d PressEventTime:%d",
//                                TwoTapGestureLength, manhattanLength0, manhattanLength1,
//                                TwoTapGestureInterval ,getCurEventTime(), getPressTime());
                        setIgnore(true);
                }

                ges->setPoint(0, mtPoints[0].coords.x, mtPoints[0].coords.y);
                ges->setPoint(1, mtPoints[1].coords.x, mtPoints[1].coords.y);
            }
            
            if (isIgnore()) {
                ges->clearData();
                retFlag = ResultFlag_Ignore;
            }
            else {
                ges->setLastCenter(ges->getCenter().x, ges->getCenter().y);
                ges->setLastOffset(ges->getOffset().x, ges->getOffset().y);

                int centerX = (ges->getPoint(0).x + ges->getPoint(1).x)/2;
                int centerY = (ges->getPoint(0).y + ges->getPoint(1).y)/2;
                ges->setCenter(centerX, centerY);
                ges->setOffset(centerX - ges->getLastCenter().x, centerY - ges->getLastCenter().y);
                        
                GesturePoint startOffset;
                startOffset.x = centerX - ges->getStartCenter().x;
                startOffset.y = centerY - ges->getStartCenter().y;
                ges->setStartOffset(startOffset.x, startOffset.y);
                
                ges->setState(GESTURE_STATE_FINISHED);
                retFlag = ResultFlag_Trigger;
            }
        }
        else {
            
        }
        break;

    default:
        break;
    }

    return retFlag;
}

