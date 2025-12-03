#include <iostream>
#include "httplib.h" // The new library
#include "Gene.h"
#include "BTree.h"
#include "HashTable.h"

using namespace std;
using namespace httplib;

// GLOBAL DATABASE
const string DB_FILE = "genes.dat";
BTree geneIndex(3);
HashTable nameIndex;

// Helper to enable CORS (So React can talk to C++)
void enableCORS(Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
    Server svr;
    
    cout << "=== DNA Cloud Server Running on Port 8080 ===" << endl;

    // 1. SEARCH API (e.g., /search?id=101)
    svr.Get("/search", [](const Request& req, Response& res) {
        enableCORS(res);
        
        if (req.has_param("id")) {
            int id = stoi(req.get_param_value("id"));
            long pos = geneIndex.search(id);
            
            if (pos != -1) {
                // Read from disk
                ifstream file(DB_FILE, ios::binary);
                file.seekg(pos);
                Gene g;
                file.read((char*)&g, sizeof(Gene));
                
                // Return simplified JSON-like string
                res.set_content("{ \"found\": true, \"name\": \"" + string(g.name) + "\", \"sequence\": \"" + string(g.sequence) + "\" }", "application/json");
            } else {
                res.set_content("{ \"found\": false }", "application/json");
            }
        }
    });

    // 2. ADD API (e.g., /add?id=101&name=BRCA1&seq=ATCG)
    svr.Post("/add", [](const Request& req, Response& res) {
        enableCORS(res);
        
        // Simulating form data parsing (React will send JSON, but we'll use params for simplicity first)
        // Note: In a real app we'd parse the body, but let's assume URL params for the first test
        // OR better: React sends standard form data.
        
        // For this Step, we will just return success to verify connection
        res.set_content("Received", "text/plain");
    });

    // Handle Pre-flight requests (React checks security first)
    svr.Options("/(.*)", [](const Request& req, Response& res) {
        enableCORS(res);
        res.set_content("", "text/plain");
    });

    // START SERVER
    svr.listen("0.0.0.0", 8080);
}