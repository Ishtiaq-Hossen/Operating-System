#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex for critical sections
pthread_cond_t conditions[NUM_PHILOSOPHERS];        // Condition variables for signaling
int forks[NUM_PHILOSOPHERS];                         // Array to represent the state of forks

// Function to initialize resources
void initialize() {
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_init(&conditions[i], NULL);    // Initialize condition variables
        forks[i] = 1;                               // Initially, all forks are available
    }
}

// Function for a philosopher to pick up forks
void takeForks(int philosopherNumber) {
    pthread_mutex_lock(&mutex);  // Acquire the mutex lock to enter the critical section

    // While either of the forks is not available, wait
    while (!forks[philosopherNumber] || !forks[(philosopherNumber + 1) % NUM_PHILOSOPHERS]) {
        pthread_cond_wait(&conditions[philosopherNumber], &mutex);
    }

    forks[philosopherNumber] = 0;                                      // Mark the left fork as unavailable
    forks[(philosopherNumber + 1) % NUM_PHILOSOPHERS] = 0;             // Mark the right fork as unavailable
    pthread_mutex_unlock(&mutex);                                       // Release the mutex lock
}

// Function for a philosopher to return forks
void returnForks(int philosopherNumber) {
    pthread_mutex_lock(&mutex);  // Acquire the mutex lock to enter the critical section

    forks[philosopherNumber] = 1;                                      // Mark the left fork as available
    forks[(philosopherNumber + 1) % NUM_PHILOSOPHERS] = 1;             // Mark the right fork as available

    pthread_cond_signal(&conditions[philosopherNumber]);               // Signal the left neighbor
    pthread_cond_signal(&conditions[(philosopherNumber + 1) % NUM_PHILOSOPHERS]);  // Signal the right neighbor
    pthread_mutex_unlock(&mutex);                                       // Release the mutex lock
}

// Function for a philosopher thread
void *philosopher(void *arg) {
    int philosopherNumber = *(int *)arg;  // Get the philosopher's number

    while (1) {
        // Simulate philosopher thinking
        printf("Philosopher %d is thinking\n", philosopherNumber);
        usleep(rand() % 300 + 100);  // Sleep for a random period between 100 and 400 milliseconds

        // Simulate philosopher being hungry and trying to eat
        takeForks(philosopherNumber);

        // Simulate philosopher eating
        printf("Philosopher %d is eating\n", philosopherNumber);
        usleep(rand() % 300 + 100);  // Sleep for a random period between 100 and 400 milliseconds

        // Simulate philosopher finishing eating and returning forks
        returnForks(philosopherNumber);
    }

    return NULL;
}

// Main function
int main() {
    srand(time(NULL));  // Seed for random number generation

    initialize();  // Initialize resources

    pthread_t philosophers[NUM_PHILOSOPHERS];  // Thread IDs for philosophers
    int philosopherNumbers[NUM_PHILOSOPHERS];

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopherNumbers[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopherNumbers[i]);
    }

    // Note: This program runs indefinitely and needs to be terminated manually

    // Wait for philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}
