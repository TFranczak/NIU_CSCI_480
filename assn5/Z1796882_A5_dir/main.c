/*  Name: Tommy Franczak
 *  Class: 480-0001
 *  Title: Assignment 5 main.c
 *
 *  Functionality: This program will experiment with pthreads, mutexs, and semaphores.
 *                 - Will use different threads to increase and decrease a counter
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

#define P_NUMBER 6 // Number of producers
#define C_NUMBER 4 // Number of consumers
#define BUFFER_SIZE 12 // Maximum size of the buffer
#define N_P_STEPS 4 // Number of iterations for each producer thread
#define N_C_STEPS 6 // Number of iterations for each consumer thread

pthread_mutex_t bufferMutex;
sem_t notFull, notEmpty;
int counter = 0;

void * Insert(void *);
void * Remove(void *);
void * Produce(void *);
void * Consume(void *);

int main() {
    pthread_t producers[P_NUMBER];  // Producer Threads
    pthread_t consumers[C_NUMBER];  // Consumer Threads

    // Creates semaphores.
    // notFull:  For producer. If not zero, insert widget.
    // notEmpty: For consumer. If not zero, delete widget.
    sem_init(&notFull , 0, BUFFER_SIZE);
    sem_init(&notEmpty, 0, 0);

    int rc = 0;

    // Create each pthread and assign appropriate process
    long t = 0;
    for(t=0; t < P_NUMBER; t++) {
        rc |= pthread_create(&producers[t], NULL, Produce, (void *) t);
        if (rc) {
            fprintf(stderr, "Error creating producer.\n");
            exit(-1);
        }
    }
    for(; t < C_NUMBER+P_NUMBER; t++) {
        rc |= pthread_create(&consumers[t - P_NUMBER], NULL, Consume, (void *) t);
        if (rc) {
            fprintf(stderr, "Error creating producer.\n");
            exit(-1);
        }
    }

    // Wait for all threads to finish before terminating main
    for (int i = 0; i < P_NUMBER; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < C_NUMBER; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Clean up semaphores and mutex
    pthread_mutex_destroy(&bufferMutex);
    sem_destroy(&notFull);
    sem_destroy(&notEmpty);
    pthread_exit(NULL);
    return 0;
}

/****************************************************************************\
 *      Produce: Allows a thread to insert N_P_STEPS of widgets to the counter
 *
 *      Arguments: ID of calling thread
 *
 *      returns: 0 for success
\****************************************************************************/

void * Produce(void * ID) {
    for (int i = 0; i < N_P_STEPS; i++) {
        sem_wait(&notFull);
        Insert(ID);
        sem_post(&notEmpty);
    }

    pthread_exit(NULL);
    return 0;
}


/****************************************************************************\
 *      Consume: Allows a thread to remove N_C_STEPS of widgets from the counter.
 *
 *      Arguments: ID of calling thread
 *
 *      returns: 0 for success
\****************************************************************************/

void * Consume(void * ID) {
    for (int i = 0; i < N_C_STEPS; i++) {
        sem_wait(&notEmpty);
        Remove(ID);
        sem_post(&notFull);
    }

    pthread_exit(NULL);
    return 0;
}


/****************************************************************************\
 *      Insert: Locks the counter to securely increment it
 *
 *      Arguments: ID of calling thread
 *
 *      returns: 0 for success
\****************************************************************************/

void * Insert(void * ID) {
    // Securely increment the counter.
    pthread_mutex_lock(&bufferMutex);
    counter++;
    sleep(1);
    printf("Thread %d inserted a widget. Counter is now %d.\n", ID, counter);
    pthread_mutex_unlock(&bufferMutex);

    return 0;
}


/****************************************************************************\
 *      Remove: Locks the counter to securely decrement it
 *
 *      Arguments: ID of calling thread
 *
 *      returns: 0 for success
\****************************************************************************/

void * Remove(void * ID) {
    // Securely decrement the counter.
    pthread_mutex_lock(&bufferMutex);
    counter--;
    sleep(1);
    printf("Thread %d deleted a widget. Counter is now %d.\n", ID, counter);
    pthread_mutex_unlock(&bufferMutex);

    return 0;
}
