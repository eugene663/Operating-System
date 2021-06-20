#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define ITER 1000
#define MAX 30
void put(int value);
int get();
void* producer(void* arg);
void* consumer(void* arg);
int count = 0;
int buffer[30];
int fill_cnt = 0;
int use_cnt = 0;
const int sizeofbuffer = 30;
sem_t s;
sem_t fill;
sem_t empty;

int main() {
pthread_t tid1, tid2;
sem_init(&s, 0, 1);
sem_init(&fill, 0, 0);
sem_init(&empty, 0, 30);
pthread_create(&tid1, NULL, producer, NULL);
pthread_create(&tid2, NULL, consumer, NULL);
pthread_join(tid1, NULL);
pthread_join(tid2, NULL);
if (count != 0)
printf("BOOM! counter=%d\n", count);
else
printf("OK counter=%d\n", count);
sem_destroy (&s);
sem_destroy (&empty);
sem_destroy (&fill);
}
void* producer(void* arg) {
int i;
int value;
	for (i=0; i < ITER ; i++) {
		sem_wait(&empty);
		sem_wait(&s);
		value = count;
		printf("%u: %d\n",(unsigned int) pthread_self(), value);
		put(value);
		sem_post(&s);
		sem_post(&fill);
	}
	pthread_exit(NULL);
}
void put(int value) {
	buffer[fill_cnt] = value;
	fill_cnt = (fill_cnt + 1) % MAX;
	count++;
}

void* consumer(void* arg) {
	int i;
       int value;
	for (i=0; i<ITER; i++){
		sem_wait(&fill);
		sem_wait(&s);
		value = count;
		printf("%u: %d\n", (unsigned int) pthread_self(), value);
		get();
		sem_post(&s);
		sem_post(&empty);
	}
	pthread_exit(NULL);
}
int get() {
int tmp = buffer[use_cnt];
use_cnt = (use_cnt + 1) %MAX;
count--;
return tmp;
}
