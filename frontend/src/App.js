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
  const [message, setMessage] = useState('System ready. Waiting for input...');

  // --- LOGIC FUNCTIONS ---
  const handleAdd = async () => {
    if(!id || !name || !sequence) { setMessage("Error: Please fill all fields"); return; }
    setMessage("Processing... Sending data to Cloud Backend.");
    try {
      const res = await fetch(`http://localhost:8080/add?id=${id}&name=${name}&sequence=${sequence}`, { method: 'POST' });
      const data = await res.json();
      if(data.status === "success") {
        setMessage(`✅ Success! Gene '${name}' (ID: ${id}) has been indexed in the B-Tree.`);
        setId(''); setName(''); setSequence('');
      } else if(data.message === "Duplicate ID") {
        setMessage("⚠️ Error: Duplicate Gene ID. Please use a unique ID.");
      } else { setMessage("❌ Error: Server rejected data."); }
    } catch (error) { setMessage("❌ Error: Backend connection failed. Is the C++ server running?"); }
  };

  const handleSearchById = async () => {
    if(!searchId) return;
    try {
      const res = await fetch(`http://localhost:8080/search?id=${searchId}`);
      const data = await res.json();
      if (data.found) setMessage(`✅ FOUND (via B-Tree Search):\nID: ${searchId}\nName: ${data.name}\nSeq: ${data.sequence}`);
      else setMessage(`⚠️ ID ${searchId} not found in database.`);
    } catch (error) { setMessage("❌ Connection Failed."); }
  };

  const handleSearchByName = async () => {
    if(!searchName) return;
    try {
      const res = await fetch(`http://localhost:8080/search?name=${searchName}`);
      const data = await res.json();
      if (data.found) setMessage(`✅ FOUND (via Hashing O(1)):\nName: ${searchName}\nID: ${data.id}\nSeq: ${data.sequence}`);
      else setMessage(`⚠️ Name '${searchName}' not found.`);
    } catch (error) { setMessage("❌ Connection Failed."); }
  };

  const handleRangeSearch = async () => {
    if(!minRange || !maxRange) return;
    setMessage(`🔍 Executing B-Tree Range Query [${minRange} - ${maxRange}]...`);
    try {
        const res = await fetch(`http://localhost:8080/range?min=${minRange}&max=${maxRange}`);
        const data = await res.json();
        if(data.found && data.results.length > 0) {
            let resultText = `✅ Found ${data.count} Genes in Range:\n`;
            data.results.forEach(g => { resultText += `• ${g.name} (ID: ${g.id})\n`; });
            setMessage(resultText);
        } else { setMessage("⚠️ No genes found in this range."); }
    } catch(error) { setMessage("❌ Connection Failed."); }
  };

  // --- DEMO BUTTON ---
  const loadDemoData = async () => {
    setMessage("🚀 Loading Demo Data...");
    const demoGenes = [
        {id: 101, name: "BRCA1", seq: "ATCG-CANCER"},
        {id: 205, name: "INSULIN", seq: "GGG-AAA-SUGAR"},
        {id: 300, name: "MYOGLOBIN", seq: "MUSCLE-OXY"},
        {id: 404, name: "ERROR-GENE", seq: "NOT-FOUND-404"},
        {id: 555, name: "TP53", seq: "GUARDIAN-CELL"}
    ];
    for (const g of demoGenes) {
        await fetch(`http://localhost:8080/add?id=${g.id}&name=${g.name}&sequence=${g.seq}`, { method: 'POST' });
    }
    setMessage("✅ Demo Data Loaded! Try searching Range 100-600.");
  };

  return (
    <div className="dashboard-layout">
      {/* SIDEBAR */}
      <div className="sidebar">
        <div className="brand">🧬 DNA Indexer</div>
        <div className="menu-item active">Dashboard</div>
        <div className="menu-item">Analytics</div>
        <div className="menu-item">Settings</div>
        <div style={{marginTop: 'auto', marginBottom: '10px'}}>
             <button onClick={loadDemoData} style={{background: 'transparent', border: '1px solid #a3aed0', color: '#a3aed0', padding: '5px 10px', cursor: 'pointer', borderRadius: '5px', fontSize: '11px', width: '100%'}}>
               ⚡ Load Demo Data
             </button>
        </div>
        <div style={{color: '#a3aed0', fontSize: '12px'}}>v1.0.0 (Pro)</div>
      </div>

      {/* MAIN CONTENT */}
      <div className="main-content">
        <div className="header">
          <h2>Genome Dashboard</h2>
          <p>Manage and query distributed DNA records via C++ B-Tree Engine.</p>
        </div>

        <div className="grid-container">
          {/* CARD 1: ADD */}
          <div className="card">
            <h3>➕ Ingest Data</h3>
            <div className="form-group">
                <label>Gene ID</label>
                <input type="number" placeholder="e.g. 101" value={id} onChange={(e) => setId(e.target.value)} />
            </div>
            <div className="form-group">
                <label>Gene Name</label>
                <input type="text" placeholder="e.g. BRCA1" value={name} onChange={(e) => setName(e.target.value)} />
            </div>
            <div className="form-group">
                <label>DNA Sequence</label>
                <input type="text" placeholder="e.g. ATCG-GCTA" value={sequence} onChange={(e) => setSequence(e.target.value)} />
            </div>
            <button className="btn-primary" onClick={handleAdd}>Index Record</button>
          </div>

          {/* CARD 2: SEARCH */}
          <div className="card">
            <h3>🔍 Query Engine</h3>
            <div className="form-group">
                <label>Search by ID (B-Tree)</label>
                <div style={{display:'flex', gap:'10px'}}>
                    <input type="number" placeholder="Enter ID" value={searchId} onChange={(e) => setSearchId(e.target.value)} />
                    <button className="btn-secondary" onClick={handleSearchById}>Go</button>
                </div>
            </div>
            <div className="form-group">
                <label>Search by Name (Hashing)</label>
                <div style={{display:'flex', gap:'10px'}}>
                    <input type="text" placeholder="Enter Name" value={searchName} onChange={(e) => setSearchName(e.target.value)} />
                    <button className="btn-secondary" onClick={handleSearchByName}>Go</button>
                </div>
            </div>
          </div>

          {/* CARD 3: RANGE */}
          <div className="card">
             <h3>📊 Advanced Analytics</h3>
             <label>Range Query (B-Tree Traversal)</label>
             <div className="range-group">
                <input type="number" placeholder="Min ID" value={minRange} onChange={(e) => setMinRange(e.target.value)} />
                <input type="number" placeholder="Max ID" value={maxRange} onChange={(e) => setMaxRange(e.target.value)} />
             </div>
             <button className="btn-primary" style={{marginTop:'15px', background:'#2b3674'}} onClick={handleRangeSearch}>Run Analysis</button>
          </div>
        </div>

        <div className="log-area">
            <span className="log-label">System Output Stream</span>
            {message}
        </div>
      </div>
    </div>
  );
}

export default App;