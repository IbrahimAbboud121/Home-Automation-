<div align="center">

# 🏠 NEXUS HOME
### AI-Powered Smart Home System | ESP32 + Computer Vision + Hand Gesture Door Lock + Live Web Dashboard

[![YouTube](https://img.shields.io/badge/Watch%20on-YouTube-red?style=for-the-badge&logo=youtube)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID)
[![Platform](https://img.shields.io/badge/Platform-ESP32-blue?style=for-the-badge&logo=espressif)](https://www.espressif.com/)
[![Python](https://img.shields.io/badge/Python-3.x-yellow?style=for-the-badge&logo=python)](https://www.python.org/)
[![OpenCV](https://img.shields.io/badge/OpenCV-MediaPipe-green?style=for-the-badge&logo=opencv)](https://opencv.org/)
[![License](https://img.shields.io/badge/License-MIT-purple?style=for-the-badge)](LICENSE)

---

## 📺 Demo Video

[![NEXUS HOME Demo](https://img.youtube.com/vi/YOUR_VIDEO_ID/maxresdefault.jpg)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID)

> *Click the thumbnail to watch the full demo on YouTube*

---

</div>

## 📌 About

Built a fully automated smart home system using ESP32 and computer vision — featuring real-time sensor alerts, hand gesture door unlocking, and a live web dashboard to control everything remotely.

---

## ✨ Features

- 🔥 **Automated Threat Response** — Gas, flame, rain, and sound events trigger real-time actions (door, window, buzzer, actuator)
- 🖐️ **Computer Vision Keypad** — Hand gesture air keypad using MediaPipe + OpenCV for touchless door unlocking
- 🌐 **Live Web Dashboard** — Served directly from ESP32, accessible from any device on the network
- 📷 **IP Camera Integration** — Live camera feed embedded in the dashboard
- 📟 **OLED Display** — 7-page local display with danger-priority switching
- 🔔 **Smart Buzzer Siren** — Escalating alarm pattern on gas, flame, or glass-break detection

---

## 🛠️ Tech Stack

| Layer | Technology |
|-------|-----------|
| Microcontroller | ESP32 (Arduino Framework) |
| Computer Vision | Python, OpenCV, MediaPipe, Flask |
| Frontend | HTML, CSS, JavaScript (served from ESP32) |
| Communication | HTTP REST API (ESP32 WebServer) |
| Sensors | Gas, Flame, Rain, DHT11, PIR x2, Sound |
| Actuators | Servo x2, Stepper Motor, DC Motor, Buzzer, LED |

---

## ⚙️ How It Works

### 🖐️ Hand Gesture Door Lock
1. Show your **right hand** to hover over the virtual keypad
2. Close your **left hand** to press a button
3. Enter the correct password → Flask sends HTTP request to ESP32 → Door unlocks

### 🤖 Automated Responses

| Sensor | Trigger | Action |
|--------|---------|--------|
| Gas Sensor | Value > 1500 | Window opens + Buzzer |
| Flame Sensor | Detected | Door opens + Buzzer |
| Rain Sensor | Detected | Window closes + Actuator extends |
| Sound Sensor | Glass break | Window closes + Buzzer |
| PIR (Door) | Motion | LED turns ON |
| PIR (Window) | Motion | Security alert |

---

## 🚀 Getting Started

### ESP32 Setup

1. Install libraries: `ESP32Servo`, `DHT`, `Adafruit SSD1306`, `Adafruit GFX`
2. Set your WiFi credentials in `main.ino`:
```cpp
const char* SSID     = "YOUR_WIFI_SSID";
const char* PASSWORD = "YOUR_PASSWORD";
```
3. Flash to ESP32 and note the IP address from Serial Monitor

### Python (Camera + Gesture) Setup

```bash
pip install flask opencv-python mediapipe requests
```

Update the ESP32 IP in `camera.py`:
```python
ESP32_IP = "YOUR_ESP32_IP"
```

Run the server:
```bash
python camera.py
```

### Access the Dashboard

Open your ESP32's IP address in any browser on the same network.

---

## 📁 File Structure

```
nexus-home/
│
├── main.ino        # ESP32 core logic — sensors, automation, web server
├── html.h          # Web dashboard HTML
├── style.h         # Web dashboard CSS
├── camera.py       # Python — computer vision keypad + camera stream
└── README.md       # This file
```

---

## 📄 License

MIT License — feel free to use, modify, and build on this project.

---

<div align="center">

*If you found this project useful, give it a ⭐ and subscribe on YouTube!*

</div>
