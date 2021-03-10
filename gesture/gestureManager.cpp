#include "gestureManager.h"
#include "gestureRecognizerManager.h"
#include "gestureTimerManager.h"
#include "gestureCommonFun.h"
#include <string.h>
#include "malloc.h"

GestureManager* GestureManager::s_instance = 0;

struct GestureRegisterInfo {
	void* mOwner;
	uint32_t mMask;
};

class GestureRegisters {
public:
	GestureRegisters(uint32_t maxGestureType);
	~GestureRegisters();
	void enable(void *owner, uint32_t mask);
	void disable(void *owner, uint32_t mask);
	void remove(void *owner);
	void clear();
	uint32_t getGlobalMask();

private:
	GestureRegisters(const GestureRegisters& rhs);
	GestureRegisters& operator=(const GestureRegisters& rhs);

	std::vector<GestureRegisterInfo*> mRegistersV;
	int32_t* mRegisterNum;
	uint32_t mMaxGestureType;
	uint32_t mGlobalMask;
};


GestureRegisters::GestureRegisters(uint32_t maxGestureType):
mMaxGestureType(maxGestureType),
mGlobalMask(0)
{
	mRegisterNum = (int32_t*)malloc(sizeof(int32_t)*maxGestureType);
    memset(mRegisterNum, 0, sizeof(int32_t)*maxGestureType);
}

GestureRegisters::~GestureRegisters()
{
	clear();
	free(mRegisterNum);
}

void GestureRegisters::enable(void *owner, uint32_t mask)
{	
	uint32_t validMask = ((uint32_t(-1))>>((uint32_t)(sizeof(mask)*8) - mMaxGestureType)) & mask;
	
	if (!owner || (0 == validMask)) {
		return;
	}
		
	size_t num = mRegistersV.size();
	bool found = false;
	GestureRegisterInfo* info = 0;
	for (size_t pos = 0; pos < num; pos++) {
		info = mRegistersV[pos];
		if (owner == info->mOwner) {
			found = true;
			break;
		}
	}
	
	if (!found) {
		info = new GestureRegisterInfo();
		info->mOwner = owner;
		info->mMask = 0;
		mRegistersV.push_back(info);
	}

    // LOG("GestureRegisters::enable owner[0x%x] input[0x%x] old owner mask[0x%x] global mask[0x%x]", owner, validMask, info->mMask, mGlobalMask);

	for (uint32_t i = 0; i < mMaxGestureType; i++) {
		if ((0 != (1 & (validMask>>i))) && (0 == (1 & (info->mMask>>i)))) {
			mRegisterNum[i]++;
			mGlobalMask |= (1<<i);
		} 
	}

	info->mMask |= validMask;
    // LOG("GestureRegisters::enable new owner mask[0x%x] global mask[0x%x]", info->mMask, mGlobalMask);
}

void GestureRegisters::disable(void *owner, uint32_t mask)
{
	uint32_t validMask = ((uint32_t(-1))>>((uint32_t)(sizeof(mask)*8) - mMaxGestureType)) & mask;
	if (!owner || (0 == validMask)) {
		return;
	}
		
	size_t num = mRegistersV.size();
	bool found = false;
	GestureRegisterInfo* info = 0;
	for (size_t pos = 0; pos < num; pos++) {
		info = mRegistersV[pos];
		if (owner == info->mOwner) {
			found = true;
			break;
		}
	}
	
	if (!found) {
		return;
	}

    // LOG("GestureRegisters::disable owner[0x%x] input[0x%x] old owner mask[0x%x] global mask[0x%x]", owner, validMask, info->mMask, mGlobalMask);

	for (uint32_t i = 0; i < mMaxGestureType; i++) {
		if ((0 != (1 & (validMask>>i))) && (0 != (1 & (info->mMask>>i)))) {
			mRegisterNum[i]--;
			if (0 >= mRegisterNum[i]) {
				mRegisterNum[i] = 0;
				mGlobalMask |= ~(1<<i);
			}

			info->mMask |= ~(1<<i);
		} 
	}

	if (0 == info->mMask) {
		remove(owner);
        // LOG("GestureRegisters::disable new owner mask[0x%x] global mask[0x%x]", 0, mGlobalMask);
	}
	else {
        // LOG("GestureRegisters::disable new owner mask[0x%x] global mask[0x%x]", info->mMask, mGlobalMask);
	}
}

void GestureRegisters::remove(void *owner)
{
	if (!owner) {
		return;
	}

	std::vector<GestureRegisterInfo*>::iterator itList;
	for(itList = mRegistersV.begin(); itList != mRegistersV.end();){
		if (owner == (*itList)->mOwner) {
			delete *itList;
			mRegistersV.erase(itList);
			break;
		}
		else {
			++itList;
		}
	}

	mGlobalMask = 0;
	for(itList = mRegistersV.begin(); itList != mRegistersV.end();){
		mGlobalMask |= (*itList)->mMask;
		++itList;
	}
}

void GestureRegisters::clear()
{
	size_t num = mRegistersV.size();
    for (size_t i = 0; i < num; i++) {
        delete mRegistersV[i];
    }
    
    mRegistersV.clear();
	mGlobalMask = 0;
}

uint32_t GestureRegisters::getGlobalMask()
{
	return mGlobalMask;
}

GestureManager* GestureManager::instance()
{
    if (!s_instance) {
        s_instance = new GestureManager();
    }

    return s_instance;
}

void GestureManager::destroy()
{
    if (s_instance) {
        delete s_instance;
        s_instance = 0;
    }
}

GestureManager::GestureManager():
mSystemRegisters(0),
mSystemGlobalMask(0),
mCommonRegisters(0),
mCommonGlobalMask(0),
mFoucsSurfaceRegion(0),
mRecognizerManager(0),
mTimerManager(0),
mNotifyFunc(0)
// mEventLoop(0)
{
	mSystemRegisters = new GestureRegisters(WL_SYSTEM_GESTURE_TYPE_NUM);
	mCommonRegisters = new GestureRegisters(WL_COMMON_GESTURE_TYPE_NUM);
	mRecognizerManager = new GestureRecognizerManager();	
    mTimerManager = new GestureTimerManager();
    mFoucsSurfaceRegion = new GestureRegion();

    memset(m_points, 0, sizeof(MultiTouchPoint)*WL_GESTURE_MAX_POINTS);
    for (int i = 0; i < WL_GESTURE_MAX_POINTS; ++i) {
        m_points[i].id = -1;
    }
}

GestureManager::~GestureManager()
{
	if (mSystemRegisters) {
		delete mSystemRegisters;
		mSystemRegisters = 0;
	}

	if (mCommonRegisters) {
		delete mCommonRegisters;
		mCommonRegisters = 0;
	}

	if (mRecognizerManager) {
		delete mRecognizerManager;
		mRecognizerManager = 0;
	}

    // pixman_region32_fini(&mFoucsSurfaceRegion);
    if (mFoucsSurfaceRegion) {
        delete mFoucsSurfaceRegion;
        mFoucsSurfaceRegion = 0;
    }

    if (mTimerManager) {
        delete mTimerManager;
        mTimerManager = 0;
    }
}

void GestureManager::enableSystemGesture(uint32_t mask)
{
	if (mSystemRegisters) {
		mSystemRegisters->enable(this, mask);
		updateGestureRecognizer(WL_GESTURE_CLASS_SYSTEM, mSystemRegisters->getGlobalMask());
	}
}

void GestureManager::disableSystemGesture(uint32_t mask)
{
	if (mSystemRegisters) {
		mSystemRegisters->disable(this, mask);
		updateGestureRecognizer(WL_GESTURE_CLASS_SYSTEM, mSystemRegisters->getGlobalMask());
	}
}

uint32_t GestureManager::getSystemMask()
{
	if (mSystemRegisters) {
        return mSystemRegisters->getGlobalMask();
	}

	return 0;
}

void GestureManager::enableCommonGesture(void *surface, uint32_t mask)
{
	if (mCommonRegisters) {
		mCommonRegisters->enable(surface, mask);
		updateGestureRecognizer(WL_GESTURE_CLASS_COMMON, mCommonRegisters->getGlobalMask());
	}
}

void GestureManager::disableCommonGesture(void *surface, uint32_t mask)
{
	if (mCommonRegisters) {
		mCommonRegisters->disable(surface, mask);
		updateGestureRecognizer(WL_GESTURE_CLASS_COMMON, mCommonRegisters->getGlobalMask());
	}
}

void GestureManager::onSurfaceDestroy(void *surface)
{
	if (mCommonRegisters) {
		mCommonRegisters->remove(surface);
		updateGestureRecognizer(WL_GESTURE_CLASS_COMMON, mCommonRegisters->getGlobalMask());
	}
}

uint32_t GestureManager::getCommonMask()
{
	if (mCommonRegisters) {
		mCommonRegisters->getGlobalMask();
	}

	return 0;
}

void GestureManager::update()
{
    if (mTimerManager) {
        mTimerManager->update();
    }
}

void GestureManager::setNotifyFunc(FUNC_NOTIFY_GESTURE_EVENT notify)
{
	mNotifyFunc = notify;
}

//void GestureManager::setEventLoop(struct wl_event_loop *eventLoop)
//{
//	mEventLoop = eventLoop;
//}

bool GestureManager::isMultiTouching()
{
    int validPoints = 0;
    for (int i = 0; i < WL_GESTURE_MAX_POINTS; ++i) {
        if (m_points[i].id != -1) {
            ++validPoints;
        }
    }
    return validPoints > 1;
}
bool GestureManager::isPrimary(int id)
{  
    for (int i = 0; i < WL_GESTURE_MAX_POINTS; ++i) {
        if (m_points[i].id == id) {
            return m_points[i].isPrimary;
        }
    }
    return false;
}

int GestureManager::processSingleTouchDown(int id, int x, int y)
{
    int motionEventAction = -1;
    do {
        if (!mFoucsSurfaceRegion || !mFoucsSurfaceRegion->contain(x, y)) {
            // LOG_BASE("out of range");
            break;
        }
        
        int validPoints = 0;
		int idxSelf = -1;
        int idxIdle = -1;
        for (int i = 0; i < WL_GESTURE_MAX_POINTS; ++i) {
            if (m_points[i].id == id) {
                idxSelf = i;
            }
            if (m_points[i].id != -1) {
                ++validPoints;
            }
            else {
                if (-1 == idxIdle) {
                    idxIdle = i;
                }
            }
        }

        if (idxSelf != -1) {
			LOG_BASE("[id:%d] pre-press again, ignore", id);  
			break;
            // LOG_BASE("found self not finised yet, re-press again");            
			// idxIdle = idxSelf; 
        }

        if (idxIdle == -1) {
            // LOG_BASE("no idle");
            break; // reach max points ignore
        }

        motionEventAction = (validPoints == 0) ? MOTION_EVENT_ACTION_DOWN : MOTION_EVENT_ACTION_POINTER_DOWN;
        m_points[idxIdle].id = id;
        m_points[idxIdle].isPrimary = (validPoints == 0);
        m_points[idxIdle].state = TouchPointPressed;
        m_points[idxIdle].coords.x = x;
        m_points[idxIdle].coords.y = y;
    } while(0);
    return motionEventAction;
}

int GestureManager::processSingleTouchMove(int id, int x, int y)
{
    for (int i = 0; i < WL_GESTURE_MAX_POINTS; ++i) {
        if (m_points[i].id == id) {
            if (m_points[i].state == TouchPointPressed ||
                m_points[i].state == TouchPointMoved) {
                m_points[i].state = TouchPointMoved;
                m_points[i].coords.x = x;
                m_points[i].coords.y = y;
                return MOTION_EVENT_ACTION_MOVE;
            }
        }
    }
    return -1;
}

int GestureManager::processSingleTouchUp(int id, int x, int y)
{
    int motionEventAction = -1;
    bool otherPressed = false;
    for (int i = 0; i < WL_GESTURE_MAX_POINTS; ++i) {
        if (m_points[i].id == id) {
            if (m_points[i].state == TouchPointPressed ||
                m_points[i].state == TouchPointMoved) {
                m_points[i].state = TouchPointReleased;
                motionEventAction = MOTION_EVENT_ACTION_UP;
            }
        }
        else {
            if (m_points[i].id != -1
                    && (m_points[i].state == TouchPointPressed || m_points[i].state == TouchPointMoved)) {
                otherPressed = true;
            }
        }
    }

    if (otherPressed && motionEventAction == MOTION_EVENT_ACTION_UP) {
        motionEventAction = MOTION_EVENT_ACTION_POINTER_UP;
    }
    return motionEventAction;
}

void GestureManager::postProcessSingleTouch()
{
    for (int i = 0; i < WL_GESTURE_MAX_POINTS; ++i) {
        if (m_points[i].state == TouchPointReleased) {
            memset(&m_points[i], 0, sizeof(MultiTouchPoint));
            m_points[i].id = -1;
        }
    }
}

void GestureManager::processTouchEvent(int eventAction, void *data, int id, int x, int y, long long when)
{
    // LOG_BASE_TRACE("processTouchEvent:%d id:%d x:%d y:%d", eventAction, id, x, y);
    int motionEventAction = -1;
    switch (eventAction) {
    case MOTION_EVENT_ACTION_DOWN:
        motionEventAction = processSingleTouchDown(id, x, y);
        break;
    case MOTION_EVENT_ACTION_UP:
        motionEventAction = processSingleTouchUp(id, x, y);
        break;
    case MOTION_EVENT_ACTION_MOVE:
        motionEventAction = processSingleTouchMove(id, x, y);
        break;
    default:
        break;
    }

    if (-1 == motionEventAction) {
        LOG_BASE_E("Invalid EventAction");
        return;
    }

    int validPoints = 0;
    MultiTouchPoint points[WL_GESTURE_MAX_POINTS];
    for (int i = 0; i < WL_GESTURE_MAX_POINTS; ++i) {
        if (m_points[i].id != -1) {
            memcpy(&points[validPoints], &m_points[i], sizeof(MultiTouchPoint));
            ++validPoints;
        }
    }
    processTouchEvent(motionEventAction, data, points, validPoints, when);
    postProcessSingleTouch();
}

void GestureManager::processTouchEvent(int motionEventAction, 
                                        void *data,
                                        struct MultiTouchPoint *mtPoints,
                                        int pointCount,
                                        long long when)
{
    mRecognizerManager->processTouchEvent(motionEventAction, data, mtPoints, pointCount, mNotifyFunc, mFoucsSurfaceRegion, when == 0 ?  GestureCommonFun::currentTime() : when);
}

void GestureManager::setFoucsSurfaceRegion(const GestureRegion& region)
{
    memcpy(mFoucsSurfaceRegion, &region, sizeof(GestureRegion));
//	pixman_region32_copy(&mFoucsSurfaceRegion,
//				     region);
}

void GestureManager::updateGestureRecognizer(WL_GESTURE_CLASS gclass, uint32_t mask)
{	
	uint32_t oldMask = 0;
	int maxType = 0;
	if (WL_GESTURE_CLASS_SYSTEM == gclass) {
		oldMask = mSystemGlobalMask;
		maxType = WL_SYSTEM_GESTURE_TYPE_NUM;
		mSystemGlobalMask = mask;
	}
	else {
		oldMask = mCommonGlobalMask;
		maxType = WL_COMMON_GESTURE_TYPE_NUM;
		mCommonGlobalMask = mask;
	}

    // LOG("GestureManager::updateGestureRecognizer gclass[0x%x] mask[0x%x] oldmask[0x%x]", gclass, mask, oldMask);

	if (oldMask == mask) {
		return;
	}

	for (int i = 0; i < maxType; i++) {
		// not exist -->  exist
		if ((0 == (1 & oldMask>>i)) && (0 != (1 & mask>>i))) {
			mRecognizerManager->registerGestureRecognizer(GestureCommonFun::gestureTypeConvert(gclass, i));
		}
		//  exist --> not exist
		else if ((0 != (1 & oldMask>>i)) && (0 == (1 & mask>>i))) {
			mRecognizerManager->unregisterGestureRecognizer(GestureCommonFun::gestureTypeConvert(gclass, i));
		}
		//the same
		else {
		}
	}
}



