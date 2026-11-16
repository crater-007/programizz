#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
using namespace std;

volatile bool lockFlag = false;
atomic<int> activeThreads(0);
atomic<int> maximumActive(0);
long long sharedCounter = 0;

void updateMaximum(int value) {
    int oldValue = maximumActive.load();
    while (value > oldValue && !maximumActive.compare_exchange_weak(oldValue, value)) {
    }
}

void work() {
    for (int i = 0; i < 100000; i++) {
        while (lockFlag) {
        }

        lockFlag = true;

        int active = activeThreads.fetch_add(1) + 1;
        updateMaximum(active);

        sharedCounter++;
        this_thread::yield();

        activeThreads.fetch_sub(1);
        lockFlag = false;
    }
}

int main() {
    thread t1(work);
    thread t2(work);

    t1.join();
    t2.join();

    cout << "Expected counter value: 200000\n";
    cout << "Actual counter value: " << sharedCounter << '\n';
    cout << "Maximum threads inside critical section: " << maximumActive.load() << '\n';

    if (maximumActive.load() > 1 || sharedCounter != 200000) {
        cout << "Race condition was observed in this run.\n";
    } else {
        cout << "No visible failure in this run. Run again to observe the race.\n";
    }

    return 0;
}
