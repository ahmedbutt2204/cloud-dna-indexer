// FILE: backend/BTreeNode.h
#ifndef BTREENODE_H
#define BTREENODE_H

#include <iostream>
#include <vector>

using namespace std;

// A B-Tree Node
struct BTreeNode {
    // Arrays to store keys and file positions
    // t is the minimum degree. Let's say t=3.
    // Max keys = 2*t - 1 = 5
    int *keys;              // The Gene IDs
    long *filePositions;    // Where in the file this Gene is stored
    BTreeNode **children;   // Pointers to child nodes
    int n;                  // Current number of keys
    bool leaf;              // Is this a leaf node?

    BTreeNode(int t, bool isLeaf) {
        this->leaf = isLeaf;
        keys = new int[2 * t - 1];
        filePositions = new long[2 * t - 1];
        children = new BTreeNode *[2 * t];
        n = 0;
    }

    // Traverse all nodes in a subtree rooted with this node
    void traverse() {
        int i;
        for (i = 0; i < n; i++) {
            if (!leaf) children[i]->traverse();
            cout << "ID: " << keys[i] << " (FilePos: " << filePositions[i] << ")" << endl;
        }
        if (!leaf) children[i]->traverse();
    }
};

#endif