#ifndef GESTURETIMER_H
#define GESTURETIMER_H

class GestureRecognizer;
class GestureTimerPrivate;
class GestureTimer {
public:
	GestureTimer(GestureRecognizer* recognizer, void* data);
	virtual ~GestureTimer();
	GestureRecognizer* getRecognizer();
	void setInterval(unsigned int interval, bool repeat = false);
	void start();
	void stop();
    void restart();
	void* getUserData();
	void setUserData(void* data);
private:
	GestureTimer(const GestureTimer& rhs);
	GestureTimer& operator=(const GestureTimer& rhs);

private:
	GestureTimerPrivate* mPrivate;
};

#endif /* GESTURETIMER_H */
