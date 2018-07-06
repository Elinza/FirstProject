#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>

#define RUN_TIME 1024*1024
using namespace std;


std::atomic<long> p(0);
std::atomic<long> q(0);
std::atomic<long> r(0);
std::atomic<long> s(0);

void Thread1()
{
    int i = 0;
    for (i = 0; i < RUN_TIME; i++) {
        p++;
        q++;
        r++;
        s++;
    }
};

void Thread2()
{
    int i = 0;
    for (i = 0; i < RUN_TIME; i++) {
        p++;
        q++;
        s++;
    }
};

void Thread3()
{
    int i = 0;
    for (i = 0; i < RUN_TIME; i++) {
        p++;
        r++;
        s++;
    }
};

void Thread4()
{
    int i = 0;
    for (i = 0; i < RUN_TIME; i++) {
        q++;
        s++;
    }
};

int main()
{
    clock_t startClock = clock();
#if 0
    std::vector<std::thread>tid1;
    std::vector<std::thread>tid2;
    std::vector<std::thread>tid3;
    std::vector<std::thread>tid4;
    tid1.push_back(std::thread(Thread1));
    tid2.push_back(std::thread(Thread2));
    tid3.push_back(std::thread(Thread3));
    tid4.push_back(std::thread(Thread4));
#endif
    std::thread t1(Thread1);
    std::thread t2(Thread2);
    std::thread t3(Thread3);
    std::thread t4(Thread4);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    cout<<"p = "<<p<<endl;
    cout<<"q = "<<q<<endl;
    cout<<"r = "<<r<<endl;
    cout<<"s = "<<s<<endl;

    clock_t endClock = clock();
    cout<<"耗时："<<endClock-startClock<<"ms"<<endl;

    return 0;

}
