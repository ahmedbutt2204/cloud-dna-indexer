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