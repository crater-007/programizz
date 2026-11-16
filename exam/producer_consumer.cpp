#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <cerrno>
#include <cstdlib>

using namespace std;

const int BUFFER_SIZE = 5;
const int PRODUCER_COUNT = 2;
const int CONSUMER_COUNT = 2;
const int ITEMS_PER_PRODUCER = 5;

int buffer[BUFFER_SIZE];
int in = 0;
int outIndex = 0;

sem_t mutexSem;
sem_t emptySlots;
sem_t fullSlots;

void waitSemaphore(sem_t* semaphore) {
    while (sem_wait(semaphore) == -1) {
        if (errno != EINTR) {
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }
    }
}

void signalSemaphore(sem_t* semaphore) {
    if (sem_post(semaphore) == -1) {
        perror("sem_post");
        exit(EXIT_FAILURE);
    }
}

void* producer(void* argument) {
    int id = *static_cast<int*>(argument);

    for (int i = 0; i < ITEMS_PER_PRODUCER; ++i) {
        int item = id * 100 + i + 1;

        waitSemaphore(&emptySlots);
        waitSemaphore(&mutexSem);

        buffer[in] = item;
        cout << "Producer " << id << " produced: " << item << endl;
        in = (in + 1) % BUFFER_SIZE;

        signalSemaphore(&mutexSem);
        signalSemaphore(&fullSlots);

        usleep(50000);
    }

    return nullptr;
}

void* consumer(void* argument) {
    int id = *static_cast<int*>(argument);

    for (int i = 0; i < ITEMS_PER_PRODUCER; ++i) {
        waitSemaphore(&fullSlots);
        waitSemaphore(&mutexSem);

        int item = buffer[outIndex];
        cout << "Consumer " << id << " consumed: " << item << endl;
        outIndex = (outIndex + 1) % BUFFER_SIZE;

        signalSemaphore(&mutexSem);
        signalSemaphore(&emptySlots);

        usleep(80000);
    }

    return nullptr;
}

int main() {
    pthread_t producers[PRODUCER_COUNT];
    pthread_t consumers[CONSUMER_COUNT];
    int producerIds[PRODUCER_COUNT];
    int consumerIds[CONSUMER_COUNT];

    sem_init(&mutexSem, 0, 1);
    sem_init(&emptySlots, 0, BUFFER_SIZE);
    sem_init(&fullSlots, 0, 0);

    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        consumerIds[i] = i + 1;
        pthread_create(&consumers[i], nullptr, consumer, &consumerIds[i]);
    }

    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        producerIds[i] = i + 1;
        pthread_create(&producers[i], nullptr, producer, &producerIds[i]);
    }

    for (int i = 0; i < PRODUCER_COUNT; ++i) {
        pthread_join(producers[i], nullptr);
    }

    for (int i = 0; i < CONSUMER_COUNT; ++i) {
        pthread_join(consumers[i], nullptr);
    }

    sem_destroy(&mutexSem);
    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);

    cout << "Producer-Consumer problem completed." << endl;
    return 0;
}
