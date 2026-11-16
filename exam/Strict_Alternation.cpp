#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
using namespace std;

atomic<int> turn(0);
atomic<bool> firstEntryDone(false);
atomic<bool> stopWaiting(false);

void processZero() {
    cout << "Thread 0 enters its first critical section.\n";
    turn.store(1);
    firstEntryDone.store(true);

    cout << "Thread 0 wants to enter its critical section again.\n";
    cout << "Thread 0 is waiting because turn is 1.\n";

    while (turn.load() != 0 && !stopWaiting.load()) {
        this_thread::yield();
    }

    if (stopWaiting.load()) {
        cout << "Thread 0 is released only to end the demonstration.\n";
    } else {
        cout << "Thread 0 enters its second critical section.\n";
    }
}

void processOne() {
    while (!firstEntryDone.load()) {
        this_thread::yield();
    }

    cout << "Thread 1 is in its remainder section and does not want the CPU.\n";
}

int main() {
    thread t0(processZero);
    thread t1(processOne);

    this_thread::sleep_for(chrono::seconds(2));

    cout << "\nProgress is violated: Thread 0 is still waiting although Thread 1 is not interested.\n";
    stopWaiting.store(true);

    t0.join();
    t1.join();

    return 0;
}
