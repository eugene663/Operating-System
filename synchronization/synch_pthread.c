#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define ITER 1000
#define MAX 30
void* producer(void* arg);
void* consumer(void* arg);
void put(int value);
int get();
int count = 0;
int buffer[MAX];
int fill_cnt = 0;
int use_cnt = 0;
pthread_mutex_t m;
pthread_cond_t fill;
pthread_cond_t empty;
int main() {
	pthread_t tid1, tid2;
	pthread_mutex_init(&m, NULL);
	pthread_create(&tid1, NULL, producer, NULL);
	pthread_create(&tid2, NULL, consumer, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	if (count != 0)
		printf("BOOM! counter=%d\n", count);
	else
		printf("OK counter=%d\n", count);
	pthread_mutex_destroy(&m);
}

void* producer(void* arg) {
	int i, value;
	for(i=0; i <ITER; i++) {
		pthread_mutex_lock(&m);
		value = count;
		 printf("%u: %d\n", (unsigned int)pthread_self(), value);
		while(count == MAX)
			pthread_cond_wait(&empty, &m);
		put(i);
		pthread_cond_signal(&fill);
		pthread_mutex_unlock(&m);
	}
}
void put(int value) {
	buffer[fill_cnt] = value;
	fill_cnt = (fill_cnt + 1) % MAX;
	count++;
}

void* consumer(void* arg) {
	int value;
	int i;
	for(i=0; i <ITER; i++) {
		pthread_mutex_lock(&m);
		value = count;
		printf("%u: %d\n", (unsigned int)pthread_self(), value);
		while (count == 0)
			pthread_cond_wait(&fill, &m);
		value = get();
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&m);
	}
}
int get() {
	int tmp = buffer[use_cnt];
	use_cnt = (use_cnt + 1) % MAX;
	count--;
	return tmp;
}
