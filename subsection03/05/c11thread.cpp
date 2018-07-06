#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <time.h>
#define RUN_TIME 1000*1000
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
    time_t start, end;
    start = time(NULL);
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
    end = time(NULL);
    printf("start=%ld,end=%ld,time: %ld \n", start, end, end - start);
    return 0;

}
