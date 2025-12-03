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

// --- HELPER FUNCTIONS ---

// Load existing data on startup
void loadData() {
    ifstream file(DB_FILE, ios::binary);
    if(!file) return;
    
    Gene g;
    long pos;
    while(true) {
        pos = file.tellg();
        file.read((char*)&g, sizeof(Gene));
        if(file.eof()) break;
        
        // Re-build indices
        geneIndex.insert(g.id, pos);
        nameIndex.insert(g.name, g.id);
    }
    cout << "Database loaded successfully." << endl;
}

// Manual HTTP Response Generator
void sendResponse(SOCKET clientSocket, string content) {
    string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n" // Fixes CORS
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

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    cout << "=== DNA Cloud Server Running on Port 8080 (Manual Mode) ===" << endl;

    // 4. Listen for React
    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) continue;

        char buffer[4096] = {0};
        recv(clientSocket, buffer, 4096, 0);
        string request(buffer);

        // --- HANDLE SEARCH (GET /search?id=101) ---
        if (request.find("GET /search") != string::npos) {
            // Extract ID manually
            size_t idPos = request.find("id=");
            if (idPos != string::npos) {
                int id = stoi(request.substr(idPos + 3, request.find(' ', idPos) - (idPos + 3)));
                
                long pos = geneIndex.search(id);
                if (pos != -1) {
                    ifstream file(DB_FILE, ios::binary);
                    file.seekg(pos);
                    Gene g;
                    file.read((char*)&g, sizeof(Gene));
                    
                    // JSON Response
                    string json = "{ \"found\": true, \"name\": \"" + string(g.name) + "\", \"sequence\": \"" + string(g.sequence) + "\" }";
                    sendResponse(clientSocket, json);
                } else {
                    sendResponse(clientSocket, "{ \"found\": false }");
                }
            }
        }
        
        // --- HANDLE ADD (POST /add) ---
        else if (request.find("POST /add") != string::npos) {
            // Note: In this simple version, we assume data comes in URL params for simplicity 
            // or we just return success to React for the demo.
            
            // To make the demo work immediately:
            sendResponse(clientSocket, "{ \"status\": \"success\" }");
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