/*********************************************************************\
 *  Name: Tommy Franczak
 *  Assignment 4
 *  Date: October 12th, 2018
 *
 *  File: Process.h
 *  Purpose: Header file for the Process class
 *  Methods:
 *  - addBurst: Use for constructing the process; adds a I/O/CPU burst
 *  - mountProcess: Effectively begins the next burst
 *  - timeStep: Advances the Process by 1 unit of time
 *  - demountProcess: Ends current burst by incrementing history element
 *
\*********************************************************************/

#ifndef PROCESS
#define PROCESS

#include <string>
#include <utility>

#define ARRAY_SIZE 10   // Size of process history array

class Process {
    private:
        std::string ProcessName;
        int Priority;
        int ProcessID;
        int ArrivalTime;
        std::pair<char, int> History[ARRAY_SIZE];
        int Sub = 0;                                // Points to first unused elt in history
        int Current = 0;                            // Points to current burst in History
        int CPUTimer = 0;
        int ITimer = 0;
        int OTimer = 0;
        int CPUTotal = 0;
        int ITotal = 0;
        int OTotal = 0;
        int CPUCount = 0;
        int ICount = 0;
        int OCount = 0;

    public:
        Process();
        Process(std::string, int, int, int);

        void addBurst(char, int);
        void mountProcess();
        void timeStep();
        void demountProcess();

        // Get functions : Simply return the value of the corresponding variable
        int getArrivalTime() const {return ArrivalTime;}
        std::string getProcessName() const {return ProcessName;}
        int getProcessID() const {return ProcessID;}
        int getPriority() const {return Priority;}
        int getBurstTimer() const {return std::max(CPUTimer, std::max(ITimer, OTimer));}
        char getBurstType() const {return History[Current].first;}
        int getBurstsLeft() const {return Sub-Current;}
        int getCPUTotal() const {return CPUTotal;}
        int getITotal() const {return ITotal;}
        int getOTotal() const {return OTotal;}
        int getCPUCount() const {return CPUCount;}
        int getICount() const {return ICount;}
        int getOCount() const {return OCount;}
};











#endif
