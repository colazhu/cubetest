#ifndef GESTUREMANAGER_H
#define GESTUREMANAGER_H
//#include <vector>
//#include <pixman.h>
#include "stdint.h"
#include "gesturePublicDef.h"
#include "gestureDef.h"

struct GestureRegisters;
class GestureRegion;
class GestureRecognizerManager;
class GestureTimerManager;
class GestureManager
{
public:
	static GestureManager* instance();
	static void destroy();


	void enableSystemGesture(uint32_t mask);
	void disableSystemGesture(uint32_t mask);
	uint32_t getSystemMask();

    void enableCommonGesture(void *surface, uint32_t mask);
    void disableCommonGesture(void *surface, uint32_t mask);
    uint32_t getCommonMask();
    void onSurfaceDestroy(void *surface);
    void processTouchEvent(int eventAction,
                           void *data,
                           int id, int x, int y, long long when = 0);
	void processTouchEvent(int motionEventAction, 
			void *data, 
			struct MultiTouchPoint *mtPoints, 
			int pointCount,
            long long when = 0);

    void setFoucsSurfaceRegion(const GestureRegion& region);
    GestureRegion* getFoucsSurfaceRegion() {return mFoucsSurfaceRegion;}
    GestureRecognizerManager* getGestureRecognizerManager() {return mRecognizerManager;}
    GestureTimerManager* getTimerManager() { return mTimerManager;}

    void setNotifyFunc(FUNC_NOTIFY_GESTURE_EVENT notify);
    FUNC_NOTIFY_GESTURE_EVENT getNotifyFunc() {return mNotifyFunc;}
	
    void update();
private:
	GestureManager();
	~GestureManager();
	GestureManager(GestureManager&);
	GestureManager& operator=(GestureManager&);

    void updateGestureRecognizer(WL_GESTURE_CLASS gclass, uint32_t mask);
    int processSingleTouchDown(int id, int x, int y);
    int processSingleTouchMove(int id, int x, int y);
    int processSingleTouchUp(int id, int x, int y);
    void postProcessSingleTouch();
private:
    static GestureManager* s_instance;

	GestureRegisters* mSystemRegisters;
	uint32_t mSystemGlobalMask;

    GestureRegisters* mCommonRegisters;
	uint32_t mCommonGlobalMask;

    GestureRegion* mFoucsSurfaceRegion;

    GestureRecognizerManager* mRecognizerManager;
    GestureTimerManager* mTimerManager;

    MultiTouchPoint m_points[WL_GESTURE_MAX_POINTS];

    FUNC_NOTIFY_GESTURE_EVENT mNotifyFunc;
};

#endif /* GESTUREMANAGER_H */
