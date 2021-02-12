#ifndef GESTURETIMERMANAGER_H
#define GESTURETIMERMANAGER_H

typedef int (*FUNC_TIMEOUT)(void *data);
struct TimerInfo;
class GestureTimerManager
{
public:
    GestureTimerManager();
    ~GestureTimerManager();

    void update();

    int registerTimer(FUNC_TIMEOUT func, void* data);
    void unregisterTimer(int id);

    void updateTimer(int id, int interval);
private:
    TimerInfo* getTimerInfo(int id);

    enum { TimerMax = 32 };

    unsigned long mLastTime;
    TimerInfo* mTimers[TimerMax];

};

#endif // GESTURETIMERMANAGER_H
