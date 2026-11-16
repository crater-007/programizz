#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

class DiningPhilosophers {
private:
    enum State { THINKING, HUNGRY, EATING };

    static const int COUNT = 5;
    State state[COUNT];
    pthread_mutex_t lock;
    pthread_cond_t condition[COUNT];

    int left(int i) const {
        return (i + COUNT - 1) % COUNT;
    }

    int right(int i) const {
        return (i + 1) % COUNT;
    }

    void test(int i) {
        if (state[i] == HUNGRY &&
            state[left(i)] != EATING &&
            state[right(i)] != EATING) {
            state[i] = EATING;
            pthread_cond_signal(&condition[i]);
        }
    }

public:
    DiningPhilosophers() {
        pthread_mutex_init(&lock, nullptr);

        for (int i = 0; i < COUNT; ++i) {
            state[i] = THINKING;
            pthread_cond_init(&condition[i], nullptr);
        }
    }

    ~DiningPhilosophers() {
        for (int i = 0; i < COUNT; ++i) {
            pthread_cond_destroy(&condition[i]);
        }

        pthread_mutex_destroy(&lock);
    }

    void pick_up(int i) {
        pthread_mutex_lock(&lock);

        state[i] = HUNGRY;
        cout << "Philosopher " << i + 1 << " is hungry" << endl;

        test(i);

        while (state[i] != EATING) {
            pthread_cond_wait(&condition[i], &lock);
        }

        cout << "Philosopher " << i + 1 << " starts eating" << endl;
        pthread_mutex_unlock(&lock);
    }

    void put_down(int i) {
        pthread_mutex_lock(&lock);

        state[i] = THINKING;
        cout << "Philosopher " << i + 1 << " stops eating" << endl;

        test(left(i));
        test(right(i));

        pthread_mutex_unlock(&lock);
    }
};

struct PhilosopherArgs {
    DiningPhilosophers* table;
    int id;
};

void* philosopher(void* argument) {
    PhilosopherArgs* args = static_cast<PhilosopherArgs*>(argument);

    for (int i = 0; i < 3; ++i) {
        cout << "Philosopher " << args->id + 1 << " is thinking" << endl;
        usleep(70000);

        args->table->pick_up(args->id);
        usleep(100000);
        args->table->put_down(args->id);
    }

    return nullptr;
}

int main() {
    const int count = 5;
    DiningPhilosophers table;
    pthread_t threads[count];
    PhilosopherArgs arguments[count];

    for (int i = 0; i < count; ++i) {
        arguments[i] = {&table, i};
        pthread_create(&threads[i], nullptr, philosopher, &arguments[i]);
    }

    for (int i = 0; i < count; ++i) {
        pthread_join(threads[i], nullptr);
    }

    cout << "Dining Philosophers problem completed." << endl;
    return 0;
}
