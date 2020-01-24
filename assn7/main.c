/*  Name: Tommy Franczak
 *  Class: 480-0001
 *  Title: Assignment 7 main.c
 *
 *  Functionality: Simulates managing memory blocks
 */

#include <iostream>
#include <stdio.h>
#include <stdbool.h>
#include <list>
#include <Entry.h>
#include <iomanip>

using namespace std;

#define HOWOFTEN 5
#define MAXPRINT 240
#define MAXENTRY 12
#define FATSIZE 4096

list<Entry> directory;
short fat[FATSIZE]; // lol

void newFile(string filename1, int size, int verbosity);
void modifyFile(string filename1, int size);
void renameFile(string filename1, string filename2, int verbosity);
void deleteFile(string filename1, int verbosity);
void copyFile(string filename1, string filename2);
void print();

int main(int argc, char *argv[]) {
    for (int i = 0; i < 4096; i++ ) { fat[i] = 0; }

    FILE *inputFile;
    inputFile = fopen("inputfile.txt", "r");
    if (inputFile == NULL) {
        fprintf(stderr,"File open failed.\n");
        exit(-1);
    }

    // Add . directory
    fat[0] = -1;
    directory.push_front(Entry(0, 512, "."));

    // Add .. directory
    directory.push_back(Entry(0, 0, ".."));

    bool quit = false;
    int counter = 0;
    // Main loop
    while (!quit){
        if (counter % 6 == 0) {
            print();
        }

        int size;
        char type;
        char filename1[21], filename2[21];

        // Read first line
        fscanf(inputFile, "%c \n", &type);

        switch(type) {
            // New File Transaction
            case ('N'):
                fscanf(inputFile, "%s %d \n", filename1, &size);
                newFile(string(filename1), size, 1);
                break;
            // Modify File Transaction
            case ('M'):
                fscanf(inputFile, "%s %d \n", filename1, &size);
                modifyFile(string(filename1), size);
                break;
            // Copy File Transaction
            case ('C'):
                fscanf(inputFile, "%s %s \n", filename1, filename2);
                copyFile(string(filename1), filename2);
                break;
            // Rename File Transaction
            case ('R'):
                fscanf(inputFile, "%s %s \n", filename1, filename2);
                renameFile(string(filename1), filename2, 1);
                break;
            // Delete File Transaction
            case ('D'):
                fscanf(inputFile, "%s \n", filename1);
                deleteFile(string(filename1), 1);
                break;
            // Unrecognized command detected
            default:
                quit = true;
                break;
        }
        counter++;
    }
    print();


    fclose(inputFile);
    return 0;
}

/****************************************************************************\
 *      newFile: Creates a new Entry object and allocates space in the FAT
 *
 *      Arguments:
 *          filename1:  The name of the file to create
 *          size:       The size of the file to create
 *          verbosity:  1 for more verbose
\****************************************************************************/

void newFile(string filename1, int size, int verbosity) {
    if (verbosity) cout << "Transaction: Add a new file.\n";

    // Check if the file already exists in the directory
    for (auto itr = directory.begin(); itr != directory.end(); itr++) {
        if ((*itr).getName() == filename1) {
            cout << "Error adding " << filename1 << ", file already exists.\n";
            return;
        }
    }

    // Initialize varibles
    int initialSize = size;
    int prev = -1;
    int beginning;

    if (size == 0) {
        directory.push_back(Entry(-1, 0, filename1));
        if (verbosity) cout << "Successfully added a new file, " << filename1 << ", of size " << size << endl;
        return;
    }

    // Find open locations
    for (int i = 0, count = 0; i < FATSIZE; i++) {
        if (fat[i] == 0) {
            // Decrement the size left to allocate
            size -= 512;

            // Save first location
            if (count == 0) {
                beginning = i;
                count++;
            }

            // Save current ptr in previous allocation
            if (prev != -1) {
                fat[prev] = i;
                prev = -1;
            }

            // Need more iterations
            if (size > 0) {
                // Flag to show it will be reset
                fat[i] = -2;
                prev = i;
            }
            // Finished iterating
            else {
                fat[i] = -1;
                break;
            }
        }
    }
    // Add file to the directory
    directory.push_back(Entry(beginning, initialSize, filename1));
    if (verbosity) cout << "Successfully added a new file, " << filename1 << ", of size " << initialSize << endl;

}

/****************************************************************************\
 *      modifyFile: Edits an existing files size
 *
 *      Arguments:
 *          filename1:  The name of the file to modify
 *          size:       The size to modify the file to
\****************************************************************************/

void modifyFile(string filename1, int size) {
    cout << "Transaction: Modify a file size\n";

    // Search for the file to modify
    auto itr = directory.begin();
    for (; itr != directory.end(); itr++) {
        if ((*itr).getName() == filename1) {
            break;
        }
    }
    if (itr == directory.end()) {
        cout << "Error modifying " << filename1 << ", file not found\n";
        return;
    }

    newFile("TEMPORARYFILE", size, 0);
    deleteFile(filename1, 0);
    renameFile("TEMPORARYFILE", filename1, 0);

    cout << "Successfully modified file " << filename1 << endl;
}


/****************************************************************************\
 *      renameFile: Renames an existing file object
 *
 *      Arguments:
 *          filename1:  The name of the file to create
 *          size:       The size of the file to create
 *          verbosity:  1 for more verbose
\****************************************************************************/

void renameFile(string filename1, string filename2, int verbosity) {
    if (verbosity) cout << "Transaction: Rename a file\n";

    // Search for filename and rename it
    auto itr = directory.begin();
    for (; itr != directory.end(); itr++) {
        if ((*itr).getName() == filename1) {
            (*itr).setName(filename2);
            break;
        }
    }

    // File not found
    if (itr == directory.end()) {
        if (verbosity) cout << "Error renaming " << filename1 << ", file not found.\n";
        return;
    }

    // Move file to the back
    directory.splice(directory.end(), directory, itr);

    if (verbosity) cout << "Successfully changed the file name " << filename1 << " to " << filename2 << endl;
    return;
}


/****************************************************************************\
 *      deleteFile: Deletes an existing file from the directory and deallocates space in FAT
 *
 *      Arguments:
 *          filename1:  The name of the file to delete
 *          verbosity:  1 for more verbose
\****************************************************************************/

void deleteFile(string filename1, int verbosity) {
    if (verbosity) cout << "Transaction: Delete a file\n";

    // Locate file
    auto itr = directory.begin();
    while (itr != directory.end()) {
        if ((*itr).getName() == filename1) break;
        itr++;
    }

    // Non-existing file
    if (itr == directory.end()) {
        if (verbosity) cout << "Error deleting " << filename1 << ", file not found.\n";
        return;
    }

    // Find and erase all related blocks
    for (int current = (*itr).getLocation(); current != -1;) {
        int temp = current;
        current = fat[current];
        fat[temp] = 0;
    }

    // Erase the directory listing
    directory.erase(itr);

    if (verbosity) cout << "Successfully deleted " << filename1 << endl;
}


/****************************************************************************\
 *      copyFile: Copy an existing file to a new file
 *
 *      Arguments:
 *          filename1:  The name of the file to copy
 *          filename2:  The name of the file to copy to
\****************************************************************************/

void copyFile(string filename1, string filename2) {
    cout << "Transaction: Copy a file\n";
    int filename1exists = 0, filename2exists = 0;
    auto fileToCopy = directory.begin();
    for (auto itr = directory.begin(); itr != directory.end(); itr++) {
        if ((*itr).getName() == filename1) {
            filename1exists = 1;
            fileToCopy = itr;
        }
        if ((*itr).getName() == filename2) {
            filename2exists = 1;
        }
    }

    if (filename2exists) {
        cout << "Error copying" << filename1 << ", " << filename2 << " already exists\n";
        return;
    }

    if (!filename1exists) {
        cout << "Error copying " << filename1 << ", file not found.\n";
        return;
    }

    newFile(filename2, (*fileToCopy).getSize(), 0);
    cout << "Successfully copied " << filename1 << " into " << filename2 << endl;
}

/****************************************************************************\
 *      print: Print the contents of the directory and the File Allocation Table
 *
 *      Arguments:
 *          None
\****************************************************************************/

void print() {
    cout << "Directory Listing\n";
    int totalSize = 0;
    int fileCounter = 0;
    // Print the director listing
    for (auto itr = directory.begin(); itr != directory.end(); itr++) {
        int size = (*itr).getSize();
        int location = (*itr).getLocation();
        string name = (*itr).getName();
        cout << "File name: " << setw(25) << left << name << "File Size: " << setw(6) << size << endl;

        if (size < 0) cout << "Cluster(s) in use:  (none)\n";
        else {
            cout << "Cluster(s) in use: ";
            while (location != -1) {
                cout << setw(4) << location;
                location = fat[location];
            }
            cout << endl;
        }

        totalSize += size;
        fileCounter++;
    }
    cout << "Files: " << setw(3) << fileCounter << " Total Size: " << totalSize << " bytes\n\n";

    // Contents of FAT
    cout << "Contents of the File Allocation Table\n";
    for (int i = 0; i < 20; i++ ) {
        cout << "#" << setw(3) << setfill('0') << i*12 << "-" << setw(3) << (i+1)*12-1 << " ";
        for ( int j = 0; j < 12; j++ ) {
            cout << setfill(' ') << setw(4) << fat[i*12 + j] << " ";
        }
        cout << endl;
    }

}
