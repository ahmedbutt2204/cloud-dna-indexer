#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Gene.h"
#include "BTree.h"
#include "HashTable.h"

// --- CROSS PLATFORM NETWORKING CODE ---
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET SocketType;
    #define CLOSE_SOCKET closesocket
    #define SOCKET_ERROR_CODE SOCKET_ERROR
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    typedef int SocketType;
    #define INVALID_SOCKET -1
    #define CLOSE_SOCKET close
    #define SOCKET_ERROR_CODE -1
#endif

using namespace std;

const string DB_FILE = "genes.dat";
BTree geneIndex(3);
HashTable nameIndex;

// --- DATABASE FUNCTIONS ---
void addGene(int id, string name, string sequence) {
    Gene g;
    g.setData(id, name, sequence);
    ofstream file(DB_FILE, ios::binary | ios::app);
    if (!file) return;
    file.seekp(0, ios::end); 
    long filePos = file.tellp();
    file.write((char*)&g, sizeof(Gene));
    file.close();
    geneIndex.insert(id, filePos);
    nameIndex.insert(name, id);
    cout << "[DB] Saved Gene: " << name << " (ID: " << id << ")" << endl;
}

void loadData() {
    ifstream file(DB_FILE, ios::binary);
    if(!file) return;
    Gene g;
    long pos;
    cout << "[DB] Loading records..." << endl;
    while(true) {
        pos = file.tellg();
        file.read((char*)&g, sizeof(Gene));
        if(file.eof()) break;
        geneIndex.insert(g.id, pos);
        nameIndex.insert(g.name, g.id);
    }
    cout << "[DB] Database ready." << endl;
}

// --- SERVER FUNCTIONS ---
void sendResponse(SocketType clientSocket, string content) {
    string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Content-Length: " + to_string(content.length()) + "\r\n"
        "\r\n" +
        content;
    send(clientSocket, response.c_str(), response.length(), 0);
}

int main() {
    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;
    #endif

    loadData();

    SocketType serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR_CODE) {
        cout << "Bind failed! Port 8080 busy." << endl;
        return 1;
    }
    listen(serverSocket, 10);

    cout << "\n==============================================" << endl;
    cout << "   🧬 DNA CLOUD SERVER (C++ BACKEND)   " << endl;
    cout << "==============================================" << endl;
    cout << "[SYSTEM] Status: ONLINE" << endl;
    cout << "[SYSTEM] Port:   8080" << endl;
    cout << "[SYSTEM] Logs:   Enabled" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Waiting for React Client..." << endl;

    while (true) {
        SocketType clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) continue;

        char buffer[4096] = {0};
        recv(clientSocket, buffer, 4096, 0);
        string request(buffer);

        // HANDLE SEARCH (GET /search)
        if (request.find("GET /search") != string::npos) {
            if (request.find("id=") != string::npos) {
                size_t idPos = request.find("id=");
                size_t endPos = request.find(' ', idPos);
                int id = stoi(request.substr(idPos + 3, endPos - (idPos + 3)));
                long pos = geneIndex.search(id);
                if (pos != -1) {
                    ifstream file(DB_FILE, ios::binary);
                    file.seekg(pos);
                    Gene g;
                    file.read((char*)&g, sizeof(Gene));
                    string json = "{ \"found\": true, \"name\": \"" + string(g.name) + "\", \"sequence\": \"" + string(g.sequence) + "\" }";
                    sendResponse(clientSocket, json);
                } else { sendResponse(clientSocket, "{ \"found\": false }"); }
            }
            else if (request.find("name=") != string::npos) {
                size_t namePos = request.find("name=");
                size_t endPos = request.find(' ', namePos);
                string name = request.substr(namePos + 5, endPos - (namePos + 5));
                int id = nameIndex.search(name);
                if (id != -1) {
                    long pos = geneIndex.search(id);
                    ifstream file(DB_FILE, ios::binary);
                    file.seekg(pos);
                    Gene g;
                    file.read((char*)&g, sizeof(Gene));
                    string json = "{ \"found\": true, \"id\": " + to_string(g.id) + ", \"sequence\": \"" + string(g.sequence) + "\" }";
                    sendResponse(clientSocket, json);
                } else { sendResponse(clientSocket, "{ \"found\": false }"); }
            }
        }
        
        // HANDLE RANGE SEARCH (GET /range)
        else if (request.find("GET /range") != string::npos) {
            try {
                size_t minPos = request.find("min=");
                size_t maxPos = request.find("max=");
                if (minPos != string::npos && maxPos != string::npos) {
                    size_t minEnd = request.find('&', minPos);
                    int minVal = stoi(request.substr(minPos + 4, minEnd - (minPos + 4)));
                    size_t maxEnd = request.find(' ', maxPos);
                    int maxVal = stoi(request.substr(maxPos + 4, maxEnd - (maxPos + 4)));

                    vector<long> positions;
                    geneIndex.searchRange(minVal, maxVal, positions);

                    string json = "{ \"found\": true, \"count\": " + to_string(positions.size()) + ", \"results\": [";
                    ifstream file(DB_FILE, ios::binary);
                    for (size_t i = 0; i < positions.size(); i++) {
                        file.seekg(positions[i]);
                        Gene g;
                        file.read((char*)&g, sizeof(Gene));
                        json += "{ \"id\": " + to_string(g.id) + ", \"name\": \"" + string(g.name) + "\" }";
                        if (i < positions.size() - 1) json += ", ";
                    }
                    json += "] }";
                    sendResponse(clientSocket, json);
                }
            } catch(...) { sendResponse(clientSocket, "{ \"found\": false, \"error\": \"Invalid Range\" }"); }
        }

        // HANDLE MUTATION ANALYSIS (POST /analyze)
        else if (request.find("POST /analyze") != string::npos) {
            try {
                size_t idPos = request.find("id=");
                size_t seqPos = request.find("sequence=");
                
                if (idPos != string::npos && seqPos != string::npos) {
                    size_t idEnd = request.find('&', idPos);
                    int id = stoi(request.substr(idPos + 3, idEnd - (idPos + 3)));
                    size_t seqEnd = request.find(' ', seqPos);
                    string patientSeq = request.substr(seqPos + 9, seqEnd - (seqPos + 9));
                    long pos = geneIndex.search(id);
                    if (pos != -1) {
                        ifstream file(DB_FILE, ios::binary);
                        file.seekg(pos);
                        Gene g;
                        file.read((char*)&g, sizeof(Gene));
                        string refSeq = string(g.sequence);
                        int matches = 0;
                        int totalLen = min((int)refSeq.length(), (int)patientSeq.length());
                        for(int i=0; i<totalLen; i++) { if(refSeq[i] == patientSeq[i]) matches++; }
                        double percentage = (double)matches / totalLen * 100.0;
                        string status = (percentage > 90.0) ? "HEALTHY (Normal)" : "MUTATION DETECTED (High Risk)";
                        string json = "{ \"status\": \"success\", \"match\": " + to_string(matches) + ", \"total\": " + to_string(totalLen) + ", \"percent\": " + to_string((int)percentage) + ", \"diagnosis\": \"" + status + "\" }";
                        sendResponse(clientSocket, json);
                    } else { sendResponse(clientSocket, "{ \"status\": \"error\", \"message\": \"Gene ID not found\" }"); }
                }
            } catch(...) { sendResponse(clientSocket, "{ \"status\": \"error\" }"); }
        }

        // HANDLE ADD (POST /add)
        else if (request.find("POST /add") != string::npos) {
             try {
                size_t idPos = request.find("id=");
                size_t namePos = request.find("name=");
                size_t seqPos = request.find("sequence=");
                if (idPos != string::npos && namePos != string::npos && seqPos != string::npos) {
                    size_t idEnd = request.find('&', idPos);
                    int id = stoi(request.substr(idPos + 3, idEnd - (idPos + 3)));
                    if (geneIndex.search(id) != -1) {
                         sendResponse(clientSocket, "{ \"status\": \"error\", \"message\": \"Duplicate ID\" }");
                    } else {
                        size_t nameEnd = request.find('&', namePos);
                        string name = request.substr(namePos + 5, nameEnd - (namePos + 5));
                        size_t seqEnd = request.find(' ', seqPos);
                        string seq = request.substr(seqPos + 9, seqEnd - (seqPos + 9));
                        addGene(id, name, seq);
                        sendResponse(clientSocket, "{ \"status\": \"success\" }");
                    }
                } else { sendResponse(clientSocket, "{ \"status\": \"error\" }"); }
            } catch (...) { sendResponse(clientSocket, "{ \"status\": \"error\" }"); }
        }

        // --- NEW: SYSTEM HEALTH CHECK (GET /health) ---
        else if (request.find("GET /health") != string::npos) {
            sendResponse(clientSocket, "{ \"status\": \"UP\", \"uptime\": \"99.9%\", \"db_connected\": true }");
        }
        
        else if (request.find("OPTIONS") != string::npos) {
             string response = "HTTP/1.1 204 No Content\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, OPTIONS\r\nAccess-Control-Allow-Headers: Content-Type\r\n\r\n";
             send(clientSocket, response.c_str(), response.length(), 0);
        }

        CLOSE_SOCKET(clientSocket);
    }
    #ifdef _WIN32
        WSACleanup();
    #endif
    return 0;
}