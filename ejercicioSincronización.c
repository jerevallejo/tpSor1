#include <stdio.h>
#include <stdlib.h>         // para usar exit y funciones de la libreria standard
#include <pthread.h>
#include <semaphore.h> //para usar semaforos
#include <unistd.h>     // para hacer sleep

//se inicia el mutex. 
pthread_mutex_t mutex;

//aca asignamos la cantidad de rodas que se van a jugar.
#define CANTIDAD_RONDAS 40

//se inician los contadores.
int rondaActual = 1;
int salioGanar = 0;
int salioPerder = 0;

//estas variables enteras eran utilizadas para ver los valores de los semaforos 
//int valueGan;
//int valuePer; 

//inicializan los semaforos.
sem_t jug;
sem_t gan;
sem_t per;
sem_t des;
sem_t aux;


void* jugar()
{
	while(rondaActual<CANTIDAD_RONDAS)
	{	
     	sem_wait(&jug);
        pthread_mutex_lock(&mutex);
	    printf("---> jugar \n");

	    sem_post(&per);
	    sem_post(&gan);
	    sem_post(&aux);

	    pthread_mutex_unlock(&mutex);
	} 
	pthread_exit(NULL);
}

void* perder()
{

	while(rondaActual<CANTIDAD_RONDAS)
	{
	    sem_wait(&per);
	   	sem_wait(&aux);
	   	 
	    if (rondaActual>=(CANTIDAD_RONDAS +1))
	    {
				pthread_exit(NULL);
	    }else{
	    		salioPerder +=1;
	    		printf("---> perder \n");
	    }
	    sem_post(&des);
	}
	pthread_exit(NULL);

}

void* ganar()
{
	while(rondaActual<CANTIDAD_RONDAS)
	{
	    sem_wait(&gan);  
	    sem_wait(&aux);

	    if (rondaActual>=(CANTIDAD_RONDAS +1))
	    {
				pthread_exit(NULL);
	    }else{
		    	salioGanar +=1;
			    printf("---> ganar \n");
		}
	    sem_post(&des);
	}
	pthread_exit(NULL);
}

void* descansar()
{
	while(rondaActual <= CANTIDAD_RONDAS)
	{
	    sem_wait(&des);

	    pthread_mutex_lock(&mutex);

	    printf("---> descansar  \n"); 
	    rondaActual += 1;
	    if (rondaActual == (CANTIDAD_RONDAS+1))
	    {
	    	//cuando pasa la ultima ronda se habilitan todos los semaforos 
	    	//para que se finalicen todos los hilos. 
	    	//Despues de eso se imprime el mensaje de fin de jeugo y se muestran los resultados.
	    	sem_post(&per);
	    	sem_post(&gan);
	    	sem_post(&aux);
	    	sem_post(&jug);
	    	printf("----EL JUEGO TERMINO----\nResultados: \n");
	    	printf("	Ganar:%i \n",salioGanar );
	    	printf("	Perder: %i \n",salioPerder );

	    	pthread_exit(NULL);
	    }else{		
	    	//como no es la ultima ronda se muestra el numero de ronda y se vuelve a jugar.
	    	printf("Ronda de juego numero: %i \n" , rondaActual);
	    	sem_post(&jug);
	    }
	    pthread_mutex_unlock(&mutex);
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

	//se destruyen los hilos
    sem_destroy(&jug);
	sem_destroy(&per);
	sem_destroy(&gan);
	sem_destroy(&des);
	sem_destroy(&aux);

	pthread_exit(NULL);

return 0;

}

//Para compilar:   gcc ejercicioSincronizacion.c -o ejSincronizacion -lpthread
//Para ejecutar:   ./ejSincronizacion
