#ifndef HTML_H
#define HTML_H

const char* HTML_PAGE = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>NEXUS HOME</title>
<link href="https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@300;400;500;600;700&family=JetBrains+Mono:wght@400;600&display=swap" rel="stylesheet">
<link rel="stylesheet" href="/style.css">
</head>
<body>

<div class="orb orb1"></div>
<div class="orb orb2"></div>
<div class="orb orb3"></div>

<header>
  <div class="header-left">
    <div class="logo-icon">
      <svg width="36" height="36" viewBox="0 0 36 36" fill="none">
        <path d="M18 3L33 12V24L18 33L3 24V12L18 3Z" stroke="currentColor" stroke-width="1.5" fill="none"/>
        <path d="M18 9L27 14.5V25.5L18 31L9 25.5V14.5L18 9Z" fill="currentColor" opacity="0.15"/>
        <circle cx="18" cy="18" r="4" fill="currentColor"/>
      </svg>
    </div>
    <div>
      <div class="logo-name">NEXUS HOME</div>
      <div class="logo-ver">Control System v2.0</div>
    </div>
  </div>
  <div class="header-right">
    <div class="status-pill">
      <span class="pulse-dot"></span>
      <span>Live</span>
    </div>
    <div class="clock-box">
      <div class="clock" id="clock">--:--:--</div>
      <div class="clock-label">LOCAL TIME</div>
    </div>
  </div>
</header>

<nav class="tab-nav">
  <button class="tab-btn active" onclick="switchTab('sensors', this)">
    <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"/><path d="M12 1v4M12 19v4M4.22 4.22l2.83 2.83M16.95 16.95l2.83 2.83M1 12h4M19 12h4M4.22 19.78l2.83-2.83M16.95 7.05l2.83-2.83"/></svg>
    Sensors
  </button>
  <button class="tab-btn" onclick="switchTab('control', this)">
    <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="2" y="3" width="20" height="14" rx="2"/><path d="M8 21h8M12 17v4"/></svg>
    Controls
  </button>
  <button class="tab-btn" onclick="switchTab('camera', this)">
    <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M23 19a2 2 0 0 1-2 2H3a2 2 0 0 1-2-2V8a2 2 0 0 1 2-2h4l2-3h6l2 3h4a2 2 0 0 1 2 2z"/><circle cx="12" cy="13" r="4"/></svg>
    Camera
  </button>
</nav>

<!-- ══ SENSORS ══ -->
<div id="tab-sensors" class="tab-panel active">
  <div class="sensor-grid">

    <!-- GAS -->
    <div class="card" id="gas-card">
      <div class="card-top">
        <div class="card-icon gas-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2z"/><path d="M12 8v4l3 3"/></svg>
        </div>
        <div class="card-label">Gas Level</div>
        <div class="chip chip-ok" id="gas-badge">Normal</div>
      </div>
      <div class="gauge-wrap">
        <svg class="gauge" viewBox="0 0 200 130">
          <path d="M20 110 A80 80 0 0 1 180 110" fill="none" stroke="rgba(255,255,255,0.06)" stroke-width="14" stroke-linecap="round"/>
          <path id="gas-arc" d="M20 110 A80 80 0 0 1 180 110" fill="none" stroke="url(#gGrad)" stroke-width="14" stroke-linecap="round"
            stroke-dasharray="251" stroke-dashoffset="251" style="transition:stroke-dashoffset 0.6s ease"/>
          <defs>
            <linearGradient id="gGrad" x1="0%" y1="0%" x2="100%" y2="0%">
              <stop offset="0%" stop-color="#4ade80"/>
              <stop offset="60%" stop-color="#facc15"/>
              <stop offset="100%" stop-color="#f87171"/>
            </linearGradient>
          </defs>
          <text x="100" y="98" text-anchor="middle" class="gauge-val" id="gas-val">0</text>
          <text x="100" y="116" text-anchor="middle" class="gauge-unit">RAW VALUE</text>
        </svg>
      </div>
      <div class="bar-row">
        <span class="bar-lbl">Concentration</span>
        <div class="prog-bar"><div class="prog-fill green-fill" id="gas-bar"></div></div>
      </div>
    </div>

    <!-- FLAME -->
    <div class="card" id="flame-card">
      <div class="card-top">
        <div class="card-icon flame-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 2C8 7 8 10 12 12C8 12 6 15 8 19C9.5 21 11 22 12 22C13 22 14.5 21 16 19C18 15 16 12 12 12C16 10 16 7 12 2Z"/></svg>
        </div>
        <div class="card-label">Flame Detect</div>
        <div class="chip chip-ok" id="flame-badge">Clear</div>
      </div>
      <div class="orb-display">
        <div class="sensor-orb flame-orb" id="flame-orb">
          <span class="orb-main" id="flame-pct">0%</span>
          <span class="orb-sub">Intensity</span>
        </div>
      </div>
      <div class="bar-row">
        <span class="bar-lbl">Intensity Level</span>
        <div class="prog-bar"><div class="prog-fill orange-fill" id="flame-bar"></div></div>
      </div>
    </div>

    <!-- RAIN -->
    <div class="card" id="rain-card">
      <div class="card-top">
        <div class="card-icon rain-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M20 16.2A5 5 0 0 0 18 7h-1.26A8 8 0 1 0 4 15.25"/><line x1="8" y1="19" x2="8" y2="21"/><line x1="8" y1="13" x2="8" y2="15"/><line x1="16" y1="19" x2="16" y2="21"/><line x1="16" y1="13" x2="16" y2="15"/><line x1="12" y1="21" x2="12" y2="23"/><line x1="12" y1="15" x2="12" y2="17"/></svg>
        </div>
        <div class="card-label">Rain Sensor</div>
        <div class="chip chip-ok" id="rain-badge">Dry</div>
      </div>
      <div class="rain-visual" id="rain-visual">
        <div class="rain-col"><div class="rdrop"></div><div class="rdrop"></div><div class="rdrop"></div></div>
        <div class="rain-col delay1"><div class="rdrop"></div><div class="rdrop"></div><div class="rdrop"></div></div>
        <div class="rain-col delay2"><div class="rdrop"></div><div class="rdrop"></div><div class="rdrop"></div></div>
        <div class="rain-big" id="rain-pct">0%</div>
      </div>
      <div class="bar-row">
        <span class="bar-lbl">Moisture Level</span>
        <div class="prog-bar"><div class="prog-fill blue-fill" id="rain-bar"></div></div>
      </div>
    </div>

   <!-- SOUND SENSOR  -->
<div class="card" id="sound-card">
  <div class="card-top">
    <div class="card-icon sound-icon">
      <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
        <path d="M2 12a10 10 0 0 1 20 0"/>
        <path d="M6 12a6 6 0 0 1 12 0"/>
        <line x1="12" y1="2" x2="12" y2="22"/>
        <path d="M8 12a4 4 0 0 1 8 0"/>
      </svg>
    </div>
    <div class="card-label">Sound at the window</div>
    <div class="chip chip-ok" id="sound-badge">Clear</div>
  </div>
  <div class="orb-display">
    <div class="sensor-orb sound-orb" id="sound-orb">
      <div class="sound-wave w1"></div>
      <div class="sound-wave w2"></div>
      <div class="sound-wave w3"></div>
      <span class="orb-main" id="sound-status-text">Clear</span>
      <span class="orb-sub">Status</span>
    </div>
  </div>
  <p class="card-foot" id="sound-message">No sound detected at window</p>
</div>

    <!-- MOTION - DOOR -->
    <div class="card" id="motion-door-card">
      <div class="card-top">
        <div class="card-icon motion-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"/><circle cx="12" cy="12" r="3"/></svg>
        </div>
        <div class="card-label">Motion - Door</div>
        <div class="chip chip-ok" id="motion-door-badge">Clear</div>
      </div>
      <div class="orb-display">
        <div class="sensor-orb motion-orb" id="motion-door-orb">
          <div class="motion-ripple r1"></div>
          <div class="motion-ripple r2"></div>
          <div class="motion-ripple r3"></div>
          <span class="orb-main" id="motion-door-state">CLEAR</span>
          <span class="orb-sub">PIR Door</span>
        </div>
      </div>
      <p class="card-foot" id="motion-door-status">Monitoring door area...</p>
    </div>

    <!-- MOTION - WINDOW -->
    <div class="card" id="motion-window-card">
      <div class="card-top">
        <div class="card-icon motion-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z"/><circle cx="12" cy="12" r="3"/></svg>
        </div>
        <div class="card-label">Motion - Window</div>
        <div class="chip chip-ok" id="motion-window-badge">Clear</div>
      </div>
      <div class="orb-display">
        <div class="sensor-orb motion-orb" id="motion-window-orb">
          <div class="motion-ripple r1"></div>
          <div class="motion-ripple r2"></div>
          <div class="motion-ripple r3"></div>
          <span class="orb-main" id="motion-window-state">CLEAR</span>
          <span class="orb-sub">PIR Window</span>
        </div>
      </div>
      <p class="card-foot" id="motion-window-status">Monitoring window area...</p>
    </div>

    <!-- CLIMATE (wide) -->
    <div class="card card-wide">
      <div class="card-top">
        <div class="card-icon climate-icon">
          <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M14 14.76V3.5a2.5 2.5 0 0 0-5 0v11.26a4.5 4.5 0 1 0 5 0z"/></svg>
        </div>
        <div class="card-label">Climate Monitor</div>
        <div class="chip chip-ok" id="dht-badge">Reading</div>
      </div>
      <div class="dht-row">
        <div class="dht-block">
          <div class="dht-ring-wrap">
            <svg viewBox="0 0 100 100" class="dht-ring-svg">
              <circle cx="50" cy="50" r="38" fill="none" stroke="rgba(255,100,50,0.12)" stroke-width="7"/>
              <circle id="temp-arc" cx="50" cy="50" r="38" fill="none" stroke="#fb923c"
                stroke-width="7" stroke-linecap="round"
                transform="rotate(-90 50 50)" stroke-dasharray="239" stroke-dashoffset="239"
                style="transition:stroke-dashoffset 0.6s ease"/>
            </svg>
            <div class="dht-center">
              <span class="dht-num" id="dht-temp">--</span>
              <span class="dht-u">°C</span>
            </div>
          </div>
          <div class="dht-name">Temperature</div>
        </div>
        <div class="dht-sep"></div>
        <div class="dht-block">
          <div class="dht-ring-wrap">
            <svg viewBox="0 0 100 100" class="dht-ring-svg">
              <circle cx="50" cy="50" r="38" fill="none" stroke="rgba(56,189,248,0.12)" stroke-width="7"/>
              <circle id="hum-arc" cx="50" cy="50" r="38" fill="none" stroke="#38bdf8"
                stroke-width="7" stroke-linecap="round"
                transform="rotate(-90 50 50)" stroke-dasharray="239" stroke-dashoffset="239"
                style="transition:stroke-dashoffset 0.6s ease"/>
            </svg>
            <div class="dht-center">
              <span class="dht-num" id="dht-hum">--</span>
              <span class="dht-u">%</span>
            </div>
          </div>
          <div class="dht-name">Humidity</div>
        </div>
      </div>
      <p class="card-foot" id="dht-status">Initializing sensors...</p>
    </div>

  </div>
</div>

<!-- ══ CONTROLS ══ -->
<div id="tab-control" class="tab-panel">
  <div class="ctrl-grid">

    <div class="ctrl-card">
      <div class="ctrl-top">
        <div class="ctrl-icon led-ci">💡</div>
        <div class="ctrl-meta">
          <div class="ctrl-name">LED Light</div>
          <div class="ctrl-state" id="led-state">OFF</div>
        </div>
        <div class="led-indicator" id="led-ind"></div>
      </div>
      <div class="btn-row">
        <button class="btn btn-on" onclick="sendCmd('on','led-state','led-ind')">Turn On</button>
        <button class="btn btn-off" onclick="sendCmd('off','led-state','led-ind')">Turn Off</button>
      </div>
    </div>

    <div class="ctrl-card">
      <div class="ctrl-top">
        <div class="ctrl-icon door-ci">🚪</div>
        <div class="ctrl-meta">
          <div class="ctrl-name">Door Lock</div>
          <div class="ctrl-state" id="door-state">CLOSED</div>
        </div>
        <div class="led-indicator" id="door-ind"></div>
      </div>
      <div class="btn-row">
        <button class="btn btn-on" onclick="sendCmd('openDoor','door-state','door-ind')">Unlock</button>
        <button class="btn btn-off" onclick="sendCmd('closeDoor','door-state','door-ind')">Lock</button>
      </div>
    </div>

    <div class="ctrl-card">
      <div class="ctrl-top">
        <div class="ctrl-icon win-ci">🪟</div>
        <div class="ctrl-meta">
          <div class="ctrl-name">Window</div>
          <div class="ctrl-state" id="win-state">CLOSED</div>
        </div>
        <div class="led-indicator" id="win-ind"></div>
      </div>
      <div class="btn-row">
        <button class="btn btn-on" onclick="sendCmd('openWindow','win-state','win-ind')">Open</button>
        <button class="btn btn-off" onclick="sendCmd('closeWindow','win-state','win-ind')">Close</button>
      </div>
    </div>

    <!-- CLOSING ACTUATOR CARD -->
    <div class="ctrl-card">
      <div class="ctrl-top">
        <div class="ctrl-icon actuator-ci">🔒</div>
        <div class="ctrl-meta">
          <div class="ctrl-name">Closing Actuator</div>
          <div class="ctrl-state" id="actuator-state">RETRACTED</div>
        </div>
        <div class="led-indicator" id="actuator-ind"></div>
      </div>
      <div class="btn-row">
        <button class="btn btn-on" onclick="sendActuatorCmd('extend')">Extend</button>
        <button class="btn btn-off" onclick="sendActuatorCmd('retract')">Retract</button>
      </div>
      <div class="btn-row" style="margin-top: 8px;">
        <button class="btn btn-auto" id="auto-mode-btn" onclick="toggleActuatorAuto()" style="background: rgba(99,102,241,0.2); border-color: rgba(99,102,241,0.35);">Auto Mode: ON</button>
      </div>
      <p class="card-foot" id="actuator-footnote" style="font-size: 10px; color: var(--text-3); margin-top: 12px;">Auto: extends when rain detected</p>
    </div>

    <div class="ctrl-card ctrl-wide">
      <div class="ctrl-top">
        <div class="ctrl-icon motor-ci">⚙️</div>
        <div class="ctrl-meta">
          <div class="ctrl-name">DC Motor</div>
          <div class="ctrl-state" id="motor-state">STOPPED</div>
        </div>
        <div class="led-indicator" id="motor-ind"></div>
      </div>
      <div class="btn-row" style="margin-bottom:20px">
        <button class="btn btn-on" onclick="motorCmd('motorForward')">Start</button>
        <button class="btn btn-off" onclick="motorCmd('motorStop')">Stop</button>
      </div>
      <div class="speed-section">
        <div class="speed-row">
          <span class="speed-lbl">Power Output</span>
          <span class="speed-val"><span id="speed-num">0</span> / 255</span>
        </div>
        <div class="speed-track">
          <div class="speed-fill" id="speed-fill"></div>
          <input type="range" class="speed-range" id="speed-slider" min="0" max="255" value="0"
            oninput="setSpeed(this.value)" onchange="setSpeed(this.value)">
        </div>
      </div>
    </div>

  </div>
</div>

<!-- ══ CAMERA ══ -->
<div id="tab-camera" class="tab-panel">
  <div class="camera-container">
    <div class="camera-card">
      <div class="camera-header">
        <div class="camera-title">
          <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
            <path d="M23 19a2 2 0 0 1-2 2H3a2 2 0 0 1-2-2V8a2 2 0 0 1 2-2h4l2-3h6l2 3h4a2 2 0 0 1 2 2z"/>
            <circle cx="12" cy="13" r="4"/>
          </svg>
          <span>Live Camera Feed</span>
        </div>
        <div class="camera-controls">
          <button class="cam-btn" onclick="refreshCamera()">
            <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
              <path d="M23 4v6h-6"/>
              <path d="M1 20v-6h6"/>
              <path d="M3.51 9a9 9 0 0 1 14.85-3.36L23 10M1 14l4.64 4.36A9 9 0 0 0 20.49 15"/>
            </svg>
            Refresh
          </button>
        </div>
      </div>
      <div class="camera-view">
        <iframe id="cameraFrame" src="http://192.168.1.11:5000" 
                style="width: 100%; height: 500px; border: none; background: #000;">
        </iframe>
      </div>
      <div class="camera-status">
        <div class="status-indicator">
          <span class="cam-dot"></span>
          <span>Camera: 192.168.1.11:5000</span>
        </div>
      </div>
    </div>
  </div>
</div>

<script>
let cameraActive = true;
let cameraRefreshInterval;
let actuatorAutoMode = true;

// Clock
function tickClock(){document.getElementById('clock').textContent=new Date().toTimeString().slice(0,8);}
setInterval(tickClock,1000);tickClock();

// Tabs
function switchTab(name,btn){
  document.querySelectorAll('.tab-panel').forEach(p=>p.classList.remove('active'));
  document.querySelectorAll('.tab-btn').forEach(b=>b.classList.remove('active'));
  document.getElementById('tab-'+name).classList.add('active');
  btn.classList.add('active');
}

// Camera functions
function refreshCamera() {
  const iframe = document.getElementById('cameraFrame');
  const timestamp = new Date().getTime();
  iframe.src = `http://192.168.1.11:5000?t=${timestamp}`;
}

// Arc helpers
function setArc(id,pct,total){
  const el=document.getElementById(id);
  if(el) el.style.strokeDashoffset=total-(total*pct/100);
}
function setGaugeArc(id,pct){
  const el=document.getElementById(id);
  if(el) el.style.strokeDashoffset=251-(251*pct/100);
}

// Actuator functions
function sendActuatorCmd(action) {
  let endpoint = action === 'extend' ? '/actuatorExtend' : '/actuatorRetract';
  fetch(endpoint).then(r=>r.text()).then(data=>{
    updateActuatorUI();
  });
}

function toggleActuatorAuto() {
  fetch('/actuatorAuto').then(r=>r.text()).then(data=>{
    actuatorAutoMode = true;
    updateActuatorUI();
  });
}

function updateActuatorUI() {
  fetch('/actuatorStatus').then(r=>r.json()).then(data=>{
    document.getElementById('actuator-state').textContent = data.state.toUpperCase();
    document.getElementById('actuator-ind').classList.toggle('ind-on', data.state === 'extended');
    actuatorAutoMode = (data.mode === 'auto');
    const autoBtn = document.getElementById('auto-mode-btn');
    if (autoBtn) {
      autoBtn.textContent = actuatorAutoMode ? 'Auto Mode: ON' : 'Auto Mode: OFF (Manual)';
      autoBtn.style.opacity = actuatorAutoMode ? '1' : '0.7';
    }
    const footnote = document.getElementById('actuator-footnote');
    if (footnote) {
      footnote.textContent = actuatorAutoMode ? 'Auto: extends when rain detected' : 'Manual control active';
      footnote.style.color = actuatorAutoMode ? 'var(--text-3)' : '#fb923c';
    }
  });
}
// Sound sensor update 
function updateSound(soundDetected, soundPct) {
  const badge = document.getElementById('sound-badge');
  const orb = document.getElementById('sound-orb');
  const statusText = document.getElementById('sound-status-text');
  const message = document.getElementById('sound-message');
  
  if (soundDetected) {
    // When sound is detected
    badge.textContent = 'DETECTED!';
    badge.className = 'chip chip-danger';
    orb.classList.add('orb-sound-alert');
    statusText.textContent = '  Sound Detected ';
    statusText.style.color = '#f87171';
    message.textContent = '⚠ Sound detected at window! Window closed.';
    message.style.color = '#f87171';
    document.getElementById('sound-card').classList.add('card-danger');
  } else {
    // When no sound - CLEAR status
    badge.textContent = 'Clear';
    badge.className = 'chip chip-ok';
    orb.classList.remove('orb-sound-alert');
    statusText.textContent = 'Clear';
    statusText.style.color = '#4ade80';
    message.textContent = 'No sound detected at window';
    message.style.color = '#94a3b8';
    document.getElementById('sound-card').classList.remove('card-danger');
  }
}

// Sensor updates
function updateGas(val){
  const pct=Math.min(100,(val/4095)*100),danger=val>1500;
  document.getElementById('gas-val').textContent=val;
  document.getElementById('gas-bar').style.width=pct+'%';
  const badge=document.getElementById('gas-badge');
  badge.textContent=danger?'Danger':'Normal';
  badge.className='chip '+(danger?'chip-danger':'chip-ok');
  setGaugeArc('gas-arc',pct);
  document.getElementById('gas-card').classList.toggle('card-danger',danger);
}

function updateFlame(detected,intensity){
  const pct=Math.min(100,Math.max(0,((4095-intensity)/4095)*100));
  document.getElementById('flame-pct').textContent=pct.toFixed(0)+'%';
  document.getElementById('flame-bar').style.width=pct+'%';
  const badge=document.getElementById('flame-badge');
  badge.textContent=detected?'FIRE!':'Clear';
  badge.className='chip '+(detected?'chip-danger':'chip-ok');
  document.getElementById('flame-orb').classList.toggle('orb-fire',detected);
  document.getElementById('flame-card').classList.toggle('card-danger',detected);
}

function updateRain(detected,pct){
  document.getElementById('rain-pct').textContent=pct+'%';
  document.getElementById('rain-bar').style.width=pct+'%';
  const badge=document.getElementById('rain-badge');
  badge.textContent=detected?'Raining':'Dry';
  badge.className='chip '+(detected?'chip-rain':'chip-ok');
  document.getElementById('rain-visual').classList.toggle('raining',detected);
  document.getElementById('rain-card').classList.toggle('card-rain',detected);
}

function updateMotion(motionDoor, motionWindow){
  const doorOrb = document.getElementById('motion-door-orb');
  const doorBadge = document.getElementById('motion-door-badge');
  const doorState = document.getElementById('motion-door-state');
  const doorStatus = document.getElementById('motion-door-status');
  
  if(doorOrb) {
    doorOrb.classList.toggle('orb-motion', motionDoor);
    document.getElementById('motion-door-card').classList.toggle('card-motion', motionDoor);
    doorBadge.textContent = motionDoor ? 'Detected' : 'Clear';
    doorBadge.className = 'chip ' + (motionDoor ? 'chip-motion' : 'chip-ok');
    doorState.textContent = motionDoor ? 'MOTION!' : 'CLEAR';
    doorStatus.textContent = motionDoor ? '⚠ Motion detected at door' : 'Monitoring door...';
    doorStatus.style.color = motionDoor ? '#facc15' : '';
  }
  
  const windowOrb = document.getElementById('motion-window-orb');
  const windowBadge = document.getElementById('motion-window-badge');
  const windowState = document.getElementById('motion-window-state');
  const windowStatus = document.getElementById('motion-window-status');
  
  if(windowOrb) {
    windowOrb.classList.toggle('orb-motion', motionWindow);
    document.getElementById('motion-window-card').classList.toggle('card-motion', motionWindow);
    windowBadge.textContent = motionWindow ? 'Detected' : 'Clear';
    windowBadge.className = 'chip ' + (motionWindow ? 'chip-motion' : 'chip-ok');
    windowState.textContent = motionWindow ? 'MOTION!' : 'CLEAR';
    windowStatus.textContent = motionWindow ? '⚠ Motion detected at window' : 'Monitoring window...';
    windowStatus.style.color = motionWindow ? '#facc15' : '';
  }
}

function updateDHT(data){
  if(!data.valid){
    document.getElementById('dht-temp').textContent='--';
    document.getElementById('dht-hum').textContent='--';
    document.getElementById('dht-badge').textContent='Error';
    document.getElementById('dht-badge').className='chip chip-danger';
    document.getElementById('dht-status').textContent='Sensor offline';
    return;
  }
  const t=data.temperature,h=data.humidity;
  document.getElementById('dht-temp').textContent=t;
  document.getElementById('dht-hum').textContent=h;
  setArc('temp-arc',Math.min(100,(t/50)*100),239);
  setArc('hum-arc',Math.min(100,h),239);
  let s='Comfortable',c='';
  if(t>=35){s='⚠ Too Hot';c='#f87171';}
  else if(t<=10){s='⚠ Too Cold';c='#93c5fd';}
  else if(h>=80){s='⚠ Very Humid';c='#fb923c';}
  else if(h<=20){s='⚠ Very Dry';c='#fb923c';}
  const el=document.getElementById('dht-status');
  el.textContent=s; el.style.color=c;
  document.getElementById('dht-badge').textContent='Live';
  document.getElementById('dht-badge').className='chip chip-ok';
}

// Commands
function sendCmd(cmd,stateId,indId){
  fetch('/'+cmd).then(r=>r.text()).then(data=>{
    document.getElementById(stateId).textContent=data.toUpperCase();
    document.getElementById(indId).classList.toggle('ind-on',['on','opening','opened','ajar'].includes(data.toLowerCase()));
  });
}

function motorCmd(cmd){
  fetch('/'+cmd).then(()=>{
    const spd=cmd==='motorForward'?255:0;
    document.getElementById('speed-slider').value=spd;
    setSpeed(spd);
  });
}

function setSpeed(val){
  document.getElementById('speed-num').textContent=val;
  document.getElementById('speed-fill').style.width=(val/255*100)+'%';
  document.getElementById('speed-slider').value=val;
  fetch('/motorSpeed?speed='+val).then(()=>refreshMotor());
}

function refreshMotor(){
  fetch('/motorStatus').then(r=>r.text()).then(data=>{
    const [dir,spd]=data.split(':');
    document.getElementById('motor-state').textContent=dir.toUpperCase();
    document.getElementById('speed-num').textContent=spd;
    document.getElementById('speed-fill').style.width=(spd/255*100)+'%';
    document.getElementById('speed-slider').value=spd;
    document.getElementById('motor-ind').classList.toggle('ind-on',parseInt(spd)>0);
  });
}

function pollAll(){
  fetch('/sensorData').then(r=>r.json()).then(d=>{
    updateGas(d.gas);
    updateFlame(d.flame,d.flameIntensity);
    updateRain(d.rain,d.rainPct);
    updateMotion(d.motionDoor, d.motionWindow);
    updateSound(d.sound, d.soundPct);
  }).catch(()=>{});
  fetch('/dhtData').then(r=>r.json()).then(d=>updateDHT(d)).catch(()=>{});
  fetch('/Doorstatus').then(r=>r.text()).then(d=>{
    document.getElementById('door-state').textContent=d.toUpperCase();
    document.getElementById('door-ind').classList.toggle('ind-on',['opening','opened','ajar'].includes(d));
  }).catch(()=>{});
  fetch('/WindowStatus').then(r=>r.text()).then(d=>{
    document.getElementById('win-state').textContent=d.toUpperCase();
    document.getElementById('win-ind').classList.toggle('ind-on',['opening','opened'].includes(d));
  }).catch(()=>{});
  fetch('/Ledstatus').then(r=>r.text()).then(d=>{
    document.getElementById('led-state').textContent=d.toUpperCase();
    document.getElementById('led-ind').classList.toggle('ind-on',d==='on');
  }).catch(()=>{});
  refreshMotor();
  updateActuatorUI();
}

window.onload=()=>{
  pollAll();
  setInterval(pollAll,2000);
};

</script>
</body>
</html>
)rawliteral";

#endif