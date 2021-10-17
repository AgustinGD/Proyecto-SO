
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


#define MIN 0
#define MAX_PRIMERA 2
#define MAX_BUSINESS 3
#define MAX_TURISTA 5

#define CANT_PRIMERA 3
#define CANT_BUSINESS 4
#define CANT_TURISTA 6

sem_t sem_primera, sem_business, sem_turista;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t hilos_primera[CANT_PRIMERA ];
pthread_t hilos_business[CANT_BUSINESS];
pthread_t hilos_turista[CANT_TURISTA];


/*
 * check = 1 si no hay espacio, 0 caso contrario
 */
int check_primera(){
	int check = 0;
	
	if(sem_trywait(&sem_primera) == 0){
		// si habia espacio, devuelvo el que quite
		sem_post(&sem_primera);	
	} else{
		// si no hay espacio
		check = 1;
	}
	
	return check;	
}

/*
 * check = 1 si no hay espacio, 0 caso contrario
 */
int check_business(){
	int check = 0;
	
	if(sem_trywait(&sem_business) == 0){
		// si habia espacio, devuelvo el que quite
		sem_post(&sem_business);	
	} else{
		// si no hay espacio
		check = 1;
	}
	
	return check;	
}
/*
 * check = 1 si no hay espacio, 0 caso contrario
 */
int check_turista(){
	int check = 0;
	
	if(sem_trywait(&sem_turista) == 0){
		// si habia espacio, devuelvo el que quite
		sem_post(&sem_turista);	
	} else{
		// si no hay espacio
		check = 1;
	}
	
	return check;	
}

void vaciar_asientos(){
		// devuelvo los asientos de cada area
		for(int i = 0; i< MAX_PRIMERA; i++){
			sem_post(&sem_primera);		
		}
		
		for(int i = 0; i< MAX_BUSINESS; i++){
			sem_post(&sem_business);		
		}
		
		for(int i = 0; i< MAX_TURISTA; i++){
			sem_post(&sem_turista);		
		}
}

int check_bajar_barco(){
	
	//pthread_mutex_lock(&mutex);
	int check = check_business() && check_primera() && check_turista();
		// ver si ya estan todos los asientos llenos
		if(check){
				printf("*********************************************\n");
				printf("Barco lleno, partiendo...\n");
				printf("*********************************************\n");
				sleep(2);
				vaciar_asientos();
				printf("*********************************************\n");
				printf("Se vacio el barco, esperando nuevos pasajeros\n");
				printf("*********************************************\n");				
		}
	return check;				
	//pthread_mutex_unlock(&mutex);	
}

void *barco_primera() {
	
	for (int i = 0; i<2; i++){	
		
		pthread_mutex_lock(&mutex);		
		
		if(sem_trywait(&sem_primera) == 0){
			printf("pasajero primera sube\n");
			check_bajar_barco();			
		}else{			
			pthread_mutex_unlock(&mutex);
			sem_wait(&sem_primera);					
		}
		
		//check_bajar_barco();
		
		pthread_mutex_unlock(&mutex);			
	}
	
	return NULL;
}

void *barco_business() {
	
for (int i = 0; i<2; i++){	
		
		pthread_mutex_lock(&mutex);		
		
		if(sem_trywait(&sem_business) == 0){
			printf("pasajero business sube\n");
			check_bajar_barco();			
		}else{
			pthread_mutex_unlock(&mutex);
			sem_wait(&sem_business);	
					
		}
		
		//check_bajar_barco();
		
		pthread_mutex_unlock(&mutex);			
	}
	
	return NULL;
}

void *barco_turista() {
	
	for (int i = 0; i<2; i++){	
			
		pthread_mutex_lock(&mutex);		
		
		if(sem_trywait(&sem_turista) == 0){
			printf("pasajero turista sube\n");
			check_bajar_barco();			
		}else{			
			pthread_mutex_unlock(&mutex);
			sem_wait(&sem_turista);			
		}
		
		//check_bajar_barco();
		
		pthread_mutex_unlock(&mutex);			
	}
	
	return NULL;
}

int main(int argc, char **argv)
{		
	sem_init(&sem_primera, 0, MAX_PRIMERA);
	sem_init(&sem_business, 0, MAX_BUSINESS);
	sem_init(&sem_turista, 0, MAX_TURISTA);
	
	// Creacion de hilos primera
	for(int i=0; i < CANT_PRIMERA; i++){
		pthread_create(&hilos_primera[i], NULL, barco_primera, NULL);		
	}	
	// Creacion de hilos business
	for(int i=0; i < CANT_BUSINESS; i++){
		pthread_create(&hilos_business[i], NULL, barco_business, NULL);		
	}
	// Creacion de hilos turista
	for(int i=0; i < CANT_TURISTA; i++){
		pthread_create(&hilos_turista[i], NULL, barco_turista, NULL);		
	}
	
	// Joind de hilos primera
	for(int i=0; i< CANT_PRIMERA; i++)
		pthread_join(hilos_primera[i], NULL);
		
	// Joind de hilos business
	for(int i=0; i< CANT_BUSINESS; i++)
		pthread_join(hilos_business[i], NULL);
		
	// Joind de hilos turista
	for(int i=0; i< CANT_TURISTA; i++)
		pthread_join(hilos_turista[i], NULL);		
		
	return 0;
}
