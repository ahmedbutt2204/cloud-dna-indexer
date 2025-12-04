# 🧬 Cloud-Based DNA Sequence Indexing System

### A Distributed Genome Search Engine using Advanced Data Structures

## 📌 Project Overview
This project is a **3-Tier Full Stack Application** designed to store, index, and retrieve massive amounts of DNA records. Since genetic data is too large to fit into standard memory (RAM), this system implements a **custom file-based database engine** using **B-Trees** for disk storage and **Hashing** for instant lookups.

The project simulates a real-world bioinformatics cloud system where the Frontend (User) is decoupled from the Backend (Server) and Storage (Disk).

## 🎯 Problem Statement
*   **Big Data:** DNA sequences are massive. Storing them in arrays or linked lists causes Memory Overflow.
*   **Search Latency:** Finding a specific gene in a billion records takes too long with linear search.
*   **Solution:** We use **B-Trees** (Order 3) to minimize disk reads and **Hashing** for O(1) name lookups.