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
#include <ctype.h>
#include <stdlib.h>

#define CMD_BFR 256  // Input Buffer size
#define CMD_BFS "256"
#define CMD_MAX 16   // Max commands supported
#define CMD_SIZ 32   // Max length of commands
#define ARG_MAX 16   // Each command may have up to 16 args

#define READ_END 0  // Macros to make I/O redirection more verbose
#define WRITE_END 1

int parseCmds(char*, char*[][ARG_MAX]);
void handleCmds(int, char*[][ARG_MAX]);

int fd[2];

int main() {
    char commandLine[CMD_BFR];
    char *cmdv[CMD_MAX][ARG_MAX] = {{}};
    int cmdc;


    // Main loop
    while (1) {
        if (pipe(fd) == -1) {
            exit(-5);
        }

        printf("480shell> ");

        // Grabs user input
        scanf("%" CMD_BFS "[^\n]", commandLine);
        while(getchar() != '\n');

        // Parse the command off the line
        cmdc = parseCmds(commandLine, cmdv);

        // error parsing
        if (cmdc == -1) {
            fprintf(stderr, "Command not valid.\n");
            continue;
        }

        handleCmds(cmdc, cmdv);


        // Reset for next input
        cmdc = 0;
        commandLine[0] = '\0';
//        dup2(fd[0], 0);
//        dup2(fd[1], 1);
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

int parseCmds(char* commandLine, char* cmdv[][ARG_MAX]) {
    if (strcmp(commandLine, "") == 0) return 0;

    int cmdc = 0;
    int word = 0;


    char* temp[ARG_MAX];
    // Split pipe delimited args
    char *p = strtok(commandLine, "||");
    while (p != NULL) {
        temp[cmdc++] = p;
        p = strtok(NULL, "||");
    }

    int emptyString = 1;
    // Make sure each command is not just whitespace
    for (int i = 0; i < cmdc; i++) {
        int j = 0;
        while (temp[i][j] != 0) {
            if (!isspace(temp[i][j])) {
                emptyString = 0;
                break;
            }
            j++;
        }
        if (emptyString) return -1;
    }

    int argc = 0;
    // Split commands and their args
    for (int i = 0; i < cmdc; i++) {
        p = strtok(temp[i], " ");
        for (argc = 0; p != NULL; argc++) {
            cmdv[i][argc] = p;
            p = strtok(NULL, " ");
        }
        // Signal end of arg list
        cmdv[i][argc] = NULL;
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

void handleCmds(int cmdc, char* cmdv[][ARG_MAX]) {
    if (cmdc == 0) return;

    // user quit
    if (strcmp(cmdv[0][0], "q") == 0 || strcmp(cmdv[0][0], "quit") == 0) {\
        close(fd[READ_END]);
        close(fd[WRITE_END]);
        exit(1);
    }

    int N, M;
    // Handle non-piped command
    if (cmdc <= 1) {
        N = fork();
        switch (N) {
            // Fork failed
            case(-1): break;
            // Child process
            case(0):
                execvp(cmdv[0][0], cmdv[0]);
                fprintf(stderr, "Command 1 invalid.\n");
                exit(-5);
                break;
            // Parent process
            default:
                close(fd[READ_END]);
                close(fd[WRITE_END]);
                wait(0);
                break;
        }
    }

    else {
        N = fork();

        switch(N) {
            // Fork failed
            case(-1): break;
            // Child process
            case(0):
                // Write to stdout
                dup2(fd[WRITE_END], STDOUT_FILENO);
                close(fd[READ_END]);
                close(fd[WRITE_END]);

                // Run command
                execvp(cmdv[0][0], cmdv[0]);
                fprintf(stderr, "Command 1 invalid.\n");
                exit(-5);
                break;
            // Parent process
            default:
                M = fork();

                switch(M) {
                    // Fork failed
                    case(-1):
                        exit(-5);
                        break;
                    // Second child
                    case(0):
                        // Read from stdin
                        dup2(fd[READ_END], STDIN_FILENO);
                        close(fd[WRITE_END]);
                        close(fd[READ_END]);

                        // Run command
                        execvp(cmdv[1][0], cmdv[1]);
                        fprintf(stderr, "Command 2 invalid.\n");
                        exit(-5);
                        break;
                    // Parent process
                    default:
                        close(fd[WRITE_END]);
                        close(fd[READ_END]);

                        wait(0);
                        break;
                }
                wait(0);
                break;
        }
    }
}




