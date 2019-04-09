#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>   //para usar booleanos
#include <semaphore.h> //para usar semaforos
#include <unistd.h>     // para hacer sleep

//usar semaforos no la bariable bool
int proceso = 20;//aca declaro la candidad de veces que se van a ejecutar los procesos
pthread_mutex_t mutex;
bool puedeLeer = false;
bool puedeEscribir = true;


void *escribir(){
        while(puedeEscribir){
                pthread_mutex_lock(&mutex);
                puedeEscribir = false;
                printf("Escribiendo datos");
                printf("\n");
                sleep(2);//duerme el proceso 2 segundos simpulando la escritura
                puedeLeer = true;
                pthread_mutex_unlock(&mutex);
        }
        
        
}
void *leer(){
        while(puedeLeer){
                pthread_mutex_lock(&mutex);
                puedeLeer = false;
                printf("Leyendo datos");
                printf("\n");
                sleep(1);//duerme el proceso 1 segundos para simular la lectura 
                puedeEscribir = true;
                pthread_mutex_unlock(&mutex);
        }
        
}

int main(void) 
{               
                for (int i = 0; i < proceso; ++i)
                {
                        
            //    printf("i %i", i);
              //  printf("\n");
                //se declaran los hilos
                pthread_t hiloLeer;
                pthread_t hiloEscribir;
                //se crean los hilos y se les asigna la funcion a ejecutar
                pthread_create(&hiloLeer, NULL, &leer, NULL);
                pthread_create(&hiloEscribir, NULL, &escribir, NULL);
                
                //se espera a que los hilos terminen su ejecucion y se muestran 
                pthread_join(hiloLeer, NULL);
                pthread_join(hiloEscribir, NULL);
        }
        return 0;
}

//Para compilar:   gcc ejercicioMutex.c -o ejercicioMutex -lpthread
//Para ejecutar:   ./ejercicioMutex