/*  Name: Tommy Franczak
 *  Class: 480-0001
 *  Title: Assignment 1 main.c
 *
 *  Functionality: This program will experiment with forks and PIDs
 *
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


int main() {
    int mainpid, parentpid;

    mainpid = getpid();
    parentpid = getppid();

    fprintf(stderr, "I am the original process. My PID is %d and my parent's PID is %d.\n", mainpid, parentpid);

    // First fork variables
    int N, childpid, parentpid2;

    // Second fork variables
    int N2, grandpid, childpid2;

    fprintf(stderr, "Now we have the first fork.\n");

    // First fork
    N = fork();
    switch (N) {
        // Err
        case -1:
            fprintf(stderr, "The first fork failed.\n");
            exit(-1);
            break;
        // Child
        case 0:
            childpid = getpid();
            parentpid2 = getppid();

            fprintf(stderr, "I am the child. My PID is %d and my parent's PID is %d.\n", childpid, parentpid2);
            fprintf(stderr, "Now we have the second fork.\n");

            // Second fork
            N2 = fork();
            switch(N2) {
                // Err
                case -1:
                    fprintf(stderr, "The second fork failed.\n");
                    exit(-1);
                    break;
                // Grand child
                case 0:
                    grandpid = getpid();
                    childpid2 = getppid();

                    fprintf(stderr, "I am the grandchild. My PID is %d and my parent's PID is %d.\n", grandpid, childpid2);
                    fprintf(stderr, "I am the grandchild, about to exit\n");

                    exit(0);
                    break;
                // Child
                default:
                    fprintf(stderr, "I am the still the child. My PID is %d and my parent's PID is %d.\n", childpid, mainpid);
                    wait(0);

                    fprintf(stderr, "I am the child, about to exit.\n");
                    exit(0);
                    break;
            }
            break;
        // Parent
        default:
            fprintf(stderr, "I am the parent. My PID %d and my parent's PID is %d.\n", mainpid, parentpid);
            sleep(2);

            fprintf(stderr, "I am the parent, about to call ps.\n");
            system("ps");
            wait(0);

            fprintf(stderr, "I am the parent, about to exit.\n");
            exit(0);
            break;
    }

}

// Hopefully this program is self documenting for the most part :D
