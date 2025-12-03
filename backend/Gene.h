#ifndef GENE_H
#define GENE_H

#include <iostream>
#include <cstring>
#include <string>

// The Data Structure
struct Gene {
    int id;               // Key
    char name[50];        // Value 1
    char sequence[100];   // Value 2
    bool isDeleted;

    Gene() {
        id = -1;
        std::memset(name, 0, 50);
        std::memset(sequence, 0, 100);
        isDeleted = false;
    }

    void setData(int _id, std::string _name, std::string _seq) {
        id = _id;
        std::strncpy(name, _name.c_str(), 49);
        std::strncpy(sequence, _seq.c_str(), 99);
        isDeleted = false;
    }
};

#endif