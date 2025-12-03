// FILE: backend/main.cpp
#include <iostream>
#include <fstream>
#include "Gene.h"
#include "BTree.h"

using namespace std;

const string DB_FILE = "genes.dat";
// Create a B-Tree with minimum degree 3
BTree geneIndex(3); 

// 1. Save Gene to Disk AND Index it in B-Tree
void addGene(int id, string name, string sequence) {
    Gene g;
    g.setData(id, name, sequence);

    // Open file in Append + Binary mode
    ofstream file(DB_FILE, ios::binary | ios::app);
    if (!file) {
        cout << "Error opening database!" << endl;
        return;
    }

    // DISK MANIPULATION:
    // Get current position in file (Where this record will live)
    long filePos = file.tellp();
    
    // Write data to disk
    file.write((char*)&g, sizeof(Gene));
    file.close();

    // B-TREE INDEXING:
    // Map the ID -> File Position
    geneIndex.insert(id, filePos);

    cout << "[Success] Added Gene ID: " << id << " at Disk Position: " << filePos << endl;
}

// 2. Search using B-Tree
void searchGene(int id) {
    // A. Ask B-Tree where the data is on the disk
    long pos = geneIndex.search(id);

    if (pos == -1) {
        cout << "[Not Found] Gene ID " << id << " does not exist." << endl;
        return;
    }

    // B. Go to that exact spot on the disk (Random Access)
    ifstream file(DB_FILE, ios::binary);
    file.seekg(pos); // Jump to the byte

    Gene g;
    file.read((char*)&g, sizeof(Gene));
    
    cout << "\n--- SEARCH RESULT ---" << endl;
    cout << "ID: " << g.id << endl;
    cout << "Name: " << g.name << endl;
    cout << "Sequence: " << g.sequence << endl;
    cout << "---------------------\n" << endl;
}

int main() {
    cout << "=== DNA Indexer System (B-Tree Integrated) ===" << endl;

    // Test Adding Data
    addGene(101, "BRCA1", "ATCG-CGTA");
    addGene(505, "INSULIN", "GGGG-AAAA");
    addGene(202, "TP53", "TTTT-CCCC");

    // Test Searching Data
    cout << "Searching for ID 505..." << endl;
    searchGene(505);

    cout << "Searching for ID 999 (Fake)..." << endl;
    searchGene(999);

    return 0;
}