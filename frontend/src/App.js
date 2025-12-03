// FILE: frontend/src/App.js
import React, { useState } from 'react';
import './App.css';

function App() {
  // State variables to hold the form data
  const [id, setId] = useState('');
  const [name, setName] = useState('');
  const [sequence, setSequence] = useState('');
  const [searchId, setSearchId] = useState('');
  const [searchName, setSearchName] = useState('');
  const [message, setMessage] = useState('System Ready...');

  // 1. Function to Handle "Add Gene"
  const handleAdd = async () => {
    setMessage("Processing Add Request...");
    // TODO: Connect to C++ Backend here
    console.log("Adding:", id, name, sequence);
    
    // Simulate success for now
    setTimeout(() => setMessage(`Success: Added Gene ${name} (ID: ${id})`), 500);
  };

  // 2. Function to Handle "Search by ID"
  const handleSearchById = async () => {
    setMessage(`Searching B-Tree for ID: ${searchId}...`);
    // TODO: Connect to C++ Backend here
  };

  // 3. Function to Handle "Search by Name"
  const handleSearchByName = async () => {
    setMessage(`Searching Hash Table for Name: ${searchName}...`);
    // TODO: Connect to C++ Backend here
  };

  return (
    <div className="container">
      <h1>🧬 Cloud DNA Indexer</h1>
      
      {/* SECTION 1: ADD GENE */}
      <div className="card">
        <h2>➕ Add New Gene Record</h2>
        <input 
          type="number" 
          placeholder="Gene ID (e.g., 101)" 
          value={id} onChange={(e) => setId(e.target.value)} 
        />
        <input 
          type="text" 
          placeholder="Gene Name (e.g., BRCA1)" 
          value={name} onChange={(e) => setName(e.target.value)} 
        />
        <input 
          type="text" 
          placeholder="DNA Sequence (e.g., ATCG-GGTA...)" 
          value={sequence} onChange={(e) => setSequence(e.target.value)} 
        />
        <button onClick={handleAdd}>Save to Cloud Database</button>
      </div>

      {/* SECTION 2: SEARCH */}
      <div className="card">
        <h2>🔍 Search Database</h2>
        
        {/* Search by ID */}
        <div style={{marginBottom: '20px'}}>
          <label>Search by ID (B-Tree):</label>
          <input 
            type="number" 
            placeholder="Enter ID..." 
            value={searchId} onChange={(e) => setSearchId(e.target.value)} 
          />
          <button onClick={handleSearchById}>Search ID</button>
        </div>

        {/* Search by Name */}
        <div>
          <label>Search by Name (Hashing):</label>
          <input 
            type="text" 
            placeholder="Enter Name..." 
            value={searchName} onChange={(e) => setSearchName(e.target.value)} 
          />
          <button onClick={handleSearchByName} style={{backgroundColor: '#ff9800'}}>Search Name</button>
        </div>
      </div>

      {/* STATUS LOG */}
      <div className="result-box">
        <strong> System Log:</strong><br/>
        {message}
      </div>

    </div>
  );
}

export default App;