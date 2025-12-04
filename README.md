# 🧬 Cloud-Based DNA Sequence Indexing System

### 🚀 A Distributed Genome Search Engine using Advanced Data Structures

## 📌 Project Overview
This project is a **3-Tier Full Stack Application** designed to store, index, and retrieve massive amounts of DNA records. Since genetic data is too large to fit into standard memory (RAM), this system implements a **custom file-based database engine** using **B-Trees** for disk storage and **Hashing** for instant lookups.

The project simulates a real-world bioinformatics cloud system where the Frontend (User) is decoupled from the Backend (Server) and Storage (Disk).

## 🛠️ Tech Stack
*   **Frontend (Tier 1):** React.js (Dark Mode GUI)
*   **Backend (Tier 2):** C++ (Custom Multi-threaded HTTP Server)
*   **Database (Tier 3):** Custom Binary File Storage (No SQL/NoSQL used)
*   **Networking:** REST API (JSON over HTTP)

## ⚡ Key Features
1.  **🧬 B-Tree Indexing (100%):**
    *   Used to index **Gene IDs**.
    *   Ensures `O(log n)` search time even for millions of records.
    *   Implements **Disk Manipulation** (reading/writing nodes to binary files).

2.  **⚡ Hashing (40%):**
    *   Used to index **Gene Names**.
    *   Provides `O(1)` average time complexity for searching genes by name.

3.  **☁️ Client-Server Architecture:**
    *   Decoupled architecture where the C++ backend runs independently on a server port (8080).