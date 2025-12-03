import React, { useState } from 'react';
import './App.css';

function App() {
  const [id, setId] = useState('');
  const [name, setName] = useState('');
  const [sequence, setSequence] = useState('');
  const [searchId, setSearchId] = useState('');
  const [searchName, setSearchName] = useState('');
  const [message, setMessage] = useState('System Ready... Waiting for Server');

  // 1. ADD GENE (Connects to C++ POST /add)
  const handleAdd = async () => {
    if(!id || !name || !sequence) { setMessage("Error: Please fill all fields"); return; }
    setMessage("Sending data to Cloud Backend...");
    
    try {
      const res = await fetch(`http://localhost:8080/add?id=${id}&name=${name}&sequence=${sequence}`, {
        method: 'POST'
      });
      
      const data = await res.json();
      if(data.status === "success") {
        setMessage(`✅ Success! Gene '${name}' (ID: ${id}) saved to B-Tree on Disk.`);
        setId(''); setName(''); setSequence('');
      } else {
        setMessage("❌ Error: Server rejected the data.");
      }
    } catch (error) {
      setMessage("❌ Error: Could not connect to C++ Backend. Is it running?");
    }
  };

  // 2. SEARCH BY ID (Connects to C++ GET /search?id=)
  const handleSearchById = async () => {
    if(!searchId) return;
    setMessage(`🔍 Querying B-Tree for ID: ${searchId}...`);
    
    try {
      const res = await fetch(`http://localhost:8080/search?id=${searchId}`);
      const data = await res.json();
      
      if (data.found) {
        setMessage(`✅ FOUND IN DATABASE!\n\nGene Name: ${data.name}\nGene ID: ${searchId}\nSequence: ${data.sequence}`);
      } else {
        setMessage(`⚠️ Not Found: ID ${searchId} is not in the B-Tree index.`);
      }
    } catch (error) {
      setMessage("❌ Connection Failed. Check Console.");
    }
  };

  // 3. SEARCH BY NAME (Connects to C++ GET /search?name=)
  const handleSearchByName = async () => {
    if(!searchName) return;
    setMessage(`🔍 Hashing Name '${searchName}' to find ID...`);
    
    try {
      const res = await fetch(`http://localhost:8080/search?name=${searchName}`);
      const data = await res.json();
      
      if (data.found) {
        setMessage(`✅ FOUND VIA HASHING!\n\nGene Name: ${searchName}\nMapped to ID: ${data.id}\nSequence: ${data.sequence}`);
      } else {
        setMessage(`⚠️ Not Found: Name '${searchName}' is not in the Hash Table.`);
      }
    } catch (error) {
      setMessage("❌ Connection Failed.");
    }
  };

  return (
    <div className="container">
      <h1>🧬 Cloud DNA Indexer (3-Tier)</h1>
      
      {/* ADD SECTION */}
      <div className="card">
        <h2>➕ Add New Gene Record</h2>
        <input type="number" placeholder="Gene ID (e.g., 101)" value={id} onChange={(e) => setId(e.target.value)} />
        <input type="text" placeholder="Gene Name (e.g., BRCA1)" value={name} onChange={(e) => setName(e.target.value)} />
        <input type="text" placeholder="DNA Sequence (e.g., ATCG...)" value={sequence} onChange={(e) => setSequence(e.target.value)} />
        <button onClick={handleAdd}>Save to Cloud Database</button>
      </div>

      {/* SEARCH SECTION */}
      <div className="card">
        <h2>🔍 Search Database</h2>
        
        <div style={{marginBottom: '20px'}}>
          <label>Search by ID (B-Tree):</label>
          <input type="number" placeholder="Enter ID..." value={searchId} onChange={(e) => setSearchId(e.target.value)} />
          <button onClick={handleSearchById}>Search ID</button>
        </div>

        <div>
          <label>Search by Name (Hashing):</label>
          <input type="text" placeholder="Enter Name..." value={searchName} onChange={(e) => setSearchName(e.target.value)} />
          <button onClick={handleSearchByName} style={{backgroundColor: '#ff9800'}}>Search Name</button>
        </div>
      </div>

      {/* RESULT DISPLAY */}
      <div className="result-box">
        <strong> System Log:</strong><br/>
        <pre>{message}</pre>
      </div>
    </div>
  );
}

export default App;