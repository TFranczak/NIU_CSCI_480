/*  Name: Tommy Franczak
 *  Class: 480-0001
 *  Title: Assignment 7 Entry.h
 *
 *  Functionality: Simulates a file in our directory
 */


#ifndef ENTRY_H
#define ENTRY_H

#include <string.h>

class Entry {
    std::string name;
    int location;
    int size;

    public:
    // Constructors
    Entry(){};
    Entry(int location, int size, std::string name) {
        this->name = name;
        this->location = location;
        this->size = size;
    };

    // Getters
    int getLocation() { return location; };
    std::string getName() { return name; };
    int getSize() { return size; };

    // Setters
    void setLocation(int location) { this->location = location; };
    void setName(std::string name) { this->name = name; };
    void setSize(int size) { this->size = size;};
};

#endif
