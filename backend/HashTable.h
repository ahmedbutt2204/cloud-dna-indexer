#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <vector>
#include <list>
#include <string>

using namespace std;

// A simple Hash Table that maps "Gene Name" -> "Gene ID"
class HashTable {
    // We use a fixed size array of lists (Chaining method for collisions)
    static const int TABLE_SIZE = 100;
    list<pair<string, int>> table[TABLE_SIZE];

    // Simple Hash Function: Sum of ASCII characters
    int hashFunction(string key) {
        int sum = 0;
        for (char c : key) {
            sum += c;
        }
        return sum % TABLE_SIZE;
    }

public:
    // Insert a Name -> ID mapping
    void insert(string name, int id) {
        int index = hashFunction(name);
        table[index].push_back({name, id});
    }

    // Search for a Name and get the ID
    int search(string name) {
        int index = hashFunction(name);
        for (auto entry : table[index]) {
            if (entry.first == name) {
                return entry.second; // Return the ID found
            }
        }
        return -1; // Not found
    }
};

#endif