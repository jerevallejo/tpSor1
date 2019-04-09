#include <stdio.h>
#include <stdlib.h>         // para usar exit y funciones de la libreria standard
#include <pthread.h>
#include <semaphore.h> //para usar semaforos
#include <unistd.h>     // para hacer sleep

pthread_mutex_t mutex;
#define CANTIDAD_RONDAS 3000
int rondaActual = 1;
int salioGanar = 0;
int salioPerder = 0;
//inicializo los semaforos
sem_t jug;
sem_t gan;
sem_t per;
sem_t des;
sem_t aux;


void* jugar()
{
	while(rondaActual<CANTIDAD_RONDAS)
	{
		//printf("dentro del while jugar\n");
     	sem_wait(&jug);
        pthread_mutex_lock(&mutex);

		printf("dentro del mutex jugar\n");
        //sem_wait(&jug);

	    printf("---> jugar \n");

	    sem_post(&per);
	    sem_post(&gan);
	    sem_post(&aux);
	    printf("esta antesde salir del mutex\n");

	    pthread_mutex_unlock(&mutex);
	} 
	pthread_exit(NULL);
}

void* perder()
{

	while(rondaActual<CANTIDAD_RONDAS)
	{
		//printf("dentro del while perder\n");
		
//pthread_mutex_lock(&mutex);
	    sem_wait(&per);

//printf("antes del mutex perder\n");
	    pthread_mutex_lock(&mutex);
	    printf("dentro del mutex perder\n");

	   	sem_wait(&aux);
	   	 
	   // printf("dentro del mutex perder\n");

//	   	sem_wait(&gan);

	    if (rondaActual>=(CANTIDAD_RONDAS +1))
	    {
				pthread_exit(NULL);
	    }else{
	    		salioPerder +=1;
	    		printf("---> perder \n");
	    		
	   	sem_wait(&gan);
	    sem_post(&des);
		}

//	   	sem_wait(&gan);
//	    sem_post(&des);

				
	   printf("antes de salir del mutex perder\n");
	    pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);

}

void* ganar()
{
	while(rondaActual<CANTIDAD_RONDAS)
	{

	    	  //  pthread_mutex_lock(&mutex);
		//printf("dntreo del while ganar\n");
	    sem_wait(&gan);  

	       // sem_wait(&aux);
////printf("antes del mutex ganar\n");
	    pthread_mutex_lock(&mutex);
	    //printf("dentro del mutex ganar\n");

	    sem_wait(&aux);

	//sem_wait(&per); 
	    if (rondaActual>=(CANTIDAD_RONDAS +1))
	    {
				pthread_exit(NULL);
	    }else{
	    	salioGanar +=1;
		    printf("---> ganar \n");
		}
		sem_wait(&per);
	    sem_post(&des);

	    pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
 
}

void* descansar()
{
	while(rondaActual <= CANTIDAD_RONDAS)
	{
	    sem_wait(&des);

	    //pthread_mutex_lock(&mutex);

	    printf("---> descansar  \n"); 
	    rondaActual += 1;
	    if (rondaActual == (CANTIDAD_RONDAS +1))
	    {
	    	sem_post(&per);
	    	sem_post(&gan);
	    	sem_post(&aux);
	    	sem_post(&jug);
	    	printf("----EL JUEGO TERMINO.----\nResultados: \n");
	    	printf("Ganar:%i \n",salioGanar );
	    	printf("Perder: %i \n",salioPerder );
	    	pthread_exit(NULL);
	    }else{		
	    	printf("Ronda de juego numero: %i \n" , rondaActual);
	    	
	    }
	    sem_post(&jug);
	   // pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);

}

int main() 
{               

    //se declaran los hilos
    pthread_t hiloJugar;
    pthread_t hiloGanar;
    pthread_t hiloPerder;
    pthread_t hiloDescansar;

	//se inician los hilos
    sem_init(&jug,0,1);
	sem_init(&gan,0,0);
	sem_init(&per,0,0);
	sem_init(&des,0,0);
	sem_init(&aux,0,0);


    //se crean los hilos y se les asigna la funcion a ejecutar
    pthread_create(&hiloJugar, NULL, &jugar, NULL);
    pthread_create(&hiloGanar, NULL, &ganar, NULL);
    pthread_create(&hiloPerder, NULL, &perder, NULL);
    pthread_create(&hiloDescansar, NULL, &descansar, NULL);
                
    //se espera a que los hilos terminen su ejecucion y se muestran 
    pthread_join(hiloJugar, NULL);
    pthread_join(hiloGanar, NULL);
    pthread_join(hiloPerder, NULL);
    pthread_join(hiloDescansar, NULL);


    sem_destroy(&jug);
	sem_destroy(&per);
	sem_destroy(&gan);
	sem_destroy(&des);
	sem_destroy(&aux);

	pthread_exit(NULL);
return 0;

}
