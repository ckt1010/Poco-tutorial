#include "Poco/ThreadLocal.h"
#include "Poco/Data/Session.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/Timer.h"
#include "Poco/AutoPtr.h"
#include "Poco/NObserver.h"
#include "Poco/Notification.h"
#include "Poco/NotificationCenter.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <fstream>
#include <iterator>

using namespace Poco::Data::Keywords;
using Poco::Thread;
using std::cout;
using std::endl;
using std::string;
using Poco::Timer;
using Poco::TimerCallback;
using Poco::AutoPtr;
using Poco::NObserver;
using Poco::Notification;
using Poco::NotificationCenter;

class Message : public Notification {
  public:
    Message(bool cond):condition(cond) {}
	bool condition;
};

class TimerExample {
  public:
    void onTimer(Poco::Timer &timer) {
        char buff[20];
        time_t now = time(NULL);
        strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
        cout << "onTimer called: " << buff << endl;
    }
};

class Target {
  public:
    Target() {
        timer = new Poco::Timer(1000);
    }

    void handleSub(const AutoPtr<Message> &pNf) {
        if (true == pNf->condition) {
            char buff[20];
            time_t now = time(NULL);
            strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
            cout << "condition enter: " << buff << endl;
            timer->stop();
            timer->start(TimerCallback<TimerExample>(te, &TimerExample::onTimer));
        }
    }

  private:
    TimerExample te;
    Timer *timer;
    
};

int main(int argc, char **argv) {
    Target tg;
    NotificationCenter nc;
    nc.addObserver(NObserver<Target, Message>(tg, &Target::handleSub));
    char buff[20];
    
    while (1) {
        time_t now = time(NULL);
        strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
        cout << "post notification now: " << buff << endl;
        try{
        nc.postNotification(new Message(true));
        }catch(Poco::IllegalStateException &ex) {
            cout << ex.displayText() << endl;
        }
        Thread::sleep(5000);
    }
    return 0;
}
