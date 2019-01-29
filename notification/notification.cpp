#include "Poco/Notification.h"
#include "Poco/AutoPtr.h"
#include "Poco/NObserver.h"
#include "Poco/NotificationCenter.h"
#include "Poco/Observer.h"
#include <iostream>

using namespace std;
using Poco::AutoPtr;
using Poco::NObserver;
using Poco::Notification;
using Poco::NotificationCenter;
using Poco::Observer;

//支持继承的消息类
class BaseNotification : public Notification {
  public:
    string test = "ckt test: BaseNotification";
};
class SubNotification : public BaseNotification {
  public:
    string test = "ckt test: SubNotification";
};

class Target {
  public:
    // BaseNotification的回调函数
    void handleBase(BaseNotification *pNf) {
        std::cout << "handleBase: " << pNf->name() << "test: " << pNf->test << std::endl;
        pNf->release(); // we got ownership, so we must release
    }
    // SubNotification的回调函数
    void handleSub(const AutoPtr<SubNotification> &pNf) {
        std::cout << "handleSub: " << pNf->name() << "test: " << pNf->test << std::endl;
    }
};

int main(int argc, char **argv) {
    NotificationCenter nc;
    Target target;

    //注册Observer
    nc.addObserver(
        Observer<Target, BaseNotification>(target, &Target::handleBase));

    //注册NObserver
    nc.addObserver(
        NObserver<Target, SubNotification>(target, &Target::handleSub));

    //发送消息
    nc.postNotification(new BaseNotification);
    nc.postNotification(new SubNotification);

    //取消注册
    nc.removeObserver(
        Observer<Target, BaseNotification>(target, &Target::handleBase));
    nc.removeObserver(
        NObserver<Target, SubNotification>(target, &Target::handleSub));
    return 0;
}