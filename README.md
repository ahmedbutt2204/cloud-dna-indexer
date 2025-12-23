# 🧬 Cloud-Based DNA Sequence Indexing System

![C++](https://img.shields.io/badge/Backend-C++17-blue?style=for-the-badge&logo=c%2B%2B)
![React](https://img.shields.io/badge/Frontend-React.js-61DAFB?style=for-the-badge&logo=react)
![Architecture](https://img.shields.io/badge/Architecture-3--Tier-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

> **A High-Performance Distributed System for Genomic Data Analysis and Storage.**

---

## 📌 Project Overview
This project is a **3-Tier Full Stack Application** designed to store, index, and retrieve massive amounts of DNA records efficiently. In the field of Bioinformatics, genetic datasets are often too voluminous to reside entirely in standard memory (RAM).

To solve this, we implemented a **Custom Disk-Based Database Engine** from scratch in C++. It utilizes **B-Trees** for persistent disk storage and efficient retrieval, and **Hash Tables** for instant memory-mapped lookups. The system simulates a real-world cloud architecture where the Frontend (User) is decoupled from the Backend (Server) and Storage (Disk).

---

## ✨ Key Features

### 1. 🔍 Advanced Query Engine
*   **B-Tree Indexing (Degree 3):** Implements a self-balancing tree structure for `O(log n)` retrieval of Gene IDs directly from the disk.
*   **Hash Mapping:** Uses a custom chaining Hash Table for `O(1)` instant lookup by Gene Name.
*   **Range Analytics:** Intelligently traverses tree nodes to fetch specific clusters of genes (e.g., *Find all genes between ID 100 and 500*).

### 2. 🧬 Mutation Analysis (Bioinformatics)
*   **Sequence Alignment:** Built-in algorithm to compare patient DNA samples against stored reference genes.
*   **Risk Diagnosis:** Calculates mutation percentage (Hamming Distance) and determines health risk levels (Healthy vs. Mutation Detected).

### 3. 🛡️ System Integrity & Security
*   **Duplicate Protection:** Prevents data corruption by checking the B-Tree before insertion.
*   **Input Sanitization:** Enforces strict DNA syntax validation (only A, T, C, G characters allowed).
*   **Disk Persistence:** Data survives server restarts via binary file manipulation (`genes.dat`).

---

## 🛠️ Technical Architecture (3-Tier)

The system strictly follows the **Client-Server-Storage** distributed model:

| Tier | Component | Technology | Description |
| :--- | :--- | :--- | :--- |
| **Tier 1** | **Presentation** | React.js | Professional Medical Dashboard with Dark/Light modes and responsive Grid layout. |
| **Tier 2** | **Application** | C++ (Native) | Multi-threaded Custom HTTP Server using raw Sockets (Winsock/Linux). Handles API logic. |
| **Tier 3** | **Data** | Binary Files | Custom `genes.dat` database manipulated via `fstream` (Seek/Tell) for block storage. |

---

## 🔌 API Reference (Internal)

The Frontend communicates with the C++ Backend via **JSON over HTTP**.

| Method | Endpoint | Params | Description |
| :--- | :--- | :--- | :--- |
| `POST` | `/add` | `id`, `name`, `sequence` | Index a new gene record to B-Tree and Disk. |
| `GET` | `/search` | `id` OR `name` | Find a specific record. |
| `GET` | `/range` | `min`, `max` | Fetch a list of genes within a numeric ID range. |
| `POST` | `/analyze` | `id`, `sequence` | Run mutation detection algorithm against a reference gene. |
| `GET` | `/health` | *None* | System heartbeat check. |

---

## 🚀 Installation & Setup

### Prerequisites
*   **C++ Compiler:** MinGW (Windows) or G++ (Linux/Mac)
*   **Node.js:** v16+ (For the Frontend)

### Step 1: Start the Backend (The Brain)
Open a terminal in the `backend/` folder:
```bash
cd backend
g++ main.cpp -o server -lws2_32   # Windows Command
# OR: g++ main.cpp -o server      # Linux/Mac Command

./server