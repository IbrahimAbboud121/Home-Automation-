#ifndef STYLES_H
#define STYLES_H

const char* CSS_STYLES = R"rawliteral(
@import url('https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@300;400;500;600;700&family=JetBrains+Mono:wght@400;600&display=swap');

*, *::before, *::after { margin:0; padding:0; box-sizing:border-box; }

:root {
  --bg:        #0f1117;
  --bg2:       #161b27;
  --surface:   rgba(255,255,255,0.04);
  --surface-h: rgba(255,255,255,0.07);
  --border:    rgba(255,255,255,0.08);
  --border-h:  rgba(255,255,255,0.14);
  --accent:    #6366f1;
  --accent2:   #818cf8;
  --green:     #4ade80;
  --red:       #f87171;
  --orange:    #fb923c;
  --yellow:    #facc15;
  --blue:      #38bdf8;
  --rain:      #60a5fa;
  --text:      #e2e8f0;
  --text-2:    #94a3b8;
  --text-3:    #475569;
  --mono:      'JetBrains Mono', monospace;
  --sans:      'Space Grotesk', sans-serif;
  --radius:    16px;
}

html, body {
  min-height: 100vh;
  background: var(--bg);
  color: var(--text);
  font-family: var(--sans);
  font-size: 15px;
  overflow-x: hidden;
  line-height: 1.5;
}

/* ── Ambient orbs ─────────────────────────────────────────────────────────── */
.orb {
  position: fixed; border-radius: 50%;
  filter: blur(80px); pointer-events: none; z-index: 0;
  opacity: 0.22;
}
.orb1 { width: 500px; height: 500px; top: -100px; left: -100px; background: #6366f1; }
.orb2 { width: 400px; height: 400px; bottom: -80px; right: -80px; background: #0ea5e9; }
.orb3 { width: 300px; height: 300px; top: 50%; left: 50%; transform: translate(-50%,-50%); background: #8b5cf6; opacity: 0.1; }

/* ── Header ───────────────────────────────────────────────────────────────── */
header {
  position: relative; z-index: 10;
  display: flex; align-items: center; justify-content: space-between;
  padding: 18px 28px;
  background: rgba(255,255,255,0.025);
  border-bottom: 1px solid var(--border);
  backdrop-filter: blur(20px);
}

.header-left { display: flex; align-items: center; gap: 14px; }

.logo-icon {
  color: var(--accent2);
  filter: drop-shadow(0 0 10px rgba(99,102,241,0.5));
}

.logo-name {
  font-weight: 700; font-size: 17px; letter-spacing: 1px;
  color: #fff;
}
.logo-ver {
  font-size: 11px; color: var(--text-3); letter-spacing: 0.5px;
}

.header-right { display: flex; align-items: center; gap: 20px; }

.status-pill {
  display: flex; align-items: center; gap: 7px;
  padding: 5px 12px; border-radius: 999px;
  background: rgba(74,222,128,0.1); border: 1px solid rgba(74,222,128,0.25);
  font-size: 12px; font-weight: 600; color: var(--green); letter-spacing: 0.3px;
}
.pulse-dot {
  width: 7px; height: 7px; border-radius: 50%;
  background: var(--green);
  box-shadow: 0 0 0 0 rgba(74,222,128,0.4);
  animation: livePulse 2s ease-in-out infinite;
}
@keyframes livePulse {
  0%  { box-shadow: 0 0 0 0 rgba(74,222,128,0.5); }
  70% { box-shadow: 0 0 0 8px rgba(74,222,128,0); }
  100%{ box-shadow: 0 0 0 0 rgba(74,222,128,0); }
}

.clock-box { text-align: right; }
.clock { font-family: var(--mono); font-size: 18px; font-weight: 600; color: #fff; letter-spacing: 2px; }
.clock-label { font-size: 9px; color: var(--text-3); letter-spacing: 2px; margin-top: 1px; }

/* ── Tab nav ──────────────────────────────────────────────────────────────── */
.tab-nav {
  position: relative; z-index: 10;
  display: flex; gap: 4px;
  padding: 12px 28px 0;
  border-bottom: 1px solid var(--border);
  background: rgba(255,255,255,0.015);
}

.tab-btn {
  display: flex; align-items: center; gap: 8px;
  background: none; border: none;
  font-family: var(--sans); font-size: 13px; font-weight: 500;
  color: var(--text-2); padding: 10px 18px 14px;
  cursor: pointer; position: relative;
  border-radius: 8px 8px 0 0;
  transition: color .2s, background .2s;
}
.tab-btn:hover { color: var(--text); background: rgba(255,255,255,0.04); }
.tab-btn.active { color: #fff; }
.tab-btn::after {
  content: ''; position: absolute; bottom: 0; left: 0; right: 0; height: 2px;
  background: linear-gradient(90deg, var(--accent), var(--accent2));
  border-radius: 2px 2px 0 0;
  transform: scaleX(0); transition: transform .25s ease;
}
.tab-btn.active::after { transform: scaleX(1); }

/* ── Panels ───────────────────────────────────────────────────────────────── */
.tab-panel { display: none; padding: 24px 28px; position: relative; z-index: 5; }
.tab-panel.active { display: block; animation: panelIn .25s ease; }
@keyframes panelIn { from { opacity:0; transform:translateY(6px); } to { opacity:1; transform:none; } }

/* ── Sensor grid ──────────────────────────────────────────────────────────── */
.sensor-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(270px, 1fr));
  gap: 16px;
}

/* ── Base card ────────────────────────────────────────────────────────────── */
.card {
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius);
  padding: 20px;
  position: relative; overflow: hidden;
  backdrop-filter: blur(12px);
  transition: border-color .3s, box-shadow .3s, transform .2s;
}
.card::after {
  content: ''; position: absolute;
  top: 0; left: 0; right: 0; height: 1px;
  background: linear-gradient(90deg, transparent, rgba(255,255,255,0.1), transparent);
}
.card:hover {
  border-color: var(--border-h);
  box-shadow: 0 8px 40px rgba(0,0,0,0.3);
  transform: translateY(-1px);
}

.card-wide { grid-column: span 2; }
@media (max-width: 640px) { .card-wide { grid-column: span 1; } }

/* Danger / alert states */
.card.card-danger {
  border-color: rgba(248,113,113,0.4) !important;
  box-shadow: 0 0 30px rgba(248,113,113,0.1) !important;
  animation: cardAlert 1.5s ease-in-out infinite;
}
@keyframes cardAlert {
  0%,100% { box-shadow: 0 0 20px rgba(248,113,113,0.08); }
  50%     { box-shadow: 0 0 40px rgba(248,113,113,0.2); }
}
.card.card-rain  { border-color: rgba(96,165,250,0.35) !important; }
.card.card-motion{ border-color: rgba(250,204,21,0.35) !important; }

/* ── Card top row ─────────────────────────────────────────────────────────── */
.card-top {
  display: flex; align-items: center; gap: 10px; margin-bottom: 18px;
}
.card-icon {
  width: 36px; height: 36px; border-radius: 10px;
  display: flex; align-items: center; justify-content: center;
  flex-shrink: 0;
}
.gas-icon    { background: rgba(99,102,241,0.15); color: var(--accent2); }
.flame-icon  { background: rgba(251,146,60,0.15); color: var(--orange); }
.rain-icon   { background: rgba(56,189,248,0.15); color: var(--blue); }
.motion-icon { background: rgba(250,204,21,0.15); color: var(--yellow); }
.climate-icon{ background: rgba(74,222,128,0.12); color: var(--green); }
.actuator-icon { background: rgba(139,92,246,0.15); color: #a78bfa; }
.sound-icon  { background: rgba(248,113,113,0.15); color: #f87171; }

.card-label {
  flex: 1; font-weight: 600; font-size: 13px; color: var(--text-2);
  letter-spacing: 0.3px;
}

/* ── Chips / badges ───────────────────────────────────────────────────────── */
.chip {
  font-size: 10px; font-weight: 700; letter-spacing: 0.8px; text-transform: uppercase;
  padding: 3px 9px; border-radius: 999px;
  transition: all .3s;
}
.chip-ok     { background: rgba(74,222,128,0.12); color: var(--green);  border: 1px solid rgba(74,222,128,0.25); }
.chip-danger { background: rgba(248,113,113,0.15); color: var(--red);   border: 1px solid rgba(248,113,113,0.35); animation: chipBlink .6s ease-in-out infinite; }
.chip-rain   { background: rgba(96,165,250,0.12);  color: var(--rain);  border: 1px solid rgba(96,165,250,0.3); }
.chip-motion { background: rgba(250,204,21,0.12);  color: var(--yellow);border: 1px solid rgba(250,204,21,0.3); }
@keyframes chipBlink { 0%,100%{opacity:1;} 50%{opacity:.5;} }

/* ── Gauge (gas) ──────────────────────────────────────────────────────────── */
.gauge-wrap { display: flex; justify-content: center; margin: 6px 0 4px; }
.gauge { width: 170px; height: 115px; }
.gauge-val  { font-family: var(--mono); font-size: 28px; font-weight: 600; fill: #fff; }
.gauge-unit { font-family: var(--sans); font-size: 10px; fill: var(--text-3); letter-spacing: 1px; }

/* ── Progress bars ────────────────────────────────────────────────────────── */
.bar-row { margin-top: 14px; display: flex; flex-direction: column; gap: 6px; }
.bar-lbl  { font-size: 10px; color: var(--text-3); font-weight: 500; letter-spacing: 0.5px; }
.prog-bar {
  height: 5px; background: rgba(255,255,255,0.06);
  border-radius: 3px; overflow: hidden;
}
.prog-fill {
  height: 100%; width: 0%; border-radius: 3px;
  transition: width .5s ease;
}
.green-fill  { background: linear-gradient(90deg, #4ade80, #facc15, #f87171); }
.orange-fill { background: linear-gradient(90deg, #facc15, #fb923c, #f87171); }
.blue-fill   { background: linear-gradient(90deg, #bae6fd, #38bdf8, #60a5fa); }
.red-fill    { background: linear-gradient(90deg, #facc15, #fb923c, #f87171); }

/* ── Orb display ──────────────────────────────────────────────────────────── */
.orb-display { display: flex; justify-content: center; margin: 8px 0; }
.sensor-orb {
  width: 120px; height: 120px; border-radius: 50%;
  display: flex; flex-direction: column; align-items: center; justify-content: center;
  border: 1.5px solid rgba(255,255,255,0.1);
  background: radial-gradient(circle at 35% 35%, rgba(255,255,255,0.04), transparent);
  position: relative; transition: all .4s;
}
.orb-main { font-family: var(--mono); font-size: 26px; font-weight: 600; color: #fff; }
.orb-sub  { font-size: 10px; color: var(--text-3); margin-top: 3px; letter-spacing: 0.5px; }

/* Flame orb active */
.flame-orb.orb-fire {
  border-color: rgba(251,146,60,0.6);
  background: radial-gradient(circle, rgba(251,146,60,0.18), rgba(248,113,113,0.08) 70%, transparent);
  box-shadow: 0 0 30px rgba(251,146,60,0.25), inset 0 0 20px rgba(251,146,60,0.08);
  animation: fireOrb .7s ease-in-out infinite alternate;
}
@keyframes fireOrb {
  from { transform: scale(1);    }
  to   { transform: scale(1.04); box-shadow: 0 0 45px rgba(251,146,60,0.4); }
}

/* Sound orb active (NEW) */
.sound-orb.orb-sound-alert {
  border-color: rgba(248,113,113,0.7);
  background: radial-gradient(circle, rgba(248,113,113,0.2), rgba(248,113,113,0.08) 70%, transparent);
  box-shadow: 0 0 30px rgba(248,113,113,0.3);
  animation: soundAlert 0.5s ease-in-out infinite alternate;
}
@keyframes soundAlert {
  from { transform: scale(1); box-shadow: 0 0 20px rgba(248,113,113,0.3); }
  to   { transform: scale(1.05); box-shadow: 0 0 50px rgba(248,113,113,0.5); }
}

/* Sound wave animation */
.sound-wave {
  position: absolute;
  border-radius: 50%;
  border: 2px solid rgba(248,113,113,0);
  pointer-events: none;
}
.w1, .w2, .w3 {
  width: 100%;
  height: 100%;
  top: 0;
  left: 0;
}
.orb-sound-alert .w1 {
  animation: soundWave 1s ease-out infinite;
}
.orb-sound-alert .w2 {
  animation: soundWave 1s ease-out 0.3s infinite;
}
.orb-sound-alert .w3 {
  animation: soundWave 1s ease-out 0.6s infinite;
}
@keyframes soundWave {
  0% {
    border-color: rgba(248,113,113,0.6);
    transform: scale(1);
    opacity: 1;
  }
  100% {
    border-color: rgba(248,113,113,0);
    transform: scale(1.5);
    opacity: 0;
  }
}

/* Motion orb */
.motion-ripple {
  position: absolute; border-radius: 50%;
  border: 1px solid rgba(250,204,21,0.2);
  opacity: 0; pointer-events: none;
}
.r1 { width: 130%; height: 130%; }
.r2 { width: 160%; height: 160%; }
.r3 { width: 190%; height: 190%; }

.motion-orb.orb-motion {
  border-color: rgba(250,204,21,0.5);
  background: radial-gradient(circle, rgba(250,204,21,0.12), transparent);
  box-shadow: 0 0 25px rgba(250,204,21,0.2);
}
.motion-orb.orb-motion .motion-ripple {
  animation: rippleOut 2s ease-out infinite;
}
.motion-orb.orb-motion .r2 { animation-delay: .5s; }
.motion-orb.orb-motion .r3 { animation-delay: 1s; }
@keyframes rippleOut {
  0%   { opacity:.5; transform:scale(.7); }
  100% { opacity:0;  transform:scale(1); }
}

/* ── Rain visual ──────────────────────────────────────────────────────────── */
.rain-visual {
  display: flex; align-items: center; justify-content: center;
  gap: 16px; height: 90px; margin: 8px 0;
}
.rain-col { display: flex; flex-direction: column; gap: 6px; }
.rdrop {
  width: 3px; height: 14px; border-radius: 2px;
  background: var(--blue); opacity: .15;
  transition: opacity .3s;
}
.delay1 { animation-delay: .2s; }
.delay2 { animation-delay: .4s; }

.raining .rain-col { animation: rainFall .8s ease-in-out infinite; }
.raining .delay1   { animation-delay: .25s; }
.raining .delay2   { animation-delay: .1s; }
.raining .rdrop    { opacity: .7; }
@keyframes rainFall {
  0%,100% { transform: translateY(0); }
  50%     { transform: translateY(4px); }
}

.rain-big {
  font-family: var(--mono); font-size: 32px; font-weight: 600;
  color: var(--rain); min-width: 70px; text-align: center;
}

/* ── Climate DHT ──────────────────────────────────────────────────────────── */
.dht-row {
  display: flex; align-items: center; justify-content: center;
  gap: 24px; margin: 10px 0;
}
.dht-block { display: flex; flex-direction: column; align-items: center; gap: 10px; }
.dht-sep { width: 1px; height: 110px; background: var(--border-h); }

.dht-ring-wrap {
  position: relative; width: 110px; height: 110px;
}
.dht-ring-svg { width: 100%; height: 100%; }
.dht-center {
  position: absolute; inset: 0;
  display: flex; align-items: center; justify-content: center;
  gap: 1px;
}
.dht-num  { font-family: var(--mono); font-size: 22px; font-weight: 600; color: #fff; }
.dht-u    { font-size: 12px; color: var(--text-3); align-self: flex-start; margin-top: 4px; }
.dht-name { font-size: 11px; color: var(--text-3); font-weight: 500; letter-spacing: 0.5px; }

.card-foot {
  text-align: center; font-size: 12px; color: var(--green);
  margin-top: 12px; font-weight: 500; transition: color .3s;
}

/* ── Control grid ─────────────────────────────────────────────────────────── */
.ctrl-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(270px, 1fr));
  gap: 16px;
}

/* ── Control card ─────────────────────────────────────────────────────────── */
.ctrl-card {
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius);
  padding: 22px;
  backdrop-filter: blur(12px);
  transition: border-color .3s, box-shadow .2s;
}
.ctrl-card:hover { border-color: var(--border-h); }
.ctrl-wide { grid-column: span 2; }
@media (max-width: 640px) { .ctrl-wide { grid-column: span 1; } }

.ctrl-top {
  display: flex; align-items: center; gap: 14px; margin-bottom: 20px;
}
.ctrl-icon {
  width: 44px; height: 44px; border-radius: 12px; font-size: 22px;
  display: flex; align-items: center; justify-content: center;
  flex-shrink: 0;
}
.led-ci      { background: rgba(250,204,21,0.12); }
.door-ci     { background: rgba(99,102,241,0.12); }
.win-ci      { background: rgba(56,189,248,0.12); }
.motor-ci    { background: rgba(74,222,128,0.10); }
.actuator-ci { background: rgba(139,92,246,0.12); }

.ctrl-meta { flex: 1; }
.ctrl-name  { font-size: 11px; color: var(--text-3); font-weight: 600; letter-spacing: 0.5px; text-transform: uppercase; }
.ctrl-state { font-family: var(--mono); font-size: 17px; font-weight: 600; color: #fff; margin-top: 2px; }

/* Status dot */
.led-indicator {
  width: 10px; height: 10px; border-radius: 50%;
  background: var(--text-3);
  transition: all .3s;
}
.led-indicator.ind-on {
  background: var(--green);
  box-shadow: 0 0 0 3px rgba(74,222,128,0.2), 0 0 10px rgba(74,222,128,0.4);
  animation: dotPulse 2s ease-in-out infinite;
}
@keyframes dotPulse {
  0%,100%{ box-shadow: 0 0 0 3px rgba(74,222,128,0.2), 0 0 10px rgba(74,222,128,0.4); }
  50%    { box-shadow: 0 0 0 5px rgba(74,222,128,0.1), 0 0 18px rgba(74,222,128,0.5); }
}

/* ── Buttons ──────────────────────────────────────────────────────────────── */
.btn-row { display: flex; gap: 10px; }

.btn {
  flex: 1; padding: 11px 16px;
  border-radius: 10px; border: 1px solid transparent;
  font-family: var(--sans); font-size: 13px; font-weight: 600;
  cursor: pointer; transition: all .2s; letter-spacing: 0.3px;
}
.btn-on {
  background: linear-gradient(135deg, rgba(99,102,241,0.2), rgba(129,140,248,0.1));
  border-color: rgba(99,102,241,0.35); color: var(--accent2);
}
.btn-on:hover {
  background: linear-gradient(135deg, rgba(99,102,241,0.3), rgba(129,140,248,0.18));
  border-color: rgba(129,140,248,0.5);
  box-shadow: 0 0 20px rgba(99,102,241,0.2);
  transform: translateY(-1px);
}
.btn-on:active { transform: translateY(0); }

.btn-off {
  background: rgba(255,255,255,0.04);
  border-color: rgba(255,255,255,0.08); color: var(--text-2);
}
.btn-off:hover {
  background: rgba(255,255,255,0.08); color: var(--text);
  transform: translateY(-1px);
}
.btn-off:active { transform: translateY(0); }

.btn-auto {
  background: rgba(99,102,241,0.15);
  border-color: rgba(99,102,241,0.3);
  color: #a78bfa;
}
.btn-auto:hover {
  background: rgba(99,102,241,0.25);
  border-color: rgba(99,102,241,0.5);
  transform: translateY(-1px);
}

/* ── Speed slider ─────────────────────────────────────────────────────────── */
.speed-section {
  padding-top: 18px; border-top: 1px solid var(--border);
}
.speed-row {
  display: flex; justify-content: space-between;
  align-items: center; margin-bottom: 10px;
}
.speed-lbl { font-size: 11px; color: var(--text-3); font-weight: 600; letter-spacing: 0.5px; text-transform: uppercase; }
.speed-val { font-family: var(--mono); font-size: 13px; color: var(--accent2); }

.speed-track {
  position: relative; height: 8px;
  background: rgba(255,255,255,0.06); border-radius: 4px;
}
.speed-fill {
  position: absolute; left:0; top:0; bottom:0; width:0%;
  border-radius: 4px;
  background: linear-gradient(90deg, var(--accent), var(--accent2));
  box-shadow: 0 0 8px rgba(99,102,241,0.4);
  transition: width .1s;
}
.speed-range {
  position: absolute; inset: -8px 0;
  width: 100%; opacity: 0; cursor: pointer; z-index: 2;
  height: calc(100% + 16px);
}

/* ── Responsive ───────────────────────────────────────────────────────────── */
@media (max-width: 600px) {
  header    { padding: 12px 16px; }
  .tab-panel{ padding: 16px; }
  .sensor-grid, .ctrl-grid { grid-template-columns: 1fr; gap: 12px; }
  .card-wide, .ctrl-wide   { grid-column: span 1; }
  .dht-row  { flex-direction: column; }
  .dht-sep  { width: 80%; height: 1px; }
  .clock    { font-size: 15px; }
  .logo-name{ font-size: 15px; }
}

/* ── Camera Panel Styles ─────────────────────────────────────────────────────── */
.camera-container {
  max-width: 1200px;
  margin: 0 auto;
}

.camera-card {
  background: var(--surface);
  border: 1px solid var(--border);
  border-radius: var(--radius);
  overflow: hidden;
  backdrop-filter: blur(12px);
  transition: all 0.3s ease;
}

.camera-card:hover {
  border-color: var(--border-h);
  box-shadow: 0 8px 40px rgba(0,0,0,0.3);
}

.camera-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 20px 24px;
  border-bottom: 1px solid var(--border);
  background: rgba(255,255,255,0.02);
}

.camera-title {
  display: flex;
  align-items: center;
  gap: 10px;
  font-size: 18px;
  font-weight: 600;
  color: #fff;
}

.camera-title svg {
  color: var(--accent2);
}

.camera-controls {
  display: flex;
  gap: 12px;
}

.cam-btn {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 8px 16px;
  background: rgba(255,255,255,0.05);
  border: 1px solid var(--border);
  border-radius: 8px;
  color: var(--text);
  font-family: var(--sans);
  font-size: 13px;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.2s ease;
}

.cam-btn:hover {
  background: rgba(255,255,255,0.1);
  border-color: var(--border-h);
  transform: translateY(-1px);
}

.cam-btn:active {
  transform: translateY(0);
}

.camera-view {
  position: relative;
  background: #000;
  min-height: 480px;
  display: flex;
  align-items: center;
  justify-content: center;
  background: linear-gradient(135deg, #0a0a0a 0%, #1a1a1a 100%);
}

#cameraFeed {
  width: 100%;
  max-height: 70vh;
  object-fit: contain;
  background: #0a0a0a;
}

.camera-overlay {
  position: absolute;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0,0,0,0.95);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 10;
  backdrop-filter: blur(10px);
}

.overlay-content {
  text-align: center;
  color: var(--text);
}

.overlay-content svg {
  color: #f87171;
  margin-bottom: 20px;
}

.overlay-content p {
  margin: 10px 0;
  font-size: 16px;
}

.overlay-content button {
  margin-top: 20px;
  padding: 10px 24px;
  background: var(--accent);
  border: none;
  border-radius: 8px;
  color: white;
  font-family: var(--sans);
  font-size: 14px;
  font-weight: 600;
  cursor: pointer;
  transition: all 0.2s ease;
}

.overlay-content button:hover {
  background: var(--accent2);
  transform: translateY(-1px);
}

.camera-status {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 16px 24px;
  border-top: 1px solid var(--border);
  background: rgba(255,255,255,0.02);
}

.status-indicator {
  display: flex;
  align-items: center;
  gap: 8px;
  font-size: 13px;
  color: var(--text-2);
}

.cam-dot {
  width: 8px;
  height: 8px;
  border-radius: 50%;
  background: #4ade80;
  box-shadow: 0 0 0 0 rgba(74,222,128,0.4);
  animation: pulse 2s ease-in-out infinite;
}

.cam-dot.connecting {
  background: #facc15;
  animation: pulse 1s ease-in-out infinite;
}

.cam-dot.error {
  background: #f87171;
  animation: none;
}

.cam-dot.inactive {
  background: #64748b;
  animation: none;
}

@keyframes pulse {
  0% {
    box-shadow: 0 0 0 0 rgba(74,222,128,0.4);
  }
  70% {
    box-shadow: 0 0 0 6px rgba(74,222,128,0);
  }
  100% {
    box-shadow: 0 0 0 0 rgba(74,222,128,0);
  }
}

.camera-info {
  display: flex;
  gap: 20px;
  font-size: 12px;
  color: var(--text-3);
  font-family: var(--mono);
}

.camera-info span {
  display: flex;
  align-items: center;
  gap: 5px;
}

/* Responsive camera */
@media (max-width: 768px) {
  .camera-header {
    flex-direction: column;
    gap: 15px;
    align-items: flex-start;
  }
  
  .camera-controls {
    width: 100%;
  }
  
  .cam-btn {
    flex: 1;
    justify-content: center;
  }
  
  .camera-status {
    flex-direction: column;
    gap: 10px;
    align-items: flex-start;
  }
  
  .camera-info {
    flex-wrap: wrap;
  }
}
)rawliteral";

#endif