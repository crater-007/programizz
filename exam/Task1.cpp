#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

using namespace std;

timed_mutex MutexA;
timed_mutex MutexB;
mutex printMutex;
atomic<int> ready(0);

void showMessage(const string& message) {
    lock_guard<mutex> lock(printMutex);
    cout << message << endl;
}

void Thread_1() {
    MutexA.lock();
    showMessage("Thread_1 acquired MutexA.");
    ready++;

    while (ready.load() < 2) {
        this_thread::yield();
    }

    this_thread::sleep_for(chrono::milliseconds(100));
    showMessage("Thread_1 is requesting MutexB.");

    if (MutexB.try_lock_for(chrono::milliseconds(500))) {
        showMessage("Thread_1 acquired MutexB.");
        MutexB.unlock();
    } else {
        showMessage("Thread_1 is blocked because MutexB is held by Thread_2.");
    }

    MutexA.unlock();
}

void Thread_2() {
    MutexB.lock();
    showMessage("Thread_2 acquired MutexB.");
    ready++;

    while (ready.load() < 2) {
        this_thread::yield();
    }

    this_thread::sleep_for(chrono::milliseconds(100));
    showMessage("Thread_2 is requesting MutexA.");

    if (MutexA.try_lock_for(chrono::milliseconds(500))) {
        showMessage("Thread_2 acquired MutexA.");
        MutexA.unlock();
    } else {
        showMessage("Thread_2 is blocked because MutexA is held by Thread_1.");
    }

    MutexB.unlock();
}

void correctedThread(const string& name) {
    unique_lock<timed_mutex> lockA(MutexA);
    showMessage(name + " acquired MutexA.");

    this_thread::sleep_for(chrono::milliseconds(100));

    unique_lock<timed_mutex> lockB(MutexB);
    showMessage(name + " acquired MutexB.");

    showMessage(name + " completed successfully.");
}

int main() {
    cout << "Deadlock Simulation" << endl;

    thread t1(Thread_1);
    thread t2(Thread_2);

    t1.join();
    t2.join();

    cout << "\nDeadlock Prevention Using a Fixed Lock Order" << endl;

    thread t3(correctedThread, "Thread_1");
    thread t4(correctedThread, "Thread_2");

    t3.join();
    t4.join();

    cout << "\nBoth threads completed without deadlock." << endl;

    return 0;
}
