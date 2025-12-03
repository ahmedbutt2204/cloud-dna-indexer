# Cloud-Based DNA Sequence Indexing System

## 1. Project Overview
This project is a **Distributed Genome Indexing System** designed to store and efficiently search large datasets of DNA records (Genes). Since DNA data is voluminous, it cannot fit entirely in memory (RAM). This system implements a **custom disk-based storage engine** using **B-Trees** and **Hashing** to manage data on a cloud server.

This project strictly follows a **3-Tier Architecture** (Frontend → Backend → Custom Database).

## 2. Architecture & Technologies
*   **Tier 1: Frontend (GUI)**
    *   **Technology:** React.js (Running on Client Laptop)
    *   **Role:** Provides a user-friendly interface to Add, Delete, and Search genes. It sends HTTP requests to the Cloud Backend.
*   **Tier 2: Backend (Logic)**
    *   **Technology:** C++ (Hosted on AWS/Google Cloud)
    *   **Library:** Crow / cpp-httplib (For API handling)
    *   **Role:** Processes requests and executes Data Structure logic.
*   **Tier 3: Database (Storage)**
    *   **Technology:** Custom C++ File Handling (No SQL/Firebase used)
    *   **Role:** Implements "Disk Manipulation" by reading/writing binary data files directly.

## 3. Data Structures Implementation
### A. B-Tree (Primary Index - 100% Focus)
*   **Usage:** Used to store and index **Gene IDs** (Integers).
*   **Disk Manipulation:** The B-Tree is not just in RAM; it writes nodes to a hard disk file (`genes.dat`). This allows the system to handle data larger than the available memory.
*   **Operations:** Insert, Delete, Search, and Range Query (e.g., "Find genes 100 to 500").

### B. Hashing (Secondary Index - 40% Focus)
*   **Usage:** Used to map **Gene Names** (Strings) to their IDs.
*   **Technique:** A Hash Table helps in `O(1)` fast lookup when searching for a specific gene name.

## 4. Key Features
1.  **Cloud Hosting:** The C++ backend runs on a remote Linux Virtual Machine.
2.  **Custom Persistence:** Implements a mini-database engine from scratch (avoiding MySQL/MongoDB).
3.  **Networked GUI:** Connects a modern React frontend to a raw C++ backend.

## 5. How to Run
*(Instructions will be added here as development progresses)*
