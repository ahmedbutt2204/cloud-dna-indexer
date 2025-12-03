#include <iostream>
#include <fstream>
#include "Gene.h"
#include "BTree.h"

using namespace std;

const string DB_FILE = "genes.dat";
BTree geneIndex(3); // Degree 3

void addGene(int id, string name, string sequence) {
    Gene g;
    g.setData(id, name, sequence);

    // 1. Write to Disk
    ofstream file(DB_FILE, ios::binary | ios::app);
    if (!file) return;
    
    long filePos = file.tellp(); // Get address on disk
    file.write((char*)&g, sizeof(Gene));
    file.close();

    // 2. Index in B-Tree
    geneIndex.insert(id, filePos);
    cout << "Saved ID: " << id << " at Disk Pos: " << filePos << endl;
}

void searchGene(int id) {
    // 1. Ask B-Tree for address
    long pos = geneIndex.search(id);
    if (pos == -1) {
        cout << "ID " << id << " not found." << endl;
        return;
    }

    // 2. Read from Disk using address
    ifstream file(DB_FILE, ios::binary);
    file.seekg(pos);
    Gene g;
    file.read((char*)&g, sizeof(Gene));
    
    cout << "Found -> ID: " << g.id << ", Name: " << g.name << endl;
}

int main() {
    cout << "--- DNA Indexer ---" << endl;
    
    addGene(101, "BRCA1", "ATCG");
    addGene(202, "TP53", "GGGG");
    
    cout << "\nSearching..." << endl;
    searchGene(101);
    searchGene(999);

    return 0;
}