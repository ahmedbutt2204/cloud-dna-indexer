import React, { useState } from 'react';
import './App.css';

function App() {
  const [id, setId] = useState('');
  const [name, setName] = useState('');
  const [sequence, setSequence] = useState('');
  const [searchId, setSearchId] = useState('');
  const [searchName, setSearchName] = useState('');
  const [minRange, setMinRange] = useState('');
  const [maxRange, setMaxRange] = useState('');
  const [message, setMessage] = useState('System Ready...');

  const handleAdd = async () => {
    if(!id || !name || !sequence) { setMessage("Error: Please fill all fields"); return; }
    setMessage("Sending data to Cloud Backend...");
    try {
      const res = await fetch(`http://localhost:8080/add?id=${id}&name=${name}&sequence=${sequence}`, { method: 'POST' });
      const data = await res.json();
      if(data.status === "success") {
        setMessage(`✅ Success! Gene '${name}' (ID: ${id}) saved.`);
        setId(''); setName(''); setSequence('');
      } else { setMessage("❌ Error: Server rejected data."); }
    } catch (error) { setMessage("❌ Error: Backend not running."); }
  };

  const handleSearchById = async () => {
    if(!searchId) return;
    try {
      const res = await fetch(`http://localhost:8080/search?id=${searchId}`);
      const data = await res.json();
      if (data.found) setMessage(`✅ FOUND (B-Tree):\nID: ${searchId}\nName: ${data.name}\nSeq: ${data.sequence}`);
      else setMessage(`⚠️ ID ${searchId} not found.`);
    } catch (error) { setMessage("❌ Connection Failed."); }
  };

  const handleSearchByName = async () => {
    if(!searchName) return;
    try {
      const res = await fetch(`http://localhost:8080/search?name=${searchName}`);
      const data = await res.json();
      if (data.found) setMessage(`✅ FOUND (Hashing):\nName: ${searchName}\nID: ${data.id}\nSeq: ${data.sequence}`);
      else setMessage(`⚠️ Name '${searchName}' not found.`);
    } catch (error) { setMessage("❌ Connection Failed."); }
  };

  // --- NEW: RANGE SEARCH ---
  const handleRangeSearch = async () => {
    if(!minRange || !maxRange) return;
    setMessage(`🔍 Searching B-Tree Range [${minRange} - ${maxRange}]...`);
    try {
        const res = await fetch(`http://localhost:8080/range?min=${minRange}&max=${maxRange}`);
        const data = await res.json();
        
        if(data.found && data.results.length > 0) {
            let resultText = `✅ Found ${data.count} Genes in Range:\n`;
            data.results.forEach(g => {
                resultText += `• [ID: ${g.id}] ${g.name}\n`;
            });
            setMessage(resultText);
        } else {
            setMessage("⚠️ No genes found in this range.");
        }
    } catch(error) { setMessage("❌ Connection Failed."); }
  };

  return (
    <div className="container">
      <h1>🧬 Cloud DNA Indexer (Advanced)</h1>
      
      <div className="card">
        <h2>➕ Add New Gene</h2>
        <input type="number" placeholder="ID (e.g., 101)" value={id} onChange={(e) => setId(e.target.value)} />
        <input type="text" placeholder="Name (e.g., BRCA1)" value={name} onChange={(e) => setName(e.target.value)} />
        <input type="text" placeholder="Sequence (e.g., ATCG)" value={sequence} onChange={(e) => setSequence(e.target.value)} />
        <button onClick={handleAdd}>Save</button>
      </div>

      <div className="card">
        <h2>🔍 Search Options</h2>
        
        <div style={{marginBottom: '15px'}}>
            <label>1. Exact ID (B-Tree):</label>
            <input type="number" placeholder="Enter ID..." value={searchId} onChange={(e) => setSearchId(e.target.value)} />
            <button onClick={handleSearchById}>Find ID</button>
        </div>

        <div style={{marginBottom: '15px'}}>
            <label>2. Exact Name (Hashing):</label>
            <input type="text" placeholder="Enter Name..." value={searchName} onChange={(e) => setSearchName(e.target.value)} />
            <button style={{backgroundColor: '#ff9800'}} onClick={handleSearchByName}>Find Name</button>
        </div>

        <div>
            <label>3. Range Query (B-Tree Power):</label>
            <div style={{display:'flex', gap:'10px'}}>
                <input type="number" placeholder="Min ID" value={minRange} onChange={(e) => setMinRange(e.target.value)} />
                <input type="number" placeholder="Max ID" value={maxRange} onChange={(e) => setMaxRange(e.target.value)} />
            </div>
            <button style={{backgroundColor: '#9c27b0'}} onClick={handleRangeSearch}>Search Range</button>
        </div>
      </div>

      <div className="result-box">
        <strong>> System Log:</strong><br/>
        <pre>{message}</pre>
      </div>
    </div>
  );
}

export default App;