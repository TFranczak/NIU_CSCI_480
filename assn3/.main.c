/*  Name: Tommy Franczak
 *  Class: 480-0001
 *  Title: Assignment 2 main.c
 *
 *  Functionality: This program will experiment with forks and pipes.
 *                 - Will increment a variable M between different the
 *                   parent, child, and grandchild process
 */


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define CMD_BFR 256  // Input Buffer size
#define CMD_BFS "256"
#define CMD_MAX 16   // Max commands supported
#define CMD_SIZ 32   // Max length of commands
#define ARG_MAX 16   // Each command may have up to 16 args

int parseCmds(char*, char[][ARG_MAX][CMD_SIZ]);
void handleCmds(int, char[][ARG_MAX][CMD_SIZ]);

int fd[2];

int main() {
    char commandLine[CMD_BFR];
    char *cmdv[CMD_MAX][ARG_MAX][CMD_SIZ] = {{}};
    int cmdc;

    if (pipe(fd) == -1) {
        exit(-5);
    }

    // Main loop
    while (1) {
        printf("480shell> ");

        // Grabs user input
        scanf("%" CMD_BFS "[^\n]", commandLine);
        while(getchar() != '\n');

        // Parse the command off the line
        cmdc = parseCmds(commandLine, cmdv);

        for (int i = 0; i < cmdc; i++){
            printf("cmd %d: %s %s\n", i, cmdv[i][0], cmdv[i][1]);
        }

        handleCmds(cmdc, cmdv);


        // Reset for next input
        cmdc = 0;
        commandLine[0] = '\0';
    }

    return 0;
}


/*
 *  parseCmds():
 *      Use: Grabs space delimited commands off of the command line
 *
 *      Arguments: command line string, place to put the commands
 *
 *      returns: Int representing total commands on command line
 */

int parseCmds(char* commandLine, char cmdv[][ARG_MAX][CMD_SIZ]) {
    int cmdc = 0;
    int word = 0;

    char temp[ARG_MAX][CMD_SIZ];

    // Split pipe delimited args
    char *p = strtok(commandLine, "||");
    while (p != NULL) {
        strcpy(temp[cmdc++], p);
        p = strtok(NULL, "||");
    }

    // Split commands and their args
    for (int i = 0; i < cmdc; i++) {
        p = strtok(temp[i], " ");
        for (int j = 0; p != NULL; j++) {
            strcpy(cmdv[i][j], p);
            p = strtok(NULL, " ");
        }
    }
    return cmdc;
}


/*
 *  handleCmds():
 *      Use: Does the expected behavior using the commands
 *
 *      Arguments: command count and command array
 *
 *      returns:
 */

void handleCmds(int cmdc, char cmdv[][ARG_MAX][CMD_SIZ]) {
    int N;

    if (cmdc <= 1) {
        N = fork();
        switch (N) {
            // Fork failed
            case(-1): break;
            // Child process
            case(0):
                execvp(cmdv[0][0], p);
                break;
            // Parent process
            default:
                wait(0);
                break;
        }
        printf("hi\n");
    }


    for (int i = 0; i < cmdc; i++) {
        if (strcmp(cmdv[i][0],"q") == 0 || strcmp(cmdv[i][0],"quit") == 0) {
            printf("\n");
            exit(1);
        }
    }

}




