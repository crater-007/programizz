#include <iostream>
#include <thread>
#include <atomic>
using namespace std;

atomic<bool> flag[2] = {false, false};
atomic<int> turn(0);
int sharedCounter = 0;
atomic<int> activeThreads(0);
atomic<int> maximumActive(0);

void updateMaximum(int value) {
    int oldValue = maximumActive.load();
    while (value > oldValue && !maximumActive.compare_exchange_weak(oldValue, value)) {
    }
}

void peterson(int id) {
    int other = 1 - id;

    for (int i = 0; i < 100000; i++) {
        flag[id].store(true);
        turn.store(other);

        while (flag[other].load() && turn.load() == other) {
        }

        int active = activeThreads.fetch_add(1) + 1;
        updateMaximum(active);

        sharedCounter++;

        activeThreads.fetch_sub(1);
        flag[id].store(false);
    }
}

int main() {
    thread t1(peterson, 0);
    thread t2(peterson, 1);

    t1.join();
    t2.join();

    cout << "Expected counter value: 200000\n";
    cout << "Actual counter value: " << sharedCounter << '\n';
    cout << "Maximum threads inside critical section: " << maximumActive.load() << '\n';

    if (sharedCounter == 200000 && maximumActive.load() == 1) {
        cout << "Peterson's Solution verified: mutual exclusion is maintained.\n";
    } else {
        cout << "Verification failed.\n";
    }

    return 0;
}
