#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>   //para usar booleanos
#include <semaphore.h> //para usar semaforos
#include <unistd.h>     // para hacer sleep

//usar semaforos no la bariable bool

//aca declaro la candidad de veces que se van a ejecutar los procesos
#define PROCESOS 5

//esta variable controla el numero de ronda
int procesoActual = 1;

//se inicia el mutex 
pthread_mutex_t mutex;

//se inician semaforos para coordinar los procesos.
sem_t esc;
sem_t lee;

bool puedeLeer = false;
bool puedeEscribir = true;


void *escribir(){
        while(procesoActual<PROCESOS){

                sem_wait(&esc);
                pthread_mutex_lock(&mutex);
                printf("nuermo ronda:%i\n", procesoActual);
                printf("Escribiendo datos");
                printf("\n");
                sleep(2);//duerme el proceso 2 segundos simpulando la escritura
                sem_post(&lee);
                pthread_mutex_unlock(&mutex);
        }
        
        
}
void *leer(){
        while(procesoActual<=PROCESOS){

                sem_wait(&lee);
                pthread_mutex_lock(&mutex);

                printf("Leyendo datos");
                printf("\n");
                sleep(1);//duerme el proceso 1 segundos para simular la lectura 
                procesoActual += 1;
                sem_post(&esc);
                pthread_mutex_unlock(&mutex);
        }
        
}

int main(void) 
{               
                //se declaran los hilos
                pthread_t hiloLeer;
                pthread_t hiloEscribir;
                //se crean los hilos y se les asigna la funcion a ejecutar

                //se inician los semaforos y el de escribir comienza en uno para ser el primero en ejecutarce 
                sem_init(&esc,0,1);
                sem_init(&lee,0,0);

                pthread_create(&hiloLeer, NULL, &leer, NULL);
                pthread_create(&hiloEscribir, NULL, &escribir, NULL);
                
                //se espera a que los hilos terminen su ejecucion y se muestran 
                pthread_join(hiloLeer, NULL);
                pthread_join(hiloEscribir, NULL);

                sem_destroy(&esc);
                sem_destroy(&lee);
        
        return 0;
}

//Para compilar:   gcc ejercicioMutex.c -o ejMutex -lpthread
//Para ejecutar:   ./ejMutex