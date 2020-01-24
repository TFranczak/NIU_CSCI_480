/*********************************************************************\
 *  Name: Tommy Franczak
 *  Assignment 4
 *  Date: October 12th, 2018
 *
 *  File: Assign4.cc
 *  Purpose: Main driver of assignment 4.
 *
\*********************************************************************/

#include <Assign4.h>

int main (int argc, char *argv[]) {

    // Open the input file
    FILE *input = fopen("inputfile.txt", "r");
    if (!input)
        exit(-5);

    // Parse input file
    char pname[256];
    int priority, arrivalTime, burstLength;
    char burstType;
    int processID = 101;
    char* buf = new char[100];
    while (1) {
        // Read first line
        fscanf(input, "%s %d %d\n", pname, &priority, &arrivalTime);
        if (!strcmp(pname, "STOPHERE")) break;

        // Create the process
        Process *ProcPtr = new Process(pname, arrivalTime, priority, processID++);

        // Read second line arg by arg
        for (int i = 0; i < MAX_ARGS; i++) {
            fscanf(input, "%c %d ", &burstType, &burstLength);

            // End of burst list
            if (burstType == 'N' || burstLength == 0) {
                continue;
            }
            // Add burst to the process
            else {
                 ProcPtr->addBurst(burstType, burstLength);
            }
        } fscanf(input," \n");
        //fgets(buf, 100, input);

        // Add the process to queue
        Entry.push(ProcPtr);
    }
    fclose(input);
    delete [] buf;


    // Initialize Process variables to NULL
    Active = 0;
    IActive = 0;
    OActive = 0;

    // MAIN PROCESSING LOOP
    int time = 0;
    for (time = 0; time < MAX_TIME; time++) {
        // Done if all queues are empty (no processes left)
        int totalProcs = Entry.size() + Ready.size() + Input.size() + Output.size();
        if (Active != NULL) totalProcs++;
        if (IActive != NULL) totalProcs++;
        if (OActive != NULL) totalProcs++;
        if (totalProcs == 0) break;

        // Ensure the active queues have the max procs allowed
        int activeProcs = totalProcs - Entry.size();
        // Add on all processes that have arrived
        while (activeProcs < AT_ONCE && !Entry.empty() && Entry.front()->getArrivalTime() <= time) {
            int ProcessID = Entry.front()->getProcessID();
            printf("Process %d moved from the Entry queue to the Ready queue at time %d.\n\n", ProcessID, time);
            Ready.push(Entry.front());
            Entry.pop();
            activeProcs++;
        }


        // Print diagnostics every HOW_OFTEN time steps
        if (time % HOW_OFTEN == 0) {
            printDiagnostic(time);
        }

        // Idle time until a proc enters
        if (activeProcs == 0) continue;



        // -------------------------------- Handle the 3 Devices -----------------------------------
        // No Active Process : get one
        if (Active == NULL && !Ready.empty()) {
            Active = Ready.top();
            Ready.pop();

            Active->mountProcess();
        }
        // Active Process: iterate through a time step
        else if (Active != NULL) {
            TimeStep(Active, time);
        }

        // No Active Process : get one
        if (IActive == NULL && !Input.empty()) {
            IActive = Input.top();
            Input.pop();

            IActive->mountProcess();
        }
        // IActive Process: iterate through a time step
        else if (IActive != NULL) {
            TimeStep(IActive, time);
        }

        // No IActive Process : get one
        if (OActive == NULL && !Output.empty()) {
            OActive = Output.top();
            Output.pop();

            OActive->mountProcess();
        }
        // IActive Process: iterate through a time step
        else if (OActive != NULL) {
            TimeStep(OActive, time);
        }
    }

    // End of run : Print diagnostic
    printf("The run has ended.\n");
    printf("Cumulative process idle time: %d\n", TimeWaited);
    printf("Number of terminated proceses = %d\n", ProcsTerminated);
    printDiagnostic(time);
}

