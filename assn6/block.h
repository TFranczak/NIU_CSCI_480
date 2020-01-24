#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <iostream>
#include <iomanip>

// class block
// Used to simulate blocks of memory for Assignment 6
class block {
    private:
    int start;
    int size;
    std::string pid = "";
    std::string bid = "";
    block* first;
    block* second;

    public:
    // Constructors
    block(){};
    block(int, int);
    block(int, int, std::string);
    block(int, int, std::string, std::string);
    block(const block&);

    // Get methods: Return the corresponding private variable
    std::string getPid() const { return pid; };
    std::string getBid() const { return bid; };
    int getStart() const { return start; };
    int getSize() const { return size; };

    void allocate(int);
    void append(int);

    friend std::ostream& operator<<(std::ostream&, const block&);
};

// Block Constructor
// Create a unallocated block of memory
// Arguments:
//      size:   The size of the block
//      start:  The starting address of the block
block::block(int size, int start) {
    this->size = size;
    this->start = start;
    return;
}

// Block constructor
// Create an allocated block of memory
// Arguments:
//      size:   The size of the block               (int)
//      start:  The starting address of the block   (int)
//      pid:    The ID of the owning process        (string)
//      bid:    The ID of the current block         (string)
block::block(int size, int start, std::string pid, std::string bid) {
    this->size = size;
    this->start = start;
    this->pid = pid;
    this->bid = bid;
    return;
}

// Block copy constructor
//  an allocated block of memory
// Arguments:
//  rhs:    The block object to be copied   (block&)
block::block(const block& rhs) {
    this->size = rhs.getSize();
    this->start = rhs.getStart();
    this->pid = rhs.getPid();
    this->bid = rhs.getBid();
    return;
}

// operator<< overload
// Return information about the object for easy printing.
// Arguments:
//      os:     The ostream object to print to
//      myblock: The block to print

std::ostream& operator<<(std::ostream& os, const block& myblock) {
    // Unallocated block
    if (myblock.getBid() == "" && myblock.getPid() == "") {
        os << "Start Address = " << std::setw(8) << myblock.getStart();
        os << " | ";
        os << "Size = " << std::setw(7) << myblock.getSize();
        os << std::endl;
    }
    // Allocated block
    else {
        os << "Start Address = " << std::setw(8) << myblock.getStart();
        os << " | ";
        os << "Size = " << std::setw(7) << myblock.getSize();
        os << " | ";
        os << "PID = " << std::setw(4) << myblock.getPid();
        os << " | ";
        os << "BID = " << std::setw(4) << myblock.getBid();
        os << std::endl;
    }
    return os;
}

// allocate
// Allocate space at beginning of the block.
// Arguments:
//  size: The size of the allocation    (int)

void block::allocate(int size) {
    this->start += size;
    this->size = this->size - size;
    return;
}

// append
// Append two blocks together
// Arguments:
//  size: Size to append on the block   (int)

void block::append(int size) {
    this->size += size;
    return;
}


#endif
