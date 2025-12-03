#include <iostream>
#include <fstream>
#include "Gene.h"
#include "BTree.h"
#include "HashTable.h"

using namespace std;

const string DB_FILE = "genes.dat";

// GLOBAL DATA STRUCTURES
BTree geneIndex(3);      // Handles ID Search (B-Tree)
HashTable nameIndex;     // Handles Name Search (Hashing)

// Function to add a gene
void addGene(int id, string name, string sequence) {
    Gene g;
    g.setData(id, name, sequence);

    // 1. Open File
    ofstream file(DB_FILE, ios::binary | ios::app);
    if (!file) {
        cout << "Error opening database!" << endl;
        return;
    }

    // 2. FIX: Force pointer to the end to get correct position
    file.seekp(0, ios::end); 
    long filePos = file.tellp();

    // 3. Write Data
    file.write((char*)&g, sizeof(Gene));
    file.close();

    // 4. Update Indices
    geneIndex.insert(id, filePos);  // Update B-Tree
    nameIndex.insert(name, id);     // Update Hash Table

    cout << "Saved Gene: " << name << " (ID: " << id << ") at Disk Pos: " << filePos << endl;
}

// Search by ID (B-Tree)
void searchByID(int id) {
    long pos = geneIndex.search(id);
    if (pos == -1) {
        cout << "[Fail] ID " << id << " not found." << endl;
        return;
    }

    ifstream file(DB_FILE, ios::binary);
    file.seekg(pos);
    Gene g;
    file.read((char*)&g, sizeof(Gene));
    cout << "[Found via B-Tree] ID: " << g.id << " | Name: " << g.name << endl;
}

// Search by Name (Hashing)
void searchByName(string name) {
    // 1. Get ID from Hash Table
    int id = nameIndex.search(name);
    if (id == -1) {
        cout << "[Fail] Name '" << name << "' not found." << endl;
        return;
    }
    
    // 2. Use that ID to search B-Tree
    cout << "[Found via Hashing] Name '" << name << "' maps to ID " << id << ". Fetching..." << endl;
    searchByID(id);
}

int main() {
    cout << "=== DNA Indexer (Final Backend) ===" << endl;

    // Add Data
    addGene(101, "BRCA1", "ATCG-SEQ-1");
    addGene(202, "TP53", "GGGG-SEQ-2");
    addGene(500, "EGFR", "AAAA-SEQ-3");

    cout << "\n--- Testing Search ---" << endl;
    searchByID(202);          // Uses B-Tree
    searchByName("EGFR");     // Uses Hashing -> B-Tree
    searchByName("UNKNOWN");  // Should fail

    return 0;
}