#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/shm.h>

#define BUFFER_SIZE 30

#define KEY_BUFFER ((key_t) 1243)
#define SEGSIZE_BUFFER (BUFFER_SIZE * sizeof(int))

#define KEY_IN ((key_t) 1244)
#define SEGZIE_IN sizeof(int)

#define KEY_OUT ((key_t) 1245)
#define SEGZIE_OUT sizeof(int)

#define KEY_MUTEX ((key_t) 1246)
#define SEGZIE_MUTEX sizeof(pthread_mutex_t)


int buffer[BUFFER_SIZE];

void productor(){
	int id1 = shmget(KEY_BUFFER, SEGSIZE_BUFFER, 0);
	int id2 = shmget(KEY_MUTEX, SEGZIE_MUTEX, 0);
	int id3 = shmget(KEY_IN, SEGZIE_IN, 0);
	int id4 = shmget(KEY_OUT, SEGZIE_OUT, 0);
	
	
	int* buffer = (int *) shmat(id1,0,0);
	pthread_mutex_t* mutex = (pthread_mutex_t*) shmat(id2,0,0);
	
	int* in = (int *) shmat(id3,0,0);
	int* out = (int *) shmat(id4,0,0);
	
	while (1){
		pthread_mutex_lock(mutex);
			if( ((*in + 1) % BUFFER_SIZE) != *out){
				buffer[*in] = rand() % 100;
				*in = (*in + 1) % BUFFER_SIZE;
			}	
		
		pthread_mutex_unlock(mutex);
	}
}

void consumidor(int id1, int id2, int id3, int id4){
	
	
	int* buffer = (int *) shmat(id1,0,0);
	pthread_mutex_t* mutex = (pthread_mutex_t*) shmat(id2,0,0);
	
	int* in = (int *) shmat(id3,0,0);
	int* out = (int *) shmat(id4,0,0);
	
	while(1){
		pthread_mutex_lock(mutex);
			if( *in != *out){
				printf("se lee del buffer: %d/n", buffer[*out]);
				*out = (*out + 1) % BUFFER_SIZE;
			}
		pthread_mutex_unlock(mutex);
	}
}

int main(int argc, char **argv)
{		

	int id1 = shmget(KEY_BUFFER, SEGSIZE_BUFFER, IPC_CREAT|0666);
	int id2 = shmget(KEY_MUTEX, SEGZIE_MUTEX, IPC_CREAT|0666);
	int id3 = shmget(KEY_IN, SEGZIE_IN, IPC_CREAT|0666);
	int id4 = shmget(KEY_OUT, SEGZIE_OUT, IPC_CREAT|0666);
	
	// inicializacion del mutex y punteros del buffer
	pthread_mutex_t* mutex = (pthread_mutex_t*) shmat(id1,0,0);
	*mutex = PTHREAD_MUTEX_INITIALIZER;
	
	int* in = (int *) shmat(id3,0,0);
	int* out = (int *) shmat(id4,0,0);
	
	*in = 0;
	*out = 0;
	
	int pid = fork();
	if (pid == 0){
		productor();
	}else{
		consumidor(id1, id2, id3, id4);
	}	
}
