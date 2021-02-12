#ifndef ACTIONCACHE_H
#define ACTIONCACHE_H

#include "Action.h"
class Action;
typedef std::list<Action*> ActionList;
class ActionCache
{
public:
    ActionCache();
    virtual ~ActionCache();

    void addAction(Action* action);
    void removeAction(Action* action);
    bool isRunning();
    void update(float dt);
    void stopAll();
protected:
    ActionList m_actions;
};

#endif // ACTIONCACHE_H
