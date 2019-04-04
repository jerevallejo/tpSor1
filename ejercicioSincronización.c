#include <stdio.h>
#include <stdlib.h>         // para usar exit y funciones de la libreria standard
#include <pthread.h>
#include <semaphore.h> //para usar semaforos
#include <unistd.h>     // para hacer sleep

pthread_mutex_t mutex;
#define CANTIDAD_RONDAS 5
int rondaActual = 0;
//inicializo los semaforos
sem_t jug;
sem_t gan;
sem_t per;
sem_t des;
sem_t jugando;
sem_t ter;

//me falta temrinar el juego, no puedo imprimir el terminar 

void* jugar()
{

	while(rondaActual<CANTIDAD_RONDAS)
	{
     	sem_wait(&jug);
     	//resta una jugada 
     	sem_wait(&jugando);
	    printf(" jugar ");
	    printf("%i \n" , rondaActual);
	    sem_post(&per);
	    sem_post(&gan);
	    // sem_post(&ter);
	} 
	pthread_exit(NULL);

}
void* perder()
{

	while(rondaActual<CANTIDAD_RONDAS)
	{
	    sem_wait(&per);
	    sem_wait(&gan);
	    printf(" perder ");
	    	    printf("%i \n" , rondaActual);
	    // printf("\n");
	    sem_post(&des);		
	}
	pthread_exit(NULL);

}

void* ganar()
{
	while(rondaActual<CANTIDAD_RONDAS)
	{
	    sem_wait(&gan);
	    sem_wait(&per);
	    printf(" ganar ");
	    	    printf("%i \n" , rondaActual);
	    // printf("\n");
	    // printf("%i \n" ,i);
	    sem_post(&des);
	}
	pthread_exit(NULL);
 
}

void* descansar()
{
	while(rondaActual <= CANTIDAD_RONDAS)
	{
	    sem_wait(&des);
	    printf(" descansar ");
	    printf("%i \n" , rondaActual);
	    // printf("\n"); 
	    rondaActual += 1;
	printf("%i \n" , rondaActual);
	printf("%i \n" , CANTIDAD_RONDAS);
	    sem_post(&jug);
	    sem_post(&ter);
	}
	// printf("JUEGO TERMINADO");
	pthread_exit(NULL);

}

void* terminar()
{
	while(rondaActual >= CANTIDAD_RONDAS )
	{
	    sem_wait(&ter);
	    printf(" JUEGO TERMINADO ");
	    // printf("%i \n" ,i);
	    //timpo de espera 
	    pthread_exit(NULL);
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
    pthread_t hiloTerminar;
	//
    sem_init(&jug,0,1);
	sem_init(&gan,0,0);
	sem_init(&per,0,0);
	sem_init(&des,0,0);
	//inicio este semaforo en -20 que son la cantidad de rondas que se va a ejecutar
	 sem_init(&ter,0,0);
	 sem_init(&jugando,0,16);

    //se crean los hilos y se les asigna la funcion a ejecutar
    pthread_create(&hiloJugar, NULL, &jugar, NULL);
    pthread_create(&hiloGanar, NULL, &ganar, NULL);
    pthread_create(&hiloPerder, NULL, &perder, NULL);
    pthread_create(&hiloDescansar, NULL, &descansar, NULL);
     pthread_create(&hiloTerminar, NULL, &terminar, NULL);
                
    //se espera a que los hilos terminen su ejecucion y se muestran 
    pthread_join(hiloJugar, NULL);
    pthread_join(hiloGanar, NULL);
    pthread_join(hiloPerder, NULL);
    pthread_join(hiloDescansar, NULL);
     pthread_join(hiloTerminar, NULL);

    sem_destroy(&jug);
	sem_destroy(&per);
	sem_destroy(&gan);
	sem_destroy(&des);
	sem_destroy(&jugando);
	sem_destroy(&ter);

return 0;

}
