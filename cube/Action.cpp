#include "Action.h"
#include "MathInc.h"
#include "Director.h"
#include "ActionCache.h"
#include "Node.h"

//---------------------------------------//
Action::Action(Node* target)
        : m_target(target),
          m_autoRelease(true),
          m_duration(1.0)
{
}

Action::~Action() {
}

//---------------------------------------//
IntervalAction::IntervalAction(Node* target)
        : Action(target),
          m_status(STATUS_INIT),
          m_elapsed(0.0f),
          m_time(0.0f),
          m_curve(Linear),
          m_forward(true),
          m_delaytime(0.0f),
          m_delayElapsed(0.0f){
}

IntervalAction::~IntervalAction() {

}

void IntervalAction::setDelay(float delaytime)
{
    m_delaytime = delaytime;
    m_delayElapsed = 0.0f;
}

void IntervalAction::initTimeline(float duration, bool forward, int curve)
{
    m_duration = duration;
    m_forward = forward;
    m_curve = curve;
    reset();
}

void IntervalAction::reset()
{
    m_status = STATUS_INIT;
    m_elapsed = 0.0f;
    m_time = 0.0f;
    m_delayElapsed = 0.0f;
}

void IntervalAction::reverse()
{
    m_forward = !m_forward;
    reset();
}

bool IntervalAction::isDone() const {
    return m_status == STATUS_DONE; // m_elapsed >= m_duration;
}

void IntervalAction::update(float dt) {
    if (m_status == STATUS_DONE) {
        if(m_autoRelease) {
            Director::instance()->actionCache().removeAction(this);
        }
        return;
    }

    // first frame
    if (m_status == STATUS_INIT) {
        if (m_delaytime > 0.0f) {
            m_delayElapsed += dt;
            if (m_delayElapsed < m_delaytime) {
                return;
            }
            else {
                m_delayElapsed = 0.0f;
            }
        }

        m_elapsed = 0.0f;
        m_time = 0.0f;
        dt = 0.0f;
        onStart(m_target);
        m_status = STATUS_RUNNING;
    }

    m_elapsed += dt;
    m_time = CurveFunction::curveTo(MATH_MAX(0.0f, MATH_MIN(1.0f, m_elapsed/(MATH_MAX(m_duration, FLT_EPSILON)))),
                                    (CurveType)m_curve); // division by 0
    onUpdate(m_target, dt, m_forward ? m_time : (1.0f - m_time));

    if (m_elapsed > m_duration) {
        m_elapsed = m_duration;
        m_status = STATUS_DONE;
        onUpdate(m_target, 0.0, m_forward ? 1.0 : 0.0);
        onEnd(m_target);
    }
}
