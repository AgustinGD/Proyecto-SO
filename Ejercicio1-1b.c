#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <errno.h>

#define KEY 1234
#define FIN 100
#define MSG_GEN1SINC 101
#define MSG_GEN2SINC 102
#define MSG_ESC1SINC 103
#define MSG_ESC2SINC 104
#define MSG_CONTSINC 105
#define MSG_SINCGEN1 106
#define MSG_SINCGEN2 107
#define MSG_SINCESC1 108
#define MSG_SINCESC2 109
#define MSG_SINCCONT 110



struct msgInfo{
	long tipo;
	int valor;
};

int msgInfoSize = sizeof(struct msgInfo) - sizeof(long);
int pid,status;

void controlador();
void sincronizador();
void generador1();
void generador2();
void escritor1();
void escritor2();

int main(int argc, char **argv)
{
	srand(time(NULL));
	pid=fork();
	if(pid!=0){
		pid=fork();
		if(pid!=0){
			pid=fork();
			if(pid!=0){
				pid=fork();
				if(pid!=0){
					pid=fork();
					if(pid!=0){
						controlador();
					}
					else{
						sincronizador();
					}
				}
				else{
					escritor1();
				}
			}
			else{escritor2();
			}
		}
		else{
			srand(time(0)+getpid());
			generador1();
		}
	}
	else{
		srand(time(0)+getpid());
		generador2();
	}
	
	
	return 0;
}


void controlador(){
	int i,iteraciones,msgQueue,rand;
	struct msgInfo mensaje;
	msgQueue = msgget(KEY,IPC_CREAT|0666);
	
	iteraciones=0;
	while(iteraciones < 1){
		printf("**************************************\n");
		printf("Bienvenido! Cuantos numeros desea generar?\n");
		scanf("%d",&iteraciones);
		if(iteraciones<1) 
			printf("Error, ingrese un valor entero mayor a 0\n");
	}
	printf("\n\n**************************************\n");
	printf("Comenzando ejecucion...\n");
	printf("**************************************\n\n\n");
	
	for(i=0;i<iteraciones;i++){
		rand=random()%2+1;
		mensaje.valor=rand;
		mensaje.tipo = MSG_CONTSINC;
		msgsnd(msgQueue,&mensaje,msgInfoSize,0);
	}
		mensaje.valor=FIN;
		mensaje.tipo = MSG_CONTSINC;
		msgsnd(msgQueue,&mensaje,msgInfoSize,0);

	for(i=0;i<5;i++)
		wait(&status);
		

}

void sincronizador(){
	int msgQueue;
	struct msgInfo mensaje;
	msgQueue = msgget(KEY,0666);
	while(mensaje.valor!=FIN){
		msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_CONTSINC,0);
		switch(mensaje.valor){
			case FIN://Se finalizan los ciclos de los procesos para que terminen.
				mensaje.tipo=MSG_SINCGEN1;
				msgsnd(msgQueue,&mensaje,msgInfoSize,0);
				msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_GEN1SINC,0);
				mensaje.tipo=MSG_SINCESC1;
				msgsnd(msgQueue,&mensaje,msgInfoSize,0);
				msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_ESC1SINC,0);
				mensaje.tipo=MSG_SINCGEN2;
				msgsnd(msgQueue,&mensaje,msgInfoSize,0);
				msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_GEN2SINC,0);
				mensaje.tipo=MSG_SINCESC2;
				msgsnd(msgQueue,&mensaje,msgInfoSize,0);
				msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_ESC2SINC,0);
				
				break;
			
			case 1: 
				mensaje.tipo=MSG_SINCGEN1;
				msgsnd(msgQueue,&mensaje,msgInfoSize,0);
				msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_GEN1SINC,0);
				mensaje.tipo=MSG_SINCESC1;
				msgsnd(msgQueue,&mensaje,msgInfoSize,0);
				msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_ESC1SINC,0);
					break;
			case 2:
				mensaje.tipo=MSG_SINCGEN2;
				msgsnd(msgQueue,&mensaje,msgInfoSize,0);
				msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_GEN2SINC,0);
				mensaje.tipo=MSG_SINCESC2;
				msgsnd(msgQueue,&mensaje,msgInfoSize,0);
				msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_ESC2SINC,0);
					break;
		};
		
		
	}

}

void generador1(){
	struct msgInfo mensaje;
	int rand,msgQueue;
	msgQueue = msgget(KEY,0666);

	while(1){
		msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_SINCGEN1,0);
		if(mensaje.valor==FIN){ 
			mensaje.tipo=MSG_GEN1SINC;
			msgsnd(msgQueue,&mensaje,msgInfoSize,0);
			break;
		}
		else{
			rand=random()%100;
			printf("Generador 1 genera: %d\n",rand);
			mensaje.valor=rand;
			mensaje.tipo=MSG_GEN1SINC;
			msgsnd(msgQueue,&mensaje,msgInfoSize,0);
		}
	}
	
}

void generador2(){
	struct msgInfo mensaje;
	int rand,msgQueue;
	msgQueue = msgget(KEY,0666);

	while(1){
		msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_SINCGEN2,0);
		if(mensaje.valor==FIN){ 
			mensaje.tipo=MSG_GEN2SINC;
			msgsnd(msgQueue,&mensaje,msgInfoSize,0);

			break;
		}
		else{
			rand=random()%100;
			printf("Generador 2 genera: %d\n",rand);			
			mensaje.valor=rand;
			mensaje.tipo=MSG_GEN2SINC;
			msgsnd(msgQueue,&mensaje,msgInfoSize,0);
		}
	}
	
}

void escritor1(){
	struct msgInfo mensaje;
	int msgQueue,generado;
	msgQueue = msgget(KEY,0666);
	FILE * salida1 = fopen("Salida1.txt.","w");
	while(1){
 		msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_SINCESC1,0);
		if(mensaje.valor==FIN){
			mensaje.tipo=MSG_ESC1SINC;
			msgsnd(msgQueue,&mensaje,msgInfoSize,0);
			break;
		}
		else{
			generado = mensaje.valor;
			printf("Escritor 1 escribe %d en el archivo...\n",generado);					
			fprintf(salida1,"%d",generado);
			fprintf(salida1,"%s",",");
			mensaje.tipo=MSG_ESC1SINC;
			msgsnd(msgQueue,&mensaje,msgInfoSize,0);
			sleep(0.2);
		}
	}
	
	fclose(salida1);
	}
void escritor2(){
	struct msgInfo mensaje;
	int msgQueue,generado;
	msgQueue = msgget(KEY,0666);

	FILE * salida2 = fopen("Salida2.txt.","w");
	while(1){
 		msgrcv(msgQueue,&mensaje,msgInfoSize,MSG_SINCESC2,0);
		if(mensaje.valor==FIN){
			mensaje.tipo=MSG_ESC2SINC;
			msgsnd(msgQueue,&mensaje,msgInfoSize,0);
			break;
		}
		else{
			generado = mensaje.valor;
			printf("Escritor 2 escribe %d en el archivo...\n",generado);			
			fprintf(salida2,"%d",generado);
			fprintf(salida2,"%s",",");
			mensaje.tipo=MSG_ESC2SINC;
			msgsnd(msgQueue,&mensaje,msgInfoSize,0);
			sleep(0.2);
		}
	}	
	fclose(salida2);	
}

