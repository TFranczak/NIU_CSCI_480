/*********************************************************************\
 *  Name: Tommy Franczak
 *  Assignment 4
 *  Date: October 12th, 2018
 *
 *  File: Process.cc
 *  Purpose:  Contains declarations for the methods of Process
 *  Methods:
 *  - Process: Constructor
 *  - addBurst: Use for constructing the process; adds a I/O/CPU burst
 *  - mountProcess: Effectively begins the next burst
 *  - timeStep: Advances the Process by 1 unit of time
 *  - demountProcess: Ends current burst by incrementing history element
 *
\*********************************************************************/

#include <Process.h>

// Default constructor - not used
Process::Process(){}


// Constructor: Necessary arguments for a process
// Arguments:
//      ProcessName: Name of the process
//      ArrivalTime: Time of the processes arrival
//      Priority: The priority of the process
//      ProcessID: The autoassigned processID
Process::Process(std::string ProcessName, int ArrivalTime, int Priority, int ProcessID) {
    this->ProcessName = ProcessName;
    this->ArrivalTime = ArrivalTime;
    this->Priority = Priority;
    this->ProcessID = ProcessID;
}


// addBurst: Used for adding CPU/I/O Bursts. Stores them in the history table
// Arguments:
//      BurstType: C/I/O representing which type of brust
//      BurstLength: The actual length of the burst
void Process::addBurst(char BurstType, int BurstLength) {
    History[Sub].first  = BurstType;
    History[Sub].second = BurstLength;
    Sub++;
}


// timeStep: Advances the process by 1 unit of time
// No args/return
void Process::timeStep() {
    // Indicate which type of burst
    switch (History[Current].first) {
        // Increment burst type total time
        // Decrement time left in burst
        case ('C'):
            CPUTimer--;
            CPUTotal++;
            break;
        case ('I'):
            ITimer--;
            ITotal++;
            break;
        case ('O'):
            OTimer--;
            OTotal++;
            break;

        default:
            fprintf(stderr, "Unrecognized CPU Burst from timestep: %c; of process: %d. Current: %d\n", History[Current].first, ProcessID, Current);
    }
    return;
}

// mountProcess: Mounts the process to a device by checking the next burst in history
//      Mounting factors in the unit of time used to call it
// No args/return
void Process::mountProcess() {
    int BurstLength = History[Current].second - 1;

    // Check burst type
    switch (History[Current].first) {
        // Increment total burst counter for the relevant type
        // Increment time spent
        case ('C'):
            CPUTotal++;
            CPUCount++;
            CPUTimer = BurstLength;
            break;
        case ('I'):
            ITotal++;
            ICount++;
            ITimer = BurstLength;
            break;
        case ('O'):
            OTotal++;
            OCount++;
            OTimer = BurstLength;
            break;

        default:
            fprintf(stderr, "Unrecognized CPU Burst from mount.\n");
    }
}

// demountProcess: Simply demount the process by incrementing the History table element
//      Up to operating system to control the devices handling
// No args/return
void Process::demountProcess() {
    Current++;
}
