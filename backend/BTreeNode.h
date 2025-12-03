#ifndef BTREENODE_H
#define BTREENODE_H

#include <iostream>
#include <vector>

using namespace std;

struct BTreeNode {
    int *keys;              // Array of Gene IDs
    long *filePositions;    // Array of file locations (Disk addresses)
    BTreeNode **children;   // Pointers to child nodes
    int n;                  // Current number of keys
    bool leaf;              // Is this a leaf?

    BTreeNode(int t, bool isLeaf) {
        this->leaf = isLeaf;
        keys = new int[2 * t - 1];
        filePositions = new long[2 * t - 1];
        children = new BTreeNode *[2 * t];
        n = 0;
    }
};

#endif