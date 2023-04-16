#include <thread>
#include <iostream>
#include <atomic>
#include <vector>

using namespace std;

class SpinLock
{
    public:
        SpinLock() : oFlag(ATOMIC_FLAG_INIT) {}
        void lock() { while(oFlag.test_and_set()){} }
        void unlock() { oFlag.clear(); }
    private:
        atomic_flag oFlag;
};

int main()
{
    SpinLock lock;
    auto count = 0;

    vector<thread> threads;
    auto i = 0;
    while (i++ < 100) {
        threads.emplace_back([](SpinLock &lock, int &count) {
            auto i = 0;
            while (i++ < 100) {
                lock.lock();
                count++;
                cout << count << endl;
                lock.unlock();
            }
        }, std::ref(lock), std::ref(count));
    }

    for (auto& thread : threads) {
        thread.join();
    }
}