#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX_PATH 25

void dividirComando(char buffer[], char *parametros[] ,int *numParametros);
void dir_list(char dir[],char *parametros[], int *numParametros);
void dir_create(char *parametros[], int *numParametros);
void dir_delete(char dir[],char *parametros[], int *numParametros);
void file_create(char dir[],char *parametros[], int *numParametros);
void file_show(char *parametros[], int *numParametros);
void file_delet();
void perm_mod();
void show_help();

int main(int argc, char **argv)
{	
	char buffer[MAX_PATH],aux[MAX_PATH];
	char *params[MAX_PATH] ;
	char *command ;
	char directorioActual[MAX_PATH];
	int * numParametros;
	
	printf("\n\n****Bienvenido a la minishell de Gonzalez Diaz, Talmon**** \n\n");
	
	while(1){
		printf("Minishell:");
	
		if(getcwd(directorioActual,sizeof(directorioActual))!=0){
		printf(" %s >>",directorioActual);
		}
		else {
			perror("getcwd() error");
			return 1;
		}
		
		fgets(buffer,MAX_PATH,stdin);
		
		strtok(buffer,"\n");
		strcpy(aux,buffer);
		command = strtok(aux," ");
		dividirComando(buffer,params,numParametros);
		
			
			
		if(strcmp(command,"dir_list")==0) {*directorioActual=*strcat(directorioActual,"/dir_list");dir_list(directorioActual,params,numParametros);}
		else
			if(strcmp(command,"dir_create")==0) {*directorioActual=*strcat(directorioActual,"/dir_create");dir_create(params,numParametros);}
			else
				if(strcmp(command,"dir_delete")==0) {*directorioActual=*strcat(directorioActual,"/dir_delete");dir_delete(directorioActual,params,numParametros);}
				else
					if(strcmp(command,"file_create")==0) {*directorioActual=*strcat(directorioActual,"/file_create");file_create(directorioActual,params,numParametros);}
					else
						if(strcmp(command,"file_show")==0) {*directorioActual=*strcat(directorioActual,"/file_show");file_show(params,numParametros);}					
						else
							if(strcmp(command,"perm_mod")==0) {*directorioActual=*strcat(directorioActual,"/perm_mod");perm_mod(directorioActual,params,numParametros);}
							else
								if(strcmp(command,"show_help")==0) {show_help();}
								else 
									if(strcmp(command,"exit")==0) {break;}
									else printf("Comando incorrecto\n");
		printf("\n");	
	}
	
	printf("Hasta luego...");		
	
	
	return 0;
}

void dividirComando(char buffer[], char *parametros[], int *numParametros){
	char *palabra = strtok(buffer," ");
	int i = 0;
	while(palabra!=NULL){
		parametros[i]=palabra;
		i++;
		palabra=strtok(NULL," ");
	}
	parametros[i] = NULL;
	*numParametros = i-1;
	
}

void dir_list(char dir[],char *parametros[], int *numParametros){

    DIR *dp;
    struct dirent *entry;
    char *directorio;
    if(*numParametros==0){
        directorio=".";
    }else{
        directorio=parametros[1];
    }
    dp=opendir(directorio);
    if(dp>0){
        while((entry=readdir(dp))!=NULL)
			printf("%s\n",entry->d_name);
        closedir(dp);
    }else{
        printf("dir_list: : No existe el directorio seleccionado \n");
    }

}

void dir_create(char *parametros[], int *numParametros){
	
	if(*numParametros!=1){
		printf("dir_create Error, cantidad incompatible de argumentos\n");
	}
	else{
		int make;
		make=mkdir(parametros[1],0777);
		if(make!=0)
			printf("Error en la creacion del directorio");
	}	

}

void dir_delete(char dir[],char *parametros[], int *numParametros){
	
	if(*numParametros!=1){
		printf("dir_delete Error, cantidad incompatible de argumentos\n");
	}
	else{
		int rem;
		rem = rmdir(parametros[1]);
		if(rem ==-1) printf("No pudo eliminarse el directorio");
		else 
			if(rem == 0) printf("Directorio eliminado con exito!");
	}
}

void file_create(char dir[],char *parametros[], int *numParametros){
	if(*numParametros!=1){
		printf("file_create Error, cantidad incompatible de argumentos\n");
	}
	else{
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
		int fd;
		fd= creat(parametros[1],mode);
			printf("%d",fd);
		if(fd<0) printf("Error en creacion de archivo");
		
	}	
}


void file_show(char *parametros[], int *numParametros){
	if(*numParametros!=1){
		printf("file_show Error, cantidad incompatible de argumentos\n");
	}
	else{
		FILE * file = fopen(parametros[1],"r");
		if(file==NULL) printf("Error, no existe el archivo\n");
		else{
			char ch = fgetc(file);
			while(ch!=EOF){
				printf("%c",ch);
				ch = fgetc(file);
			}
			printf("\n");
			fclose(file);
		}
	}
}


void perm_mod(char dir[],char *parametros[], int *numParametros){

	if(*numParametros!=2){
		printf("perm_mod Error, cantidad incompatible de argumentos\n");
	}
	else{
		mode_t mode;
		if(strcmp(parametros[2],"r")==0)mode=S_IRUSR|S_IRGRP|S_IROTH;
        else if(strcmp(parametros[2],"w")==0)mode=S_IWUSR|S_IWGRP|S_IWOTH;
			else if(strcmp(parametros[2],"x")==0)mode=S_IXUSR|S_IXGRP|S_IXOTH;
				else if(strcmp(parametros[2],"rw")==0)mode=S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH;
					else if(strcmp(parametros[2],"rx")==0)mode=S_IRUSR|S_IRGRP|S_IROTH|S_IXUSR|S_IXGRP|S_IXOTH;
						else if(strcmp(parametros[2],"wx")==0)mode=S_IWUSR|S_IWGRP|S_IWOTH|S_IXUSR|S_IXGRP|S_IXOTH;
							else if(strcmp(parametros[2],"rwx")==0)mode=S_IRUSR|S_IRGRP|S_IROTH|S_IWUSR|S_IWGRP|S_IWOTH|S_IXUSR|S_IXGRP|S_IXOTH;
								 else printf("Error, no se han ingresado parametros validos. Intentelo de nuevo \n");
        if(chmod(parametros[1],mode)==-1)
            printf("perm_mod: No se pudieron modificar los permisos.");
 		
	}
}

void show_help(){
	
	printf("*************************************************************\n\n");
	printf("                    Menu de ayuda                    \n\n");
	printf("Comandos\n\n");
	printf("dir_create [parametro]: Crea un nuevo directorio con el nombre pasado por parametro en la carpeta en la que se esta actualmente\n\n");
	printf("dir_list [parametro]: lista el contenido de un directorio pasado por parametro\n");
	printf("En caso de no pasar ningun parametro se lista el contenido del directorio home/pi\n\n");
	printf("dir_delete [parametro]: Elimina el directorio pasado por parametro (Solo si esta vacio)\n\n");
	printf("file_create [parametro]: Crea un nuevo archivo en carpeta home/pi \n\n");
	printf("file_show [parametro]: Muestra el contenido del archivo pasado por parametro \n\n");
	printf("perm_mod [parametro][permiso]: Modifica los permisos del arhivo pasado por parametro\n");
	printf("Estos pueden ser: \n ");
	printf("r: solo lectura \n w: solo escritura \n x:solo ejecucion\n");
	printf(" rx: lectura y escritura \n rw: escritura y ejecucion \n wx:escritura y ejecucion\n");
	printf(" rxw: Lectura escritura y ejecucion \n\n");
	printf("*************************************************************\n");
}
