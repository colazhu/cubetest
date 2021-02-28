#include "gestureTimer.h"
#include "gestureManager.h"
#include "gestureRecognizerManager.h"
#include "gestureTimerManager.h"
//#include "wayland-server.h"
//#include "wayland-util.h"

class GestureTimerPrivate {
public:
	GestureTimerPrivate(GestureRecognizer* recognizer, void* data);
	~GestureTimerPrivate();
	static int timeOut(void* data);
	void setInterval(unsigned int interval, bool repeat = false);
	void start();
	void stop();
    void restart();
	GestureRecognizer* getRecognizer();
	void* getUserData();
	void setUserData(void* data);
	
private:
//	struct wl_event_loop *mEventLoop;
//	struct wl_event_source * mEventSource;
    int mId;
	bool mStarted;
	unsigned int mInterval;
	bool mRepeat;
	GestureRecognizer* mRecognizer;
	void* mUsrData;
};

GestureTimerPrivate::GestureTimerPrivate(GestureRecognizer* recognizer, void* data):
//mEventLoop(0),
//mEventSource(0),
mStarted(false),
mInterval(-1),
mRepeat(false),
mRecognizer(recognizer),
mUsrData(data)
{
    mId = GestureManager::instance()->getTimerManager()->registerTimer(GestureTimerPrivate::timeOut, this);
//	mEventLoop = GestureManager::instance()->getEventLoop();
//    mEventSource = wl_event_loop_add_noblock_timer(mEventLoop, GestureTimerPrivate::timeOut, this);
}

GestureTimerPrivate::~GestureTimerPrivate()
{
    GestureManager::instance()->getTimerManager()->unregisterTimer(mId);
    // wl_event_source_remove(mEventSource);
    if (mUsrData) {
        delete mUsrData;
    }
}

int GestureTimerPrivate::timeOut(void* data)
{
    // LOG_BASE("onTimeout");
	if (!data) {
		return -1;
	}
	
	GestureTimerPrivate* p = static_cast<GestureTimerPrivate*>(data);
	if (!p->mStarted) {
		return 0;
	}
	
	if (p->mRepeat) {
        GestureManager::instance()->getTimerManager()->updateTimer(p->mId, p->mInterval);
        // wl_event_source_timer_update(p->mEventSource, p->mInterval);
	}
	else {
		p->mStarted = false;
        GestureManager::instance()->getTimerManager()->updateTimer(p->mId, 0);
        // wl_event_source_timer_update(p->mEventSource, 0);
	}

	GestureManager::instance()->getGestureRecognizerManager()->processTimeOutEvent(p->mRecognizer, p->mUsrData);
	
	return 0;
}

void GestureTimerPrivate::start()
{
	mStarted = true;
    GestureManager::instance()->getTimerManager()->updateTimer(mId, mInterval);
    // wl_event_source_timer_update(mEventSource, mInterval);
}

void GestureTimerPrivate::stop()
{
	mStarted = false;
    GestureManager::instance()->getTimerManager()->updateTimer(mId, 0);
    // wl_event_source_timer_update(mEventSource, 0);
}

void GestureTimerPrivate::restart()
{
	mStarted = true;
    GestureManager::instance()->getTimerManager()->updateTimer(mId, mInterval);
    // wl_event_source_timer_update(mEventSource, mInterval);
}

void GestureTimerPrivate::setInterval(unsigned int interval, bool repeat)
{
	mInterval = interval;
	mRepeat = repeat;
}

GestureRecognizer* GestureTimerPrivate::getRecognizer()
{
	return mRecognizer;
}

void* GestureTimerPrivate::getUserData()
{
	return mUsrData;
}

void GestureTimerPrivate::setUserData(void* data)
{
	mUsrData = data;
}

GestureTimer::GestureTimer(GestureRecognizer* recognizer, void* data):
mPrivate(new GestureTimerPrivate(recognizer, data))
{
}

GestureTimer::~GestureTimer()
{
	if (mPrivate) {
		delete mPrivate;
        mPrivate = 0;
	}
}

GestureRecognizer* GestureTimer::getRecognizer()
{
	return mPrivate->getRecognizer();
}

void GestureTimer::start()
{
	mPrivate->start();
}

void GestureTimer::stop()
{
	mPrivate->stop();
}

void GestureTimer::restart()
{
    mPrivate->restart();
}

void GestureTimer::setInterval(unsigned int interval, bool repeat)
{
	mPrivate->setInterval(interval, repeat);
}

void* GestureTimer::getUserData()
{
	return mPrivate->getUserData();
}

void GestureTimer::setUserData(void* data)
{
	mPrivate->setUserData(data);
}
