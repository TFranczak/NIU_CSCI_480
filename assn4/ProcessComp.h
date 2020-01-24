/*********************************************************************\
 *  Name: Tommy Franczak
 *  Assignment 4
 *  Date: October 12th, 2018
 *
 *  File: ProcessComp.h
 *  Purpose: Contains the struct definiton for ProcessComp
 *  Methods:
 *  - operator(): Overloaded so that a Process can be used in STL priority_queue
 *
\*********************************************************************/

#ifndef PROCESSCOMP
#define PROCESSCOMP

#include <Process.h>

struct ProcessComp {
    bool operator()(const Process* lhs, const Process* rhs) const {
        // Priority Tiebreaker : Lower arrival time wins
        if (lhs->getPriority() == rhs->getPriority()) {
            return (lhs->getArrivalTime() > rhs->getArrivalTime());
        }
        // Sorts by higher priority
        else {
            return lhs->getPriority() < rhs->getPriority();
        }
    }
};

#endif
