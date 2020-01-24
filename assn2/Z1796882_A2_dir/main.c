/*  Name: Tommy Franczak
 *  Class: 480-0001
 *  Title: Assignment 2 main.c
 *
 *  Functionality: This program will experiment with forks and pipes.
 *                 - Will increment a variable M between different the
 *                   parent, child, and grandchild process
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SZ 10

int PWork(int[], int[], int[]);
int CWork(int[], int[], int[]);
int GWork(int[], int[], int[]);
int Pow(int, int);


int main() {
    int err[3];

    int fdA[2]; // For Pipe A: Parent -> Child
    int fdB[2]; //          B: Child -> Grandchile
    int fdC[2]; //          C: Grandchild -> Parent

    // Create the pipes
    // If failed: Return -5
    if (pipe(fdA) == -1) {
        fprintf(stderr, "Pipe A failed.\n");
        exit(-5);
    }
    if (pipe(fdB) == -1) {
        fprintf(stderr, "Pipe B failed.\n");
        exit(-5);
    }
    if (pipe(fdC) == -1) {
        fprintf(stderr, "Pipe C failed.\n");
        exit(-5);
    }

    // First fork
    int N1, N2;
    N1 = fork();
    switch(N1){
        // Err on fork 1
        case(-1):
            fprintf(stderr, "Fork 1 failed.\n");
            exit(-5);
            break;

        // Child Process
        case(0):
            // Second fork
            N2 = fork();

            switch(N2){
                // Err on fork 2
                case(-1):
                    fprintf(stderr, "Fork 2 failed.\n");
                    exit(-5);
                    break;

                // Grandchild process
                case(0):
                    GWork(fdA, fdB, fdC);
                    break;

                // Child Process
                default:
                    CWork(fdA, fdB, fdC);
                    break;
            }


            break;

        // Parent Process
        default:
            PWork(fdA, fdB, fdC);
            break;
    }

    return 0;
}


/*
 *  PWork:
 *      Use: Parent processes work. Does the first write.
 *           Increments M every time it comes around
 *           Stops when M > 99999999
 *
 *      Arguments: 3 communication pipes
 *
 *      Returns: 0 for success
 */

int PWork(int fdA[2], int fdB[2], int fdC[2]) {
    // Close read end of Pipe A
    close(fdA[0]);

    // Close write end of Pipe C
    close(fdC[1]);

    // Initialize buffer and value and fill them with "1"
    char Buffer[BUF_SZ], Value[BUF_SZ];
    Value[1] = '\0';
    Value[0] = '1';
    int M = 1;

    // Write the entire Value string to Child
    write(fdA[1], Value, BUF_SZ);

    fprintf(stderr, "Parent, Value: %s\n", Value);


    while (M <= 99999999) {
        // Does the read
        read(fdC[0], Value, BUF_SZ);

        // Converts string to int
        M = atoi(Value);
        // Does incrementing
        M = 4 * M + 3;

        // Converts int to string and puts it in buffer for child
        sprintf(Buffer, "%d", M);
        fprintf(stderr, "Parent, Value: %s\n", Buffer);
        write(fdA[1], Buffer, BUF_SZ);
    }

    // Close rest of pipes
    close(fdA[1]);
    close(fdB[0]);
    close(fdB[1]);
    close(fdC[0]);

    wait(0);
    exit(0);
    return 0;
}



/*
 *  CWork:
 *      Use: Child processes work.
 *           Increments M every time it comes around
 *           Stops when M > 99999999
 *
 *      Arguments: 3 communication pipes
 *
 *      Returns: 0 for success
 */

int CWork(int fdA[2], int fdB[2], int fdC[2]) {
    // Close write end of Pipe A
    close(fdA[1]);

    // Close read end of Pipe B
    close(fdB[0]);

    // Initialize buffer and value and fill them with "1"
    char Buffer[BUF_SZ], Value[BUF_SZ];
    Value[1] = '\0';
    Value[0] = '1';
    int M = 1;

    while (M <= 99999999) {
        // Reads from the Buffer
        read(fdA[0], Value, BUF_SZ);

        // Converts string to int
        M = atoi(Value);
        // Increments M
        M = 4 * M + 3;

        // Converts int to string and sends it to grandchild
        sprintf(Buffer, "%d", M);
        fprintf(stderr, "Child, Value: %s\n", Buffer);
        write(fdB[1], Buffer, BUF_SZ);
    }

    // Close rest of pipes
    close(fdA[0]);
    close(fdB[1]);
    close(fdC[0]);
    close(fdC[1]);

    wait(0);
    exit(0);
    return 0;
}



/*
 *  GWork:
 *      Use: Grandchild processes work.
 *           Increments M every time it comes around
 *           Stops when M > 99999999
 *
 *      Arguments: 3 communication pipes
 *
 *      Returns: 0 for success
 */

int GWork(int fdA[2], int fdB[2], int fdC[2]) {
    // Close write end of Pipe B
    close(fdB[1]);

    // Close read end of Pipe C
    close(fdC[0]);

    // Initialize buffer and value and fill them with "1"
    char Buffer[BUF_SZ], Value[BUF_SZ];
    Value[1] = '\0';
    Value[0] = '1';
    int M = 1;

    while (M <= 99999999) {
        // Reads from buffer
        read(fdB[0], Value, BUF_SZ);

        // String to int
        M = atoi(Value);
        // Increments M
        M = 4 * M + 3;

        // Converts int to string and sends it to parent
        sprintf(Buffer, "%d", M);
        fprintf(stderr, "Grandchild, Value: %s\n", Buffer);
        write(fdC[1], Buffer, BUF_SZ);
    }


    // Close rest of pipes
    close(fdA[0]);
    close(fdA[1]);
    close(fdB[0]);
    close(fdC[1]);

    wait(0);
    exit(0);
    return 0;
}





