#ifndef BTREE_H
#define BTREE_H

#include "BTreeNode.h"
#include <iostream>
#include <vector>

using namespace std;

class BTree {
    BTreeNode *root;
    int t; // Minimum degree

public:
    BTree(int _t) {
        root = NULL;
        t = _t;
    }

    // --- STANDARD SEARCH (Single Item) ---
    long search(int k) {
        return (root == NULL) ? -1 : searchRecursive(root, k);
    }

    long searchRecursive(BTreeNode* node, int k) {
        int i = 0;
        while (i < node->n && k > node->keys[i])
            i++;
        if (i < node->n && node->keys[i] == k)
            return node->filePositions[i];
        if (node->leaf)
            return -1;
        return searchRecursive(node->children[i], k);
    }

    // --- NEW FEATURE: RANGE SEARCH (Find IDs between min and max) ---
    // Returns a list of File Positions
    void searchRange(int minK, int maxK, vector<long> &results) {
        if (root != NULL) {
            searchRangeRecursive(root, minK, maxK, results);
        }
    }

    void searchRangeRecursive(BTreeNode* node, int minK, int maxK, vector<long> &results) {
        int i = 0;
        
        // 1. Skip keys that are smaller than minK
        while (i < node->n && node->keys[i] < minK)
            i++;

        // 2. Iterate through valid keys in this node
        while (i < node->n && node->keys[i] <= maxK) {
            // If not leaf, go down to left child first
            if (!node->leaf)
                searchRangeRecursive(node->children[i], minK, maxK, results);
            
            // Collect this key
            results.push_back(node->filePositions[i]);
            i++;
        }

        // 3. If not leaf, go down to the last child
        if (!node->leaf)
            searchRangeRecursive(node->children[i], minK, maxK, results);
    }

    // --- INSERT LOGIC (Same as before) ---
    void insert(int k, long filePos) {
        if (root == NULL) {
            root = new BTreeNode(t, true);
            root->keys[0] = k;
            root->filePositions[0] = filePos;
            root->n = 1;
        } else {
            if (root->n == 2 * t - 1) {
                BTreeNode *s = new BTreeNode(t, false);
                s->children[0] = root;
                splitChild(s, 0, root);
                int i = 0;
                if (s->keys[0] < k) i++;
                insertNonFull(s->children[i], k, filePos);
                root = s;
            } else {
                insertNonFull(root, k, filePos);
            }
        }
    }

private:
    void insertNonFull(BTreeNode *x, int k, long filePos) {
        int i = x->n - 1;
        if (x->leaf) {
            while (i >= 0 && x->keys[i] > k) {
                x->keys[i + 1] = x->keys[i];
                x->filePositions[i + 1] = x->filePositions[i];
                i--;
            }
            x->keys[i + 1] = k;
            x->filePositions[i + 1] = filePos;
            x->n = x->n + 1;
        } else {
            while (i >= 0 && x->keys[i] > k) i--;
            if (x->children[i + 1]->n == 2 * t - 1) {
                splitChild(x, i + 1, x->children[i + 1]);
                if (x->keys[i + 1] < k) i++;
            }
            insertNonFull(x->children[i + 1], k, filePos);
        }
    }

    void splitChild(BTreeNode *x, int i, BTreeNode *y) {
        BTreeNode *z = new BTreeNode(t, y->leaf);
        z->n = t - 1;
        for (int j = 0; j < t - 1; j++) {
            z->keys[j] = y->keys[j + t];
            z->filePositions[j] = y->filePositions[j + t];
        }
        if (!y->leaf) {
            for (int j = 0; j < t; j++)
                z->children[j] = y->children[j + t];
        }
        y->n = t - 1;
        for (int j = x->n; j >= i + 1; j--)
            x->children[j + 1] = x->children[j];
        x->children[i + 1] = z;
        for (int j = x->n - 1; j >= i; j--) {
            x->keys[j + 1] = x->keys[j];
            x->filePositions[j + 1] = x->filePositions[j];
        }
        x->keys[i] = y->keys[t - 1];
        x->filePositions[i] = y->filePositions[t - 1];
        x->n = x->n + 1;
    }
};

#endif