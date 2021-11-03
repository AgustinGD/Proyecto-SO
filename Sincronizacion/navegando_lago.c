
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


#define MIN 0
#define MAX_PRIMERA 2
#define MAX_BUSINESS 3
#define MAX_TURISTA 5

#define CANT_PASAJEROS 10

sem_t sem_primera, sem_business, sem_turista, sem_lleno;
pthread_mutex_t mutex_subir = PTHREAD_MUTEX_INITIALIZER;
pthread_t hilos_pasajeros[CANT_PASAJEROS];
pthread_t hilo_vaciar;


/*
 * check = 1 si no hay espacio, 0 caso contrario
 */
int check_area(sem_t sem_area){
	int check = 0;
	
	if(sem_trywait(&sem_area) == 0){
		// si habia espacio, devuelvo el que quite
		sem_post(&sem_area);	
	} else{
		// si no hay espacio
		check = 1;
	}
	
	return check;	
}

int check_bajar_barco(){
	return 	check_area(sem_primera) && check_area(sem_business) && check_area(sem_turista);
}

void *barco_navega(){
	while (1){		
		sem_wait(&sem_lleno);
		
		printf("*********************************************\n");
		printf("Barco lleno, partiendo...\n");
		printf("*********************************************\n");
		
		for (int i = 0; i<11; i++){
			printf("¸.•*¨*•.¸♪¸.•*¨*•.¸♥¸.•*¨*•.¸♪¸.•*¨*•.¸♥¸.•*¨*•.¸♪¸.•*¨*•.¸\n");
			usleep(0.2 * 1000000);
		}
		printf("*********************************************\n");
		printf("Llegada al puerto\n");
			 
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


		printf("Se vacio el barco, esperando nuevos pasajeros\n");
		printf("*********************************************\n");
		sleep(1);
		pthread_mutex_unlock(&mutex_subir);
	}	
	return NULL;
}

void *pasajero() {
	int r;
	while(1){
		
		pthread_mutex_lock(&mutex_subir);		
		
		r = random() % 3;
		switch (r){
			case 0:				
				if (sem_trywait(&sem_primera) == 0)
					printf("@ Sube pasajero Primera\n");				
			break;
			
			case 1:
				if (sem_trywait(&sem_business) == 0)
					printf("^ Sube pasajero Business\n");
			break;
			
			case 2:
				if (sem_trywait(&sem_turista) == 0)
					printf("# Sube pasajero Turista\n");
			break;				
		}		
		usleep(0.1 * 1000000);
		
		if (check_bajar_barco())
			sem_post(&sem_lleno);
		else
			pthread_mutex_unlock(&mutex_subir);		
	}
	
	return NULL;
}

int main(int argc, char **argv)
{		
	sem_init(&sem_primera, 0, MAX_PRIMERA);
	sem_init(&sem_business, 0, MAX_BUSINESS);
	sem_init(&sem_turista, 0, MAX_TURISTA);
	sem_init(&sem_lleno, 0, 0);
	
	// Creacion de hilos
	for(int i=0; i < CANT_PASAJEROS; i++)
		pthread_create(&hilos_pasajeros[i], NULL, pasajero, NULL);		
	
	pthread_create(&hilo_vaciar, NULL, barco_navega, NULL);
	
	// Join de hilos
	for(int i=0; i< CANT_PASAJEROS; i++)
		pthread_join(hilos_pasajeros[i], NULL);	
	
	pthread_join(hilo_vaciar, NULL);
			
	return 0;
}
