#include "ActionCache.h"
#include "Log.h"

ActionCache::ActionCache()
{

}

ActionCache::~ActionCache()
{
    for (ActionList::iterator it = m_actions.begin(); it != m_actions.end(); ++it) {
        if (Action* action = *it) {
            SAFE_DELETE(action);
        }
    }

    m_actions.clear();
}

void ActionCache::addAction(Action* action)
{
    if (!action)
        return;

    for (ActionList::iterator it = m_actions.begin(); it != m_actions.end(); ++it) {
        if (action == *it) {
            return;
        }
    }
    m_actions.push_back(action);
}

void ActionCache::removeAction(Action* action)
{
    if (!action)
        return;

    for (ActionList::iterator it = m_actions.begin(); it != m_actions.end(); ++it) {
        if (action == *it) {
            SAFE_DELETE(action);
            m_actions.erase(it);
            return;
        }
    }
}

bool ActionCache::isRunning()
{
    for (ActionList::iterator it = m_actions.begin(); it != m_actions.end(); ++it) {
        Action* action = *it;
        if (action && !action->isDone()) {
            return true;
        }
    }
    return false;
}

void ActionCache::update(float dt)
{
    if (!isRunning()) {
        return;
    }

    // LOG_BASE("update:%.3f", dt);
    ActionList actions;
    actions.insert(actions.begin(), m_actions.begin(), m_actions.end());
    for(ActionList::iterator it = actions.begin(); it != actions.end(); ++it) {
        Action* action = *it;
        if (action) {
            action->update(dt);
        }
    }
}

void ActionCache::stopAll()
{
    ActionList actions;
    actions.insert(actions.begin(), m_actions.begin(), m_actions.end());

    for (ActionList::iterator it = actions.begin(); it != actions.end(); ++it) {
        Action* action = *it;
        if (action && !action->isDone()) {
            action->update(action->m_duration);
        }
    }
}
