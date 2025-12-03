#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <winsock2.h> // Standard Windows Networking
#include <ws2tcpip.h>
#include "Gene.h"
#include "BTree.h"
#include "HashTable.h"

// LINKING WINSOCK (Critical for Windows)
#pragma comment(lib, "ws2_32.lib")

using namespace std;

const string DB_FILE = "genes.dat";
BTree geneIndex(3);
HashTable nameIndex;

// --- DATABASE FUNCTIONS ---

// Function to add a gene to Disk + B-Tree + Hash
void addGene(int id, string name, string sequence) {
    Gene g;
    g.setData(id, name, sequence);

    // 1. Open File
    ofstream file(DB_FILE, ios::binary | ios::app);
    if (!file) {
        cout << "Error opening database!" << endl;
        return;
    }

    // 2. Force pointer to end
    file.seekp(0, ios::end); 
    long filePos = file.tellp();

    // 3. Write Data
    file.write((char*)&g, sizeof(Gene));
    file.close();

    // 4. Update Indices
    geneIndex.insert(id, filePos);
    nameIndex.insert(name, id);

    cout << "[DB] Saved Gene: " << name << " (ID: " << id << ") at Disk Pos: " << filePos << endl;
}

// Load existing data on startup
void loadData() {
    ifstream file(DB_FILE, ios::binary);
    if(!file) return;
    
    Gene g;
    long pos;
    cout << "[DB] Loading existing records..." << endl;
    while(true) {
        pos = file.tellg();
        file.read((char*)&g, sizeof(Gene));
        if(file.eof()) break;
        
        // Re-build indices
        geneIndex.insert(g.id, pos);
        nameIndex.insert(g.name, g.id);
        cout << " - Loaded ID: " << g.id << " (" << g.name << ")" << endl;
    }
    cout << "[DB] Database ready." << endl;
}

// --- SERVER FUNCTIONS ---

// Manual HTTP Response Generator
void sendResponse(SOCKET clientSocket, string content) {
    string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n" // Fixes CORS
        "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Content-Length: " + to_string(content.length()) + "\r\n"
        "\r\n" +
        content;
    
    send(clientSocket, response.c_str(), response.length(), 0);
}

// --- MAIN SERVER LOOP ---

int main() {
    // 1. Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Winsock failed!" << endl;
        return 1;
    }

    // 2. Load Data from Disk
    loadData();

    // 3. Create Socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080); // Port 8080

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cout << "Bind failed! Port 8080 might be in use." << endl;
        return 1;
    }
    listen(serverSocket, SOMAXCONN);

    cout << "=== DNA Cloud Server Running on Port 8080 ===" << endl;

    // 4. Listen for React
    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) continue;

        char buffer[4096] = {0};
        recv(clientSocket, buffer, 4096, 0);
        string request(buffer);

        // --- HANDLE SEARCH (GET /search) ---
        if (request.find("GET /search") != string::npos) {
            
            // OPTION A: Search by ID (B-Tree)
            if (request.find("id=") != string::npos) {
                size_t idPos = request.find("id=");
                // Find end of ID (space or newline)
                size_t endPos = request.find(' ', idPos);
                string idStr = request.substr(idPos + 3, endPos - (idPos + 3));
                int id = stoi(idStr);
                
                long pos = geneIndex.search(id);
                if (pos != -1) {
                    ifstream file(DB_FILE, ios::binary);
                    file.seekg(pos);
                    Gene g;
                    file.read((char*)&g, sizeof(Gene));
                    string json = "{ \"found\": true, \"name\": \"" + string(g.name) + "\", \"sequence\": \"" + string(g.sequence) + "\" }";
                    sendResponse(clientSocket, json);
                } else {
                    sendResponse(clientSocket, "{ \"found\": false }");
                }
            }
            // OPTION B: Search by Name (Hashing)
            else if (request.find("name=") != string::npos) {
                size_t namePos = request.find("name=");
                size_t endPos = request.find(' ', namePos);
                string name = request.substr(namePos + 5, endPos - (namePos + 5));
                
                // 1. Use Hash Table to find ID
                int id = nameIndex.search(name);
                
                if (id != -1) {
                    // 2. Use ID to search B-Tree
                    long pos = geneIndex.search(id);
                    ifstream file(DB_FILE, ios::binary);
                    file.seekg(pos);
                    Gene g;
                    file.read((char*)&g, sizeof(Gene));
                    
                    string json = "{ \"found\": true, \"id\": " + to_string(g.id) + ", \"sequence\": \"" + string(g.sequence) + "\" }";
                    sendResponse(clientSocket, json);
                } else {
                    sendResponse(clientSocket, "{ \"found\": false }");
                }
            }
        }
        
        // --- HANDLE ADD (POST /add) ---
        else if (request.find("POST /add") != string::npos) {
            try {
                size_t idPos = request.find("id=");
                size_t namePos = request.find("name=");
                size_t seqPos = request.find("sequence=");
                
                if (idPos != string::npos && namePos != string::npos && seqPos != string::npos) {
                    // Extract ID
                    size_t idEnd = request.find('&', idPos);
                    int id = stoi(request.substr(idPos + 3, idEnd - (idPos + 3)));
                    
                    // Extract Name
                    size_t nameEnd = request.find('&', namePos);
                    string name = request.substr(namePos + 5, nameEnd - (namePos + 5));
                    
                    // Extract Sequence (Stop at HTTP version or space)
                    size_t seqEnd = request.find(' ', seqPos);
                    string seq = request.substr(seqPos + 9, seqEnd - (seqPos + 9));

                    // SAVE TO DISK & B-TREE
                    addGene(id, name, seq);
                    
                    sendResponse(clientSocket, "{ \"status\": \"success\" }");
                } else {
                    sendResponse(clientSocket, "{ \"status\": \"error\" }");
                }
            } catch (...) {
                sendResponse(clientSocket, "{ \"status\": \"error\" }");
            }
        }
        
        // --- HANDLE OPTIONS (Pre-flight for CORS) ---
        else if (request.find("OPTIONS") != string::npos) {
             string response = "HTTP/1.1 204 No Content\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, OPTIONS\r\nAccess-Control-Allow-Headers: Content-Type\r\n\r\n";
             send(clientSocket, response.c_str(), response.length(), 0);
        }

        closesocket(clientSocket);
    }

    WSACleanup();
    return 0;
}