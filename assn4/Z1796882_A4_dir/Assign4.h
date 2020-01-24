/*********************************************************************\
 *  Name: Tommy Franczak
 *  Assignment 4
 *  Date: October 12th, 2018
 *
 *  File: Assign4.h
 *  Purpose: Header file for assignment 4.
 *  Functions:
 *  - printDiagnostic: Print information about the active processes and queues
 *  - Terminate: Terminate a process that has no more I/O/CPU bursts
 *  - TimeStep: Advances the process by 1 ms
 *
\*********************************************************************/

#ifndef ASSIGN4
#define ASSIGN4

#include <Process.h>
#include <ProcessComp.h>
#include <queue>
#include <string>
#include <cstring>

using namespace std;

#define MAX_TIME 500    // Lenght of run
#define AT_ONCE 5       // Max processes concurrently
#define QUEUE_SIZE 20   // Size of queue
#define ARRAY_SIZE 10   // Size of history array
#define HOW_OFTEN 20    // How often to reprint system state
#define MAX_ARGS 10     // Max args in input file

// Typedef so that I don't have to type this out every time
typedef priority_queue<Process*, vector<Process*>, ProcessComp> ProcessPQ;

ProcessPQ Ready, Input, Output;
queue<Process*> Entry;
Process *Active, *IActive, *OActive;

int TimeWaited = 0; // Counts total time waited amongst all process
int ProcsTerminated = 0; // Counts total processes killed


// printDiagnostic: Print information about the actives procecss and queues
// Arguments:
//  - time: Current time unit
void printDiagnostic(int time) {
    // Print the current state of each device
    // Prints 0 if the device is not occupied
    printf("Status at time %d\n", time);
    if (Active != NULL) printf ("Active is %d\n", Active->getProcessID());
    else printf("Active is 0\n");
    if (IActive != NULL) printf ("IActive is %d\n", IActive->getProcessID());
    else printf("IActive is 0\n");
    if (OActive != NULL) printf ("OActive is %d\n", OActive->getProcessID());
    else printf("OActive is 0\n");

    // Print entry queue
    // Duplicates queue and prints each entry by deleting the top of the temp queue
    queue<Process*>* tempEntry = new queue<Process*>(Entry);
    printf("Contents of the Entry queue:\n");
    if (tempEntry->empty()) {
        printf("(Empty)\n");
    }
    else {
        while (!tempEntry->empty()) {
            Process* Proc = tempEntry->front();
            printf("%d Priority = %d arriving at time = %d\n", Proc->getProcessID(), Proc->getPriority(), Proc->getArrivalTime());
            tempEntry->pop();
        }
    }
    delete tempEntry;

    // Print ready queue
    // Duplicates queue and prints each entry by deleting the top of the temp queue
    ProcessPQ* tempPriority = new ProcessPQ(Ready);
    printf("Contents of the Ready queue:\n");
    if (tempPriority->empty()) printf("(Empty)\n");
    else {
        while (!tempPriority->empty()) {
            Process* Proc = tempPriority->top();
            printf("%d(%d)     ", Proc->getProcessID(), Proc->getPriority());
            tempPriority->pop();
        }
        printf("\n");
    }
    delete tempPriority;

    // Print input queue
    // Duplicates queue and prints each entry by deleting the top of the temp queue
    tempPriority = new ProcessPQ(Input);
    printf("Contents of the Input queue:\n");
    if (tempPriority->empty()) printf("(Empty)\n");
    else {
        while (!tempPriority->empty()) {
            Process* Proc = tempPriority->top();
            printf("%d(%d)     ", Proc->getProcessID(), Proc->getPriority());
            tempPriority->pop();
        }
        printf("\n");
    }
    delete tempPriority;

    // Print output queue
    // Duplicates queue and prints each entry by deleting the top of the temp queue
    tempPriority = new ProcessPQ(Output);
    printf("Contents of the Output queue:\n");
    if (tempPriority->empty()) printf("(Empty)\n");
    else {
        while (!tempPriority->empty()) {
            Process* Proc = tempPriority->top();
            printf("%d(%d)     ", Proc->getProcessID(), Proc->getPriority());
            tempPriority->pop();
        }
        printf("\n");
    }
    printf("\n\n");
    delete tempPriority;
}


// Terminate: Prints final information about the process. Deletes the process in memory
// Arguments:
//  - Proc: The process to be terminated
//  - time: Current time unit
void Terminate(Process* Proc, int time) {
    int TimeSpentWaiting = time - Proc->getArrivalTime() - Proc->getCPUTotal() - Proc->getITotal() - Proc->getOTotal();

    printf("A process has terminated.\n");
    printf("%-20s %d\n", "Process ID:", Proc->getProcessID());
    printf("%-20s %s\n", "Name:", Proc->getProcessName().c_str());
    printf("%-20s %d\n", "Priority:", Proc->getPriority());
    printf("%-20s %d and ended at time %d\n", "Started at time:", Proc->getArrivalTime(), time);
    printf("%-20s %d in %d CPU bursts\n", "Total CPU time =", Proc->getCPUTotal(), Proc->getCPUCount());
    printf("%-20s %d in %d Input bursts\n", "Total Input time =", Proc->getITotal(), Proc->getICount());
    printf("%-20s %d in %d Output bursts\n", "Total Output time =", Proc->getOTotal(), Proc->getOCount());
    printf("%-20s %d\n", "Time spent waiting:", TimeSpentWaiting);
    printf("\n\n");

    TimeWaited += TimeSpentWaiting;
    ProcsTerminated++;

    delete Proc;
}


// TimeStep
// Arguments:
//  - Proc: The current process to advance in time
//  - time: The current time unity
void TimeStep(Process* Proc, int time) {
    Proc->timeStep();

    // Done with current burst.
    if (Proc->getBurstTimer() == 0) {
        char BurstType = Proc->getBurstType();

        Proc->demountProcess();

        // More bursts left
        if (Proc->getBurstsLeft() > 0) {
            if (Proc->getBurstType() == 'C') Ready.push(Proc);
            if (Proc->getBurstType() == 'O') Output.push(Proc);
            if (Proc->getBurstType() == 'I') Input.push(Proc);
        }
        // No bursts left
        else {
            //Terminate process
            Terminate(Proc, time);
        }

        // Remove process from device
        if (BurstType == 'C') Active = NULL;
        if (BurstType == 'I') IActive = NULL;
        if (BurstType == 'O') OActive = NULL;
    }
}









#endif
