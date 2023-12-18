#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "buffer.h"

pthread_mutex_t mutex;
sem_t full, empty;
buffer_product buffer[BUFFER_SIZE];
int counter;
pthread_t tid;
pthread_attr_t attr;
void *producer(void *param);
void *consumer(void *param);
int insert_product(buffer_product);
int remove_product(buffer_product*) ;


void initializeData() {
	pthread_mutex_init(&mutex, NULL);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	pthread_attr_init(&attr);
	counter = 0;
}


void *producer(void *param) {
	buffer_product product;

	while (1) {
		int rNum = rand() / 100000000;
		sleep(rNum);
		product = rand()%100;
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);
		if (insert_product(product)) {
			fprintf(stderr, " Error occurs in producer report\n");
		}
		else {
			printf("producer produced: %d\n", product);
		}
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}


void *consumer(void *param) {
	buffer_product product;
	while (1) {
		int rNum = rand() / 1000000000;
		sleep(rNum);
		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		if (remove_product(&product)) {
			fprintf(stderr, "Error occurs in consumer report\n");
		}
		else {
			printf("consumer consumed: %d\n", product);
		}
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}


int insert_product(buffer_product product) {
	if (counter < BUFFER_SIZE) {
		buffer[counter] = product;
		counter++;
		return 0;
	}
	else {
		return -1;
	}
}


int remove_product(buffer_product *product) {
	if (counter > 0) {
		*product = buffer[(counter - 1)];
		counter--;
		return 0;
	}
	else {
		return -1;
	}
}

int main(int argc, char *argv[]) {
	int i;
	 if(argc != 4) {
      fprintf(stderr, "USAGE:./F <INT> <INT> <INT>\n");
      printf("Program is leaving\n");
      exit(0);
   }
	int sleeptime = atoi(argv[1]);
	int numProd = atoi(argv[2]);
	int numCons = atoi(argv[3]);

	initializeData();

	for (i = 0; i < numProd; i++) {
		pthread_create(&tid, &attr, producer, NULL);
	}
	for (i = 0; i < numCons; i++) {
		pthread_create(&tid, &attr, consumer, NULL);
	}
	sleep(sleeptime);
	printf("This program is existing.\n");
	exit(0);
}
