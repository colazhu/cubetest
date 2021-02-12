#ifndef ACTION_H
#define ACTION_H

#include "Base.h"
#include "CurveFunction.h"

class Node;
class Action
{
    friend class ActionCache;
public:
    explicit Action(Node* target = 0);
    virtual ~Action();

    virtual bool isDone() const = 0;
    virtual void update(float dt) = 0;

    inline Node* getTarget() const {
        return m_target;
    }

    inline float getDuration() const {
        return m_duration;
    }

    inline void setDuration(float duration) {
        m_duration = duration;
    }

    inline void setAutoRelease(bool autorelease) { m_autoRelease = autorelease; }
    inline bool isAutoRelease() const { return m_autoRelease; }
    virtual void reverse() {}

protected:
    Node* m_target;
    bool m_autoRelease;
    float m_duration;
};

class IntervalAction : public Action {
public:
    IntervalAction(Node* target = 0);
    virtual ~IntervalAction();

    void initTimeline(float duration, bool forward = true, int curve = 0);
    void setDelay(float delaytime);
    void reset();
    void reverse();
    inline float getElapsed() { return m_elapsed;}
    inline float getTime() {return m_forward ? m_time : (1.0f - m_time);}
    virtual bool isDone() const;
    virtual void update(float dt);
    // void setCurve(int curve) { m_curve = curve;}

    virtual void onStart(Node* target) { UNUSED(target); }
    virtual void onEnd(Node* target) {UNUSED(target); }
    virtual void onUpdate(Node* target, float dt, float time) {
        UNUSED(target); UNUSED(dt); UNUSED(time);
    }

protected:
    enum STATUS {
        STATUS_INIT,
        STATUS_RUNNING,
        STATUS_DONE
    };

    STATUS m_status;

    bool m_forward;
    int m_curve;
    float m_time;
    float m_elapsed;

    float m_delaytime;
    float m_delayElapsed;
};

#endif // ACTION_H
