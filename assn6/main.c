/*  Name: Tommy Franczak
 *  Class: 480-0001
 *  Title: Assignment 6 main.c
 *
 *  Functionality: Simulates managing memory blocks
 */

#include <iostream>
#include <block.h>
#include <list>
#include <stdio.h>

using namespace std;

#define KB 1024         // Size of KB
#define MB 1024 * 1024  // Size of MB
#define HOWOFTEN 5
#define SYSSIZE 16 * MB // Size of system
#define BA 3 * MB       // Base address after operating system

std::list<block> InUse, Avail;

void Load(int, string, string);
void Allocate(int, string, string);
void Deallocate(string, string);
void Terminate(string);
void Print();
void Condense();

char FIT;

int main(int argc, char *argv[]) {
    // Parse command line args
    if (argc < 2) {
        cerr << "Missing command line argument 'B' or 'F'.\n";
        exit (-1);
    }
    else if ((argv[1][0] != 'B') && (argv[1][0] != 'F')) {
        cerr << "Unrecognized command line argument.\n";
        exit (-1);
    }
    else {
        FIT = argv[1][0];
        cout << FIT;
    }

    // Initialize the list values
    Avail.push_back(block(1 * MB, BA));
    Avail.push_back(block(2 * MB, BA + MB));
    Avail.push_back(block(2 * MB, BA + 3 * MB));
    Avail.push_back(block(4 * MB, BA + 5 * MB));
    Avail.push_back(block(4 * MB, BA + 9 * MB));

    FILE *inputFile;
    inputFile = fopen("assn6in.txt", "r");

    int quit = 0;
    int Transaction = 0;
    // Main loop. Quits when it encounters an unknown transaction type (? in this case)
    while (!quit) {
        if (Transaction % HOWOFTEN == 0) Print();


        char type;
        int size;
        char bid[5], pid[5];  // Block ID and Process ID

        // Read first line
        fscanf(inputFile, "%c \n", &type);

        switch(type) {
            // Load
            case ('L'):
                fscanf(inputFile, "%s %d %s \n", pid, &size, bid);
                Load(size, pid, bid);
                break;
            // Allocate
            case ('A'):
                fscanf(inputFile, "%s %d %s \n", pid, &size, bid);
                Allocate(size, pid, bid);
                break;
            // Deallocate
            case ('D'):
                fscanf(inputFile, "%s %s \n", pid, bid);
                Deallocate(pid, bid);
                break;
            // Terminate
            case ('T'):
                fscanf(inputFile, "%s \n", pid);
                Terminate(pid);
                break;
            // Unrecognized command detected
            default:
                quit = 1;
                break;
        }
        cout << "\n";
        Transaction++;
    }
    fclose(inputFile);

    cout << "\nFinal report: \n";
    Print();

    return 0;
}

/****************************************************************************\
 *      Load: Loads a process into memory by allocating a block from Avail
 *
 *      Arguments:
 *          size:   The size of the block to allocate
 *          pid:    The process ID of the process were loading
 *          bid:    The block ID of the process to allocate
\****************************************************************************/

void Load(int size, string pid, string bid) {
    cout << "Transaction: request to load process " << pid << ", block ID " << bid << " using " << size << " bytes\n";

    // First fit
    if (FIT == 'F') {
        for (auto b = Avail.begin(); b != Avail.end(); b++) {
            // Size left in block
            if ((*b).getSize() >= size) {
                cout << "Found a block of size " << (*b).getSize() << endl;

                // Remove space from Avail block and allocate new InUse block
                int start = (*b).getStart();
                (*b).allocate(size);
                InUse.push_front(block(size, start, pid, bid));

                cout << "Success in allocating a block\n";

                // No space left in block
                if ((*b).getSize() == 0) {
                    Avail.erase(b);
                }
                return;
            }
        }
    }
    // Best fit
    else if (FIT == 'B') {
        int found = 0;
        auto smallest = Avail.begin();
        for (auto b = Avail.begin(); b != Avail.end(); b++) {
            // Found big enough block
            if ((*b).getSize() >= size) {
                // First large enough block
                if (found == 0) {
                    smallest = b;
                    found++;
                }
                // If smallest remainder
                if ((*b).getSize() < (*smallest).getSize()) {
                    smallest = b;
                }
            }
        }
        // Found a big enough block
        if (found != 0) {
            cout << "Found a block of size " << (*smallest).getSize() << endl;

            // Remove space from Avail block and allocate new InUse block
            int start = (*smallest).getStart();
            (*smallest).allocate(size);
            InUse.push_front(block(size, start, pid, bid));

            cout << "Success in allocating a block\n";

            return;
        }
    }

    cerr << "No space large enough for process " << pid << ", block " << bid << endl;
}

/****************************************************************************\
 *      Allocate: Allocating a block from Avail to InUse
 *
 *      Arguments:
 *          size:   The size of the block to allocate
 *          pid:    The process ID of the process were loading
 *          bid:    The block ID of the process to allocate
\****************************************************************************/

void Allocate(int size, string pid, string bid) {
    cout << "Transaction: request to allocate " << size << " bytes for process " << pid << ", block ID: " << bid << endl;

    // First fit
    if (FIT == 'F') {
        for (auto b = Avail.begin(); b != Avail.end(); b++) {
            // Size left in block
            if ((*b).getSize() >= size) {
                cout << "Found a block of size " << (*b).getSize() << endl;

                // Remove space from Avail block and allocate new InUse block
                int start = (*b).getStart();
                (*b).allocate(size);
                InUse.push_front(block(size, start, pid, bid));

                cout << "Success in allocating a block\n";

                // No space left in block
                if ((*b).getSize() == 0) {
                    Avail.erase(b);
                }
                return;
            }
        }
    }
    // Best fit
    else if (FIT == 'B') {
        int found = 0;
        auto smallest = Avail.begin();
        for (auto b = Avail.begin(); b != Avail.end(); b++) {
            // Found big enough block
            if ((*b).getSize() >= size) {
                // First large enough block
                if (found == 0) {
                    smallest = b;
                    found++;
                }
                // If smallest remainder
                if ((*b).getSize() < (*smallest).getSize()) {
                    smallest = b;
                }
            }
        }
        // Found a big enough block
        if (found != 0) {
            cout << "Found a block of size " << (*smallest).getSize() << endl;

            // Remove space from Avail block and allocate new InUse block
            int start = (*smallest).getStart();
            (*smallest).allocate(size);
            InUse.push_front(block(size, start, pid, bid));

            cout << "Success in allocating a block\n";

            return;
        }
    }

    cerr << "No space large enough for process " << pid << ", block " << bid << endl;
}


/****************************************************************************\
 *      Deallocate: Deallocates a specific block from InUse and places it back in Avail
 *
 *      Arguments:
 *          pid:    The process ID of the block were deallocating
 *          bid:    The block ID of the block to deallocate
\****************************************************************************/

void Deallocate(string pid, string bid) {
    cout << "Transaction: request to deallocate block ID " << bid << " for process " << pid << endl;

    for (auto b = InUse.begin(); b != InUse.end(); b++) {
        // block found
        if ((*b).getPid() == pid && (*b).getBid() == bid) {

            // Find insert location
            auto Ab = Avail.begin();
            for (; Ab != Avail.end(); Ab++) {
                if ((*b).getStart() < (*Ab).getStart()) {
                    // Insert block into available
                    Avail.insert(Ab, block((*b).getSize(), (*b).getStart()));
                    break;
                }
            }
            // Handles edge case
            if (Ab == Avail.end()) Avail.push_back(block((*b).getSize(), (*b).getStart()));

            Condense();

            // Delete the block from InUse
            InUse.erase(b);
            cout << "Success in deallocating a block.\n";

            return;
        }
    }

    cerr << "PID and BID combo not found for PID: " << pid << ", BID: " << bid << endl << endl;
}

/****************************************************************************\
 *      Terminate: Terminates all blocks belonging to a certain PID
 *
 *      Arguments:
 *          pid:    The process ID of the process to terminate
\****************************************************************************/

void Terminate(string pid) {
    cout << "Transaction: request to terminate process " << pid << endl;

    int found = 0;
    for (auto b = InUse.begin(); b != InUse.end();) {
        // Erase iterator with matching Pid
        // Reinsert block into Avail
        if ((*b).getPid() == pid) {

            // Find insert location
            auto Ab = Avail.begin();
            for (; Ab != Avail.end(); Ab++) {
                if ((*b).getStart() < (*Ab).getStart()) {
                    // Insert block into available
                    Avail.insert(Ab, block((*b).getSize(), (*b).getStart()));
                    break;
                }
            }
            // Handles edge case
            if (Ab == Avail.end()) Avail.push_back(block((*b).getSize(), (*b).getStart()));

            b = InUse.erase(b);
            found++;
        }
        else {
            b++;
        }
    }
    if (found == 0) cerr << "No process with that ID is currently running.\n";
    else {
        Condense();
        cout << "Success in terminating the process\n";
    }
}


/****************************************************************************\
 *
 *      Print: Prints diagnostic information about both lists, Avail and InUse
 *
\****************************************************************************/

void Print() {
    // Print available blocks and tally size
    int size = 0;
    cout << "Available blocks: \n";
    if (Avail.empty()) cout << "(none)\n";
    else {
        for (auto b : Avail) {
            cout << b;
            size += b.getSize();
        }
        cout << "Total size of the list = " << size << endl;
    }
    cout << "\n";

    // Print InUse blocks and tally the size
    size = 0;
    cout << "Blocks in use: \n";
    if (InUse.empty()) cout << "(none)\n";
    else {
        for (auto b : InUse) {
            cout << b;
            size += b.getSize();
        }
        cout << "Total size of the list = " << size << endl;
    }
    cout << "\n";

}


/****************************************************************************\
 *
 *      Condense: Condenses continguous available blocks into one.
 *
\****************************************************************************/

void Condense() {
    // Search for consecutive blocks to combine
    // Create two list iterators to do comparisons
    auto first = Avail.begin();
    auto second = Avail.begin();
    second++;
    while (second != Avail.end()) {
        int endAddress = (*first).getStart() + (*first).getSize();
        // Found blocks to append
        if (endAddress == (*second).getStart()) {
            // Makes sure no blocks over 4 MB are created
            if ((*first).getSize() + (*second).getSize() > 4 * MB) {
                first++;
                second++;
                continue;
            }

            cout << "Merging two blocks at " << (*first).getStart() << " and " << (*second).getStart() << endl;
            // Do the append
            (*first).append((*second).getSize());
            second = Avail.erase(second); // This increments second as well
        }
        // Do not append the blocks together
        else {
            first++;
            second++;
        }
    }
}
