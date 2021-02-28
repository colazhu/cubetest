#include "gestureTimerManager.h"
#include "gestureCommonFun.h"
#include "stdlib.h"

struct TimerInfo{
    int id;
    unsigned long interval;
    unsigned long elapsed;
    void* data;
    FUNC_TIMEOUT funcTimeout;
    bool pendingDel;

    void reset(int _interval) {
        interval = _interval;
        elapsed = 0;

    }
};

GestureTimerManager::GestureTimerManager():
    mLastTime(0)
{
    for (int i = 0; i < TimerMax; ++i) {
        mTimers[i] = 0;
    }
}

GestureTimerManager::~GestureTimerManager()
{
    for (int i = 0; i < TimerMax; ++i) {
        if (mTimers[i]) {
            delete mTimers[i];
            mTimers[i] = 0;
        }
    }
}

void GestureTimerManager::update()
{
    if (mLastTime == 0) {
        mLastTime = GestureCommonFun::currentTime();
        return;
    }

    unsigned long currentTime = GestureCommonFun::currentTime();
    unsigned long dt = abs((long long)currentTime - (long long)mLastTime); //  for loop
    mLastTime = currentTime;

    for (int i = 0; i < TimerMax; ++i) {
        if (mTimers[i] && mTimers[i]->interval && !mTimers[i]->pendingDel) {
            mTimers[i]->elapsed += dt;
            if (mTimers[i]->elapsed > mTimers[i]->interval) {
                mTimers[i]->funcTimeout(mTimers[i]->data);
            }
        }
    }

    for (int i = 0; i < TimerMax; ++i) {
        if (mTimers[i] && mTimers[i]->pendingDel) {
            delete mTimers[i];
            mTimers[i] = 0;
        }
    }
}

TimerInfo* GestureTimerManager::getTimerInfo(int id) {
    for (int i = 0; i < TimerMax; ++i) {
        if (mTimers[i] && mTimers[i]->id == id) {
            return mTimers[i];
        }
    }
    return 0;
}

int GestureTimerManager::registerTimer(FUNC_TIMEOUT func, void* data)
{
    static int s_seeds = 0;
    ++s_seeds;

    for (int i = 0; i < TimerMax; ++i) {
        if (!mTimers[i]) {
            mTimers[i] = new TimerInfo({s_seeds, 0, 0, data, func, false});
            break;
        }
    }

    return s_seeds;
}

void GestureTimerManager::unregisterTimer(int id)
{
    for (int i = 0; i < TimerMax; ++i) {
        if (mTimers[i] && mTimers[i]->id == id) {
            mTimers[i]->pendingDel = true;
        }
    }
}

void GestureTimerManager::updateTimer(int id, int interval)
{
     TimerInfo* info = getTimerInfo(id);
     if (info) {
        info->reset(interval);
     }
}
