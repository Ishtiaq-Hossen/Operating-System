#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

// Semaphores to control access to shared resources
sem_t mutex, writeBlock;

int data = 0;  // Shared data to be read and written
int readersCount = 0;  // Count of active readers

// Function for reader threads
void *reader(void *arg)
{
    int readerId = *(int *)arg;
    printf("Reader %d is trying to read.\n", readerId);

    while (1)
    {
        sem_wait(&mutex);
        readersCount++;

        // If the first reader, block writers
        if (readersCount == 1)
        {
            sem_wait(&writeBlock);
        }

        sem_post(&mutex);

        printf("Reader %d is reading data: %d\n", readerId, data);
        usleep(1000000); // Simulate reading by sleeping for 1 second

        sem_wait(&mutex);
        readersCount--;

        // If the last reader, release the writeBlock semaphore
        if (readersCount == 0)
        {
            sem_post(&writeBlock);
        }

        sem_post(&mutex);

        usleep(1000000); // Sleep for 1 second before reading again
    }

    return NULL;
}

// Function for writer threads
void *writer(void *arg)
{
    int writerId = *(int *)arg;
    printf("Writer %d is trying to write.\n", writerId);

    while (1)
    {
        sem_wait(&writeBlock);

        printf("Writer %d is writing data.\n", writerId);
        data++; // Incrementing data to simulate writing

        sem_post(&writeBlock);

        usleep(2000000); // Sleep for 2 seconds before writing again
    }

    return NULL;
}

int main()
{
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int readerIds[NUM_READERS], writerIds[NUM_WRITERS];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&writeBlock, 0, 1);

    int i;

    // Create reader threads
    for (i = 0; i < NUM_READERS; i++)
    {
        readerIds[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &readerIds[i]);
    }

    // Create writer threads
    for (i = 0; i < NUM_WRITERS; i++)
    {
        writerIds[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writerIds[i]);
    }

    // Wait for reader threads to finish
    for (i = 0; i < NUM_READERS; i++)
    {
        pthread_join(readers[i], NULL);
    }

    // Wait for writer threads to finish
    for (i = 0; i < NUM_WRITERS; i++)
    {
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&writeBlock);

    return 0;
}
