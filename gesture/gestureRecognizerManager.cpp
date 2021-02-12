#include "gestureRecognizerManager.h"
#include "gestureRecognizer.h"
#include "gestureRecognizerFactory.h"
#include "gestureFactory.h"
#include "gestureCommonFun.h"
#include "gestureManager.h"
#include <string.h>
#include "malloc.h"
// #include "wayland-util.h"
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

struct GesturePriority {
	enum GESTURE_TYPE type;
    bool interruptLowPriority;
	int priority;
};

struct GesturePriority s_gesturePriority[GESTURE_TYPE_NUM] = {
	{GESTURE_TYPE_SCRATCH, true, 80},
	{GESTURE_TYPE_3FLICK,true, 80},
	{GESTURE_TYPE_MULTILONGPRESS, true, 80},

	{GESTURE_TYPE_TAP, false, 50},
	{GESTURE_TYPE_DRAG, false, 50},
	{GESTURE_TYPE_FLICK, false, 50},
	{GESTURE_TYPE_PINCH, false, 50},
	{GESTURE_TYPE_DOUBLECLICK, false, 50},
	{GESTURE_TYPE_2FLICK, false, 50},
	{GESTURE_TYPE_2ROTARY, false, 50},
	{GESTURE_TYPE_2DRAG, false, 50},
	{GESTURE_TYPE_LONGPRESS, false, 50},
	{GESTURE_TYPE_2LONGPRESS, false, 50},
	{GESTURE_TYPE_2TAP, false, 50}
};

GestureRecognizerManager::GestureRecognizerManager()
{
	for (int i = 0; i < GESTURE_TYPE_NUM; i++) {
		mResultList[i].isValid = false;
		mResultList[i].isUsing = false;
	}
}

GestureRecognizerManager::~GestureRecognizerManager()
{
	clearAllRecognizer();
}

void GestureRecognizerManager::clearAllRecognizer()
{
	for(RecognizersIterator it = mRecognizersV.begin(); it != mRecognizersV.end(); ++it) {
		if (it->second) {
			delete it->second;
			it->second = 0;
		}
	}
	mRecognizersV.clear();

	for (int i = 0; i < GESTURE_TYPE_NUM; i++) {
		if (mResultList[i].isValid) {
			if (!mResultList[i].old) {
				delete mResultList[i].old;
				mResultList[i].old = 0;
			}
			if (!mResultList[i].cur) {
				delete mResultList[i].cur;
				mResultList[i].cur = 0;
			}
			mResultList[i].isValid = false;
		}
	}
}

void GestureRecognizerManager::registerGestureRecognizer(GESTURE_TYPE gtype)
{
    // LOG("GestureRecognizerManager::registerGestureRecognizer gtype[%d]", gtype);
	
	if (GESTURE_TYPE_UNKNOWN >= gtype || GESTURE_TYPE_NUM <= gtype) {
		return;
	}

    int key = (s_gesturePriority[gtype].priority << 16) | gtype;
	for(RecognizersIterator it = mRecognizersV.begin(); it != mRecognizersV.end(); ++it) {
		if (key == it->first) {
			return;
		}
	}

	GestureRecognizer* recognizer = GestureRecognizerFactory::create(gtype);
	if (!recognizer) {
		return;
	}
	
	mRecognizersV.insert(RecognizersMap::value_type(key, recognizer));
	if (!mResultList[gtype].isValid) {
		mResultList[gtype].old = GestureFactory::create(gtype);
		mResultList[gtype].cur = GestureFactory::create(gtype);
		mResultList[gtype].isValid = true;
	}
	mResultList[gtype].isUsing = true;
}

void GestureRecognizerManager::unregisterGestureRecognizer(GESTURE_TYPE gtype)
{
    // LOG("unregisterGestureRecognizer [type:%d]", gtype);
	if (GESTURE_TYPE_UNKNOWN >= gtype || GESTURE_TYPE_NUM <= gtype) {
		return;
	}

	mResultList[gtype].isUsing = false;

    int key = (s_gesturePriority[gtype].priority << 16) | gtype;
	for(RecognizersIterator it = mRecognizersV.begin(); it != mRecognizersV.end(); ++it) {
		if (key == it->first) {
			delete it->second;
			it->second = 0;
			mRecognizersV.erase(it);
			break;
		}
	}
}

void GestureRecognizerManager::createAndAddListItem(struct Gesture_Event_Data_List** head,
    struct Gesture_Event_Data_List** tail,
    struct Gesture_Event_Data* data)
{
	if (!head || !tail || !data) {
		return;
	}
	
    Gesture_Event_Data_List* item = static_cast<Gesture_Event_Data_List*>(malloc(sizeof(Gesture_Event_Data_List)));
	if (!item) {
		return;
	}

	item->data = data;
	item->next = 0;
	if (!(*head)) {
		*head = item;
	}
	if (!(*tail)) {
		*tail = item;
	}
	else {
		(*tail)->next = item;
		*tail = item;
	}
}

void GestureRecognizerManager::freeEventDataList(struct Gesture_Event_Data_List* head)
{
	if (!head) {
		return;
	}

	while (head) {
        Gesture_Event_Data_List* item = head;
		head = head->next;
		free(item->data);
		free(item);
	}
}

void GestureRecognizerManager::processTouchEvent(int motionEventAction, 
			void *data, 
			struct MultiTouchPoint *mtPoints, 
			int pointCount,
            FUNC_NOTIFY_GESTURE_EVENT notify,
            GestureRegion* validRegion,
			long long when)
{
    //LOG("GestureRecognizerManager::processTouchEvent pointCount[%d] validRegion[%d,%d,%d,%d]",
	//	pointCount, validRegion.left,validRegion.top, validRegion.right, validRegion.bottom);
    unsigned long curTime = GestureCommonFun::currentTime();
	
	RecognizersIterator it = mRecognizersV.begin();
    if (MOTION_EVENT_ACTION_DOWN == (motionEventAction & MOTION_EVENT_ACTION_MASK)) {
		for(it = mRecognizersV.begin(); it != mRecognizersV.end(); ++it) {
			it->second->setIgnore(false);
			it->second->setPressTime(curTime);
			it->second->setCurEventTime(static_cast<unsigned long>(when));
		}
	}
	else {
		for(it = mRecognizersV.begin(); it != mRecognizersV.end(); ++it) {
			it->second->setCurEventTime(static_cast<unsigned long>(when));
		}
	}

	bool interruptLowRriority = false;
	int interruptPriority = 0;
	bool needFilter = true;
	for(it = mRecognizersV.begin(); it != mRecognizersV.end(); ++it) {
		GESTURE_TYPE gesType = it->second->getType();
        if (interruptLowRriority && interruptPriority >= s_gesturePriority[gesType].priority) {
			if (!it->second->isIgnore()) {
				needFilter = false;
				it->second->setIgnore(true);
			}
		}

        mResultList[gesType].tmpResultFlag = it->second->recognize(mResultList[gesType].cur, motionEventAction, mtPoints, pointCount, validRegion, data);
        if (s_gesturePriority[gesType].interruptLowPriority) {
			if (GestureRecognizer::ResultFlag_Ignore != mResultList[gesType].tmpResultFlag
				&& (GESTURE_STATE_STARTED <= mResultList[gesType].cur->getState() 
					&&GESTURE_STATE_FINISHED >= mResultList[gesType].cur->getState())) {
					interruptLowRriority = true;
                    interruptPriority = s_gesturePriority[gesType].priority;
			}
		}

		if (GestureRecognizer::ResultFlag_StartAndFinishTrigger == mResultList[gesType].tmpResultFlag
			|| (GestureRecognizer::ResultFlag_Trigger == mResultList[gesType].tmpResultFlag 
				&& (GESTURE_STATE_INVALID != mResultList[gesType].cur->getState() 
						&& GESTURE_STATE_UPDATED != mResultList[gesType].cur->getState())
				)
		) {
			needFilter = false;
		}
	}

	static unsigned long s_lastUpdateTime = 0;
	
        if (needFilter && (abs((long long)curTime - (long long)s_lastUpdateTime) < TOUCH_GESTURE_EVENT_PERIOD)) {

		for (int i = 0; i < GESTURE_TYPE_NUM; i++) {
			if (mResultList[i].isUsing && GESTURE_STATE_UPDATED == mResultList[i].cur->getState()) {
				mResultList[i].cur->copyData(mResultList[i].old);
			}
		}
		
		return;
	}
	
	s_lastUpdateTime = curTime;

    struct Gesture_Event_Data_List* normalHead = 0;
    struct Gesture_Event_Data_List* noramlTail = 0;

    struct Gesture_Event_Data_List* cancelHead = 0;
    struct Gesture_Event_Data_List* cancelTail = 0;
	
	for (int i = 0; i < GESTURE_TYPE_NUM; i++) {
		mResultList[i].resultFlag = mResultList[i].tmpResultFlag;
		
		if ((mResultList[i].isValid && mResultList[i].isUsing) 
			&& (GestureRecognizer::ResultFlag_Ignore != mResultList[i].resultFlag)) {
			GESTURE_STATE newState = mResultList[i].cur->getState();
			GESTURE_STATE oldState = mResultList[i].old->getState();
			// resend the canceled event
			if (GESTURE_STATE_STARTED == newState 
				&& (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState)) {
				mResultList[i].old->setState(GESTURE_STATE_CANCECLED);
				createAndAddListItem(&cancelHead, &cancelTail, GestureCommonFun::gestureToData(mResultList[i].old));
			}

			if (GESTURE_STATE_INVALID != newState 
				&& GESTURE_STATE_MAYBE != newState
				&& !(GESTURE_STATE_CANCECLED == newState && GESTURE_STATE_CANCECLED == oldState)) {
				if (GESTURE_STATE_CANCECLED == newState) {
					createAndAddListItem(&cancelHead, &cancelTail, GestureCommonFun::gestureToData(mResultList[i].cur));
				}
				else if ((GESTURE_STATE_FINISHED == newState || GESTURE_STATE_STARTED == newState)
					&& GestureRecognizer::ResultFlag_StartAndFinishTrigger == mResultList[i].resultFlag) {
					mResultList[i].cur->setState(GESTURE_STATE_STARTED);
                    createAndAddListItem(&normalHead, &noramlTail, GestureCommonFun::gestureToData(mResultList[i].cur));
					mResultList[i].cur->setState(GESTURE_STATE_FINISHED);
                    createAndAddListItem(&normalHead, &noramlTail, GestureCommonFun::gestureToData(mResultList[i].cur));
				}
				else {
                    createAndAddListItem(&normalHead, &noramlTail, GestureCommonFun::gestureToData(mResultList[i].cur));
				}
			}

			mResultList[i].old->copyData(mResultList[i].cur);
		}
	}

	if (notify) {
		notify(cancelHead, data);
        notify(normalHead, data);
	}
	
	freeEventDataList(cancelHead);
    freeEventDataList(normalHead);
}

void GestureRecognizerManager::processTimeOutEvent(GestureRecognizer* recognizer, void* data)
{
	if (!recognizer) {
		return;
	}
	
	GESTURE_TYPE gtype = recognizer->getType();
	mResultList[gtype].resultFlag = recognizer->onTimer(mResultList[gtype].cur);

    struct Gesture_Event_Data_List* noramlHead = 0;
    struct Gesture_Event_Data_List* noramlTail = 0;

    struct Gesture_Event_Data_List* cancelHead = 0;
    struct Gesture_Event_Data_List* cancelTail = 0;
	
	if (mResultList[gtype].isValid && mResultList[gtype].isUsing 
		&& (GestureRecognizer::ResultFlag_Ignore != mResultList[gtype].resultFlag)) {
			
		GESTURE_STATE newState = mResultList[gtype].cur->getState();
		GESTURE_STATE oldState = mResultList[gtype].old->getState();
		// resend the canceled event
		if (GESTURE_STATE_STARTED == newState 
			&& (GESTURE_STATE_STARTED == oldState || GESTURE_STATE_UPDATED == oldState)) {
			mResultList[gtype].old->setState(GESTURE_STATE_CANCECLED);
			createAndAddListItem(&cancelHead, &cancelTail, GestureCommonFun::gestureToData(mResultList[gtype].old));
		}

        if (GESTURE_STATE_INVALID != newState
                && GESTURE_STATE_MAYBE != newState
                && !(GESTURE_STATE_CANCECLED == newState && GESTURE_STATE_CANCECLED == oldState)) {
			if (GESTURE_STATE_CANCECLED == newState) {
				createAndAddListItem(&cancelHead, &cancelTail, GestureCommonFun::gestureToData(mResultList[gtype].cur));
			}
			else if ((GESTURE_STATE_FINISHED == newState || GESTURE_STATE_STARTED == newState)
				&& GestureRecognizer::ResultFlag_StartAndFinishTrigger == mResultList[gtype].resultFlag) {
				mResultList[gtype].cur->setState(GESTURE_STATE_STARTED);
				createAndAddListItem(&noramlHead, &noramlTail, GestureCommonFun::gestureToData(mResultList[gtype].cur));
				mResultList[gtype].cur->setState(GESTURE_STATE_FINISHED);
				createAndAddListItem(&noramlHead, &noramlTail, GestureCommonFun::gestureToData(mResultList[gtype].cur));
			}
			else {
				createAndAddListItem(&noramlHead, &noramlTail, GestureCommonFun::gestureToData(mResultList[gtype].cur));
			}
		}
			
		mResultList[gtype].old->copyData(mResultList[gtype].cur);
	}

    FUNC_NOTIFY_GESTURE_EVENT notify = GestureManager::instance()->getNotifyFunc();
	if (notify) {
		notify(cancelHead, data);
		notify(noramlHead, data);
	}
	
	freeEventDataList(cancelHead);
	freeEventDataList(noramlHead);

}
