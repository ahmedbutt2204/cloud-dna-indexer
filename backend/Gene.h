// FILE: backend/Gene.h
#ifndef GENE_H
#define GENE_H

#include <iostream>
#include <cstring>
#include <string>

// This struct represents one record in our database
// We use fixed-size char arrays so we can write to binary files easily.
struct Gene {
    int id;               // Key for B-Tree (e.g., 101)
    char name[50];        // Key for Hashing (e.g., "BRCA1")
    char sequence[100];   // DNA Data (e.g., "ATCG...")
    bool isDeleted;       // For deletion logic

    // Constructor to clean data
    Gene() {
        id = -1;
        std::memset(name, 0, 50);
        std::memset(sequence, 0, 100);
        isDeleted = false;
    }

    // Helper function to fill data easily
    void setData(int _id, std::string _name, std::string _seq) {
        id = _id;
        // Copy string to char array safely
        std::strncpy(name, _name.c_str(), 49);
        std::strncpy(sequence, _seq.c_str(), 99);
        isDeleted = false;
    }
};

#endif