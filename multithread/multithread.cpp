#include "Poco/Runnable.h"
#include "Poco/Thread.h"
#include "Poco/ThreadLocal.h"
#include <iostream>

class Counter : public Poco::Runnable {
    void run() {
        static Poco::ThreadLocal<int> tls;
        for (*tls = 0; *tls < 10; ++(*tls)) {
            std::cout << *tls << std::endl;
        }
    }
};

int main(int argc, char **argv) {
    Counter counter;
    Poco::Thread t1;
    Poco::Thread t2;
    t1.start(counter);
    t2.start(counter);
    t1.join();
    t2.join();
    return 0;
}