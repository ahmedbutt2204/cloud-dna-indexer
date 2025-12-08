# 🧬 Cloud-Based DNA Sequence Indexing System

![C++](https://img.shields.io/badge/Backend-C++-blue?style=for-the-badge&logo=c%2B%2B)
![React](https://img.shields.io/badge/Frontend-React.js-61DAFB?style=for-the-badge&logo=react)
![Architecture](https://img.shields.io/badge/Architecture-3--Tier-orange?style=for-the-badge)

### 🚀 A Distributed Bioinformatics Search Engine

## 📌 Project Overview
This project is a **High-Performance Distributed System** designed to index and analyze human genome sequencing data. Unlike traditional SQL databases, this system utilizes a **custom-built C++ Database Engine** powered by **B-Trees** and **Hash Tables** to achieve milliseconds retrieval time for massive DNA records.

The system features a decoupled **3-Tier Architecture**, separating the React GUI from the C++ Logic and Binary Disk Storage.

## ✨ Key Features

### 1. 🔍 Advanced Query Engine
*   **B-Tree Indexing:** Implements Order-3 B-Trees for `O(log n)` retrieval of Gene IDs.
*   **Hash Mapping:** Uses a custom Hash Table for `O(1)` instant lookup by Gene Name.
*   **Range Search:** intelligently traverses tree nodes to find genes within a specific ID range (e.g., 100-500).

### 2. 🧬 Mutation Analysis (Bioinformatics)
*   **Sequence Alignment:** Compares patient DNA with stored records.
*   **Diagnosis Algorithm:** Calculates mutation percentage and risk levels (Healthy vs. Mutation Detected).

### 3. 🛡️ System Integrity
*   **Duplicate Protection:** Prevents data corruption by validating IDs before insertion.
*   **Input Sanitization:** Ensures only valid DNA characters (A, T, C, G) are stored.

## 🛠️ Technical Architecture (3-Tier)

This project strictly follows the **Client-Server-Storage** model:

| Tier | Component | Technology | Description |
| :--- | :--- | :--- | :--- |
| **Tier 1** | **Presentation** | React.js (SaaS UI) | Provides a responsive Medical Dashboard for doctors/researchers. |
| **Tier 2** | **Application** | C++ (Native Server) | Multi-threaded Socket Server running on Port 8080. Handles logic & algorithms. |
| **Tier 3** | **Data** | Binary Files | Custom `genes.dat` file manipulated via `fstream` (Seek/Tell). |

### 🔌 API Endpoints
The Frontend communicates with the C++ Backend via JSON over HTTP:
*   `POST /add` - Index a new gene record.
*   `GET /search` - Find record by ID (B-Tree) or Name (Hash).
*   `GET /range` - Fetch multiple records via Tree Traversal.
*   `POST /analyze` - Run Mutation detection algorithm.

## 🚀 How to Run locally

### Prerequisites
*   **C++ Compiler:** MinGW (Windows) or G++ (Linux/Mac).
*   **Node.js:** v16+ (For the Frontend).

### Step 1: Start the Backend (The Brain)
Open a terminal in the `backend/` folder:
```bash
cd backend
g++ main.cpp -o server -lws2_32  # For Windows
./server

Server will start listening on Port 8080.
Step 2: Start the Frontend (The Interface)
Open a new terminal in the frontend/ folder:
code
Bash
cd frontend
npm start
The Dashboard will launch automatically at http://localhost:3000.
code
Code
**👉 Terminal Commands:**
```powershell

## 📂 Project Structure

```text
cloud-dna-indexer/
├── 📂 backend/              # C++ Server Logic
│   ├── main.cpp             # Server Entry Point & API Handler
│   ├── BTree.h              # Core B-Tree Implementation
│   ├── HashTable.h          # Hashing Logic
│   ├── Gene.h               # Data Structure Definition
│   └── genes.dat            # Binary Database File
│
├── 📂 frontend/             # React.js GUI
│   ├── src/
│   │   ├── App.js           # Dashboard Logic
│   │   └── App.css          # Enterprise Styling
│   └── package.json
│
├── LICENSE                  # MIT License
└── README.md                # Project Documentation
