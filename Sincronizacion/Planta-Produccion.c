#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t semA, semB, semC, semD, semE, semF;
pthread_t hiloA, hiloB, hiloC, hiloD, hiloE, hiloF;


void *elementoA() {
	while(1){
		sem_wait(&semA);
		
		usleep(2 * 100000);
		printf("A\n");
		
		sem_post(&semB);
		
	}
	return NULL;
}

void *elementoB() {
	while(1){
		sem_wait(&semB);		
		usleep(1 * 100000);
		printf("B\n");
		
		sem_post(&semC);
		
		sem_wait(&semB);		
		usleep(1 * 100000);
		printf("B\n");
		
		sem_post(&semD);
		
	}	
	return NULL;
}

void *elementoC() {
	while(1){
		sem_wait(&semC);
		
		usleep(3 * 100000);
		printf("C\n");
		
		sem_post(&semE);
		
	}	
	return NULL;
}

void *elementoD() {
	while(1){
		sem_wait(&semD);
		
		usleep(7 * 100000);
		printf("D\n");
		
		sem_post(&semE);
		
	}	
	return NULL;
}

void *elementoE() {
	while(1){
		sem_wait(&semE);
		
		usleep(2 * 100000);
		printf("E\n");
		
		sem_post(&semF);
		
	}	
	return NULL;
}

void *elementoF() {
	while(1){
		sem_wait(&semF);
		
		usleep(3 * 100000);
		printf("F\n");
		
		sem_post(&semA);
		
	}	
	return NULL;
}

int main(int argc, char **argv)
{		
	sem_init(&semA, 0, 1);
	sem_init(&semB, 0, 0);
	sem_init(&semC, 0, 0);
	sem_init(&semD, 0, 0);
	sem_init(&semE, 0, 0);
	sem_init(&semF, 0, 0);
	
	pthread_create(&hiloA, NULL, elementoA, NULL);
	pthread_create(&hiloB, NULL, elementoB, NULL);
	pthread_create(&hiloC, NULL, elementoC, NULL);
	pthread_create(&hiloD, NULL, elementoD, NULL);
	pthread_create(&hiloE, NULL, elementoE, NULL);
	pthread_create(&hiloF, NULL, elementoF, NULL);
	
	pthread_join(hiloA, NULL);
	pthread_join(hiloB, NULL);
	pthread_join(hiloC, NULL);
	pthread_join(hiloD, NULL);
	pthread_join(hiloE, NULL);
	pthread_join(hiloF, NULL);
		
	return 0;
}
