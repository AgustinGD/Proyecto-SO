
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define OUT 101

void controlador();
void sincronizador();
void generador1();
void generador2();
void escritor1();
void escritor2();

int pipeControladorSincronizador[2];
int pipeSincronizadorGenerador1[2];
int pipeSincronizadorGenerador2[2];
int pipeSincronizadorEscritor1[2];
int pipeSincronizadorEscritor2[2];
int pipeGenerador1Sincronizador[2];
int pipeGenerador2Sincronizador[2];

int main(int argc, char **argv)
{
	int pid;
	
	if(pipe(pipeControladorSincronizador)<0) exit(1);
	if(pipe(pipeSincronizadorGenerador1)<0) exit(1);
	if(pipe(pipeSincronizadorGenerador2)<0) exit(1);
	if(pipe(pipeGenerador1Sincronizador)<0) exit(1);
	if(pipe(pipeGenerador2Sincronizador)<0) exit(1);
	if(pipe(pipeSincronizadorEscritor1)<0) exit(1);
	if(pipe(pipeSincronizadorEscritor2)<0) exit(1);
	
	
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
	int rand,status,i,iteraciones;
	int out=OUT;
	close(pipeControladorSincronizador[0]);
	printf("Bienvenido! Cuantos numeros desea generar?\n");
	scanf("%d",&iteraciones);
	
	for(i=0;i<iteraciones;i++){
		rand=random()%2 + 1;
		write(pipeControladorSincronizador[1],&rand,sizeof(int));
	}
	sleep(5);
	write(pipeControladorSincronizador[1],&out,sizeof(int));

	close(pipeControladorSincronizador[1]);
	printf("CONTROLADOR: Adios\n");
	
	wait(&status);
}

void sincronizador(){
	int control,rand;
	close(pipeControladorSincronizador[1]);
	close(pipeGenerador1Sincronizador[1]);
	close(pipeGenerador2Sincronizador[1]);
	close(pipeSincronizadorGenerador1[0]);
	close(pipeSincronizadorGenerador2[0]);
	close(pipeSincronizadorEscritor1[0]);
	close(pipeSincronizadorEscritor2[0]);
	
		
	while(1){
		read(pipeControladorSincronizador[0],&control,sizeof(int));
		if(control==1){
			write(pipeSincronizadorGenerador1[1],&control,sizeof(int));
			read(pipeGenerador1Sincronizador[0],&rand,sizeof(int));
			write(pipeSincronizadorEscritor1[1],&rand,sizeof(int));
			
		}
		if (control==2){
			write(pipeSincronizadorGenerador2[1],&control,sizeof(int));
			read(pipeGenerador2Sincronizador[0],&rand,sizeof(int));	
			write(pipeSincronizadorEscritor2[1],&rand,sizeof(int));
		}	
		
		if (control==OUT){
			write(pipeSincronizadorGenerador1[1],&control,sizeof(int));
			write(pipeSincronizadorGenerador2[1],&control,sizeof(int));
			write(pipeSincronizadorEscritor1[1],&control,sizeof(int));
			write(pipeSincronizadorEscritor2[1],&control,sizeof(int));
			break;
		}
	}
		
	close(pipeSincronizadorEscritor1[1]);
	close(pipeSincronizadorEscritor2[1]);
	close(pipeControladorSincronizador[0]);
	close(pipeGenerador1Sincronizador[0]);
	close(pipeGenerador2Sincronizador[0]);
	close(pipeSincronizadorGenerador1[1]);
	close(pipeSincronizadorGenerador2[1]);
	
}

void generador1(){
	int rand,control;
	close(pipeGenerador1Sincronizador[0]);
	close(pipeSincronizadorGenerador1[1]);
	while(1){
		read(pipeSincronizadorGenerador1[0],&control,sizeof(int));
		if (control==OUT)
			break;
		else{
		rand=random()%100;
		write (pipeGenerador1Sincronizador[1],&rand,sizeof(int));
		}
		
	}
	close(pipeGenerador1Sincronizador[1]);
	close(pipeSincronizadorGenerador1[0]);

}


void generador2(){
	int rand,control;
	close(pipeGenerador2Sincronizador[0]);
	close(pipeSincronizadorGenerador2[1]);
	while(1){
		read(pipeSincronizadorGenerador2[0],&control,sizeof(int));
		if (control==OUT)
			break;
		else{
		rand=random()%100;
		write (pipeGenerador2Sincronizador[1],&rand,sizeof(int));
		}
	}
	close(pipeGenerador2Sincronizador[1]);
	close(pipeSincronizadorGenerador2[0]);
}

void escritor1(){
	int rand;
	close(pipeSincronizadorEscritor1[1]);
	sleep(1);
	FILE * salida1 = fopen("Salida1.txt.","w");
	while(1){
		read(pipeSincronizadorEscritor1[0],&rand,sizeof(int));
		if (rand==OUT)
			break;
		else{
		fprintf(salida1,"%d",rand);
		fprintf(salida1,"%c",',');
		}
	}
	fclose(salida1);
	close(pipeSincronizadorEscritor1[0]);
}


void escritor2(){
	int rand;
	close(pipeSincronizadorEscritor2[1]);
	sleep(1);
	FILE * salida2 = fopen("Salida2.txt.","w");
	while(1){
		read(pipeSincronizadorEscritor2[0],&rand,sizeof(int));
		if (rand==OUT)
			break;
		else{
		fprintf(salida2,"%d",rand);
		fprintf(salida2,"%c",',');
		}
	}
	fclose(salida2);
	close(pipeSincronizadorEscritor2[0]);
}

