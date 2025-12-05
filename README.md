# 🧬 Cloud-Based DNA Sequence Indexing System

### A Distributed Genome Search Engine using Advanced Data Structures

## 📌 Project Overview
This project is a **3-Tier Full Stack Application** designed to store, index, and retrieve massive amounts of DNA records. Since genetic data is too large to fit into standard memory (RAM), this system implements a **custom file-based database engine** using **B-Trees** for disk storage and **Hashing** for instant lookups.

The project simulates a real-world bioinformatics cloud system where the Frontend (User) is decoupled from the Backend (Server) and Storage (Disk).

## 🎯 Problem Statement
*   **Big Data:** DNA sequences are massive. Storing them in arrays or linked lists causes Memory Overflow.
*   **Search Latency:** Finding a specific gene in a billion records takes too long with linear search.
*   **Solution:** We use **B-Trees** (Order 3) to minimize disk reads and **Hashing** for O(1) name lookups.
## 🛠️ Tech Stack & Architecture

### 🏗️ 3-Tier Architecture
This project strictly follows the **Client-Server-Database** model:

*   **Tier 1: Frontend (React.js)**
    *   Provides a professional SaaS-style dashboard.
    *   Communicates with the backend via **JSON over HTTP**.
    *   Features: Dark/Light mode, Data Visualization, Range Query UI.

*   **Tier 2: Backend (C++ Custom Server)**
    *   A custom-written **Multi-threaded HTTP Server** using raw Sockets (`winsock2`).
    *   Handles API requests (`/add`, `/search`, `/range`).
    *   Manages memory and file pointers.

*   **Tier 3: Database (Custom Binary Storage)**
    *   **No SQL/NoSQL used.**
    *   Data is written to `genes.dat` in binary format.
    *   **Disk Manipulation:** Implemented using `fstream` `seekp` and `tellp`.

### 🧩 Data Structures Used
| Structure | Usage | Complexity |
| :--- | :--- | :--- |
| **B-Tree** | Indexing Gene IDs & Range Queries | `O(log n)` |
| **Hash Table** | Indexing Gene Names | `O(1)` |
| **Vector** | Temporary storage for range results | `O(n)` |
## 🚀 How to Run

### Prerequisities
*   **C++ Compiler:** MinGW (Windows) or G++ (Linux)
*   **Node.js:** For the React Frontend

### Step 1: Start the Backend (The Brain)
Open a terminal in the `backend/` folder:
```bash
cd backend
g++ main.cpp -o server -lws2_32
./server

3.  **Run these commands:**
```powershell  

## 🔮 Future Scope & Roadmap (v2.0)
While the current system successfully implements the core 3-Tier architecture and B-Tree indexing, the following features are planned for the next release:

1.  **🧬 3D DNA Visualization:**
    *   Implementing a WebGL viewer in React to show the double-helix structure of the stored sequences.
2.  **🔐 User Authentication:**
    *   Adding JWT (JSON Web Tokens) to the C++ backend to allow secure login for doctors/researchers.
3.  **⚡ Multi-Threading Optimization:**
    *   Enhancing the custom server to handle 1000+ concurrent connections using a Thread Pool.
4.  **cloud-Integration:**
    *   Dockerizing the application for easy deployment on Kubernetes clusters.