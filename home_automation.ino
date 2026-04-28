#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <Stepper.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "html.h"
#include "style.h"

// ─── WiFi ─────────────────────────────────────────────────────────────────────
const char* SSID = "TP-Link_0680";
const char* PASSWORD = "73334712";
WebServer server(80);

// ─── Pins ─────────────────────────────────────────────────────────────────────
const int LED_PIN = 5;
const int DOOR_SERVO_PIN = 25;
const int GAS_PIN = 35;
const int FLAME_PIN = 32;
const int RAIN_PIN = 33;
const int PIR_DOOR_PIN = 34;    // Door motion sensor
const int PIR_WINDOW_PIN = 13;  // Window motion sensor
const int SOUND_SENSOR_PIN = 1;  // Sound sensor DO pin (digital input)
const int BUZZER_PIN = 2;
const int MOTOR_EN = 14;
const int MOTOR_A = 27;
const int MOTOR_B = 26;
const int IN1 = 4, IN2 = 16, IN3 = 17, IN4 = 18;
#define DHT_PIN 15
#define DHT_TYPE DHT11

// ─── Closing Actuator Servo ───────────────────────────────────────────────────
const int CLOSING_ACTUATOR_PIN = 23;
Servo closingActuator;
int currentActuatorAngle = 0;     
bool actuatorAutoMode = true;      // Auto mode enabled by default

// ─── Thresholds ───────────────────────────────────────────────────────────────
const int GAS_THRESHOLD = 1500;
const int FLAME_THRESHOLD = 2000;
const int RAIN_THRESHOLD = 3000;

// ─── Sensor values ────────────────────────────────────────────────────────────
int gasValue = 0;
int flameIntensity = 0;
bool flameDetected = false;
float dhtTemp = 0.0;
float dhtHum = 0.0;
bool dhtValid = false;
int rainValue = 0;
bool rainDetected = false;
bool motionDoor = false;    // Door motion
bool motionWindow = false;  // Window motion
bool motionAutoLed = false;
bool soundDetected = false;  // Sound detection flag
bool soundAutoClosed = false; // Track if window was closed by sound
unsigned long soundDebounceTime = 0;
const unsigned long SOUND_COOLDOWN_MS = 3000; // Cooldown after detection

// ─── OLED ─────────────────────────────────────────────────────────────────────
Adafruit_SSD1306 display(128, 64, &Wire, -1);
unsigned long lastOLED = 0;
int oledPage = 0;

void oledTemp() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(15, 0);
  display.println("TEMP & HUMIDITY");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  if (!dhtValid) {
    display.setCursor(20, 30);
    display.println("** Sensor Error **");
  } else {
    display.setTextSize(2);
    display.setCursor(0, 14);
    display.print("T:");
    display.print(dhtTemp, 1);
    display.println("C");
    display.setCursor(0, 36);
    display.print("H:");
    display.print(dhtHum, 1);
    display.println("%");
  }
  display.setTextSize(1);
  display.setCursor(98, 0);
  display.println("[1/7]");
  display.display();
}

void oledGas() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("GAS SENSOR");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setTextSize(3);
  display.setCursor(10, 18);
  display.println(gasValue);
  int pct = constrain((int)((float)gasValue / 4095.0 * 100.0), 0, 100);
  display.drawRect(0, 44, 128, 10, SSD1306_WHITE);
  display.fillRect(2, 46, (int)(124.0 * pct / 100.0), 6, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.println(gasValue > GAS_THRESHOLD ? "!! HIGH GAS !!" : "Normal");
  display.setCursor(98, 0);
  display.println("[2/7]");
  display.display();
}

void oledFlame() {
  int pct = constrain((int)((float)(4095 - flameIntensity) / 4095.0 * 100.0), 0, 100);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("FLAME SENSOR");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setTextSize(3);
  display.setCursor(10, 18);
  display.print(pct);
  display.println("%");
  display.drawRect(0, 44, 128, 10, SSD1306_WHITE);
  display.fillRect(2, 46, (int)(124.0 * pct / 100.0), 6, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.println(flameDetected ? "!! FIRE !!" : "Clear");
  display.setCursor(98, 0);
  display.println("[3/7]");
  display.display();
}

void oledRain() {
  int pct = constrain((int)((float)(4095 - rainValue) / 4095.0 * 100.0), 0, 100);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("RAIN SENSOR");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setTextSize(3);
  display.setCursor(10, 18);
  display.print(pct);
  display.println("%");
  display.drawRect(0, 44, 128, 10, SSD1306_WHITE);
  display.fillRect(2, 46, (int)(124.0 * pct / 100.0), 6, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 56);
  display.println(rainDetected ? "!! RAINING !!" : "Dry");
  display.setCursor(98, 0);
  display.println("[4/7]");
  display.display();
}

void oledMotionDoor() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("MOTION - DOOR");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(10, 22);
  display.println(motionDoor ? "DETECTED!" : "Clear");
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.println(motionDoor ? ">> LED auto ON" : "No activity");
  display.setCursor(98, 0);
  display.println("[5/7]");
  display.display();
}

void oledMotionWindow() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("MOTION - WINDOW");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(10, 22);
  display.println(motionWindow ? "DETECTED!" : "Clear");
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.println(motionWindow ? ">> Check window!" : "Secure");
  display.setCursor(98, 0);
  display.println("[6/7]");
  display.display();
}

void oledSound() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("SOUND SENSOR");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(10, 22);
  display.println(soundDetected ? "GLASS BREAK!" : "Normal");
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.println(soundDetected ? ">> Window closed!" : "Monitoring...");
  display.setCursor(98, 0);
  display.println("[7/7]");
  display.display();
}

void updateOLED() {
  bool danger = (gasValue > GAS_THRESHOLD) || flameDetected || rainDetected || motionDoor || motionWindow || soundDetected;
  unsigned long interval = danger ? 500 : 5000;
  if (millis() - lastOLED < interval) return;
  lastOLED = millis();
  if (flameDetected) oledPage = 2;
  else if (gasValue > GAS_THRESHOLD) oledPage = 1;
  else if (rainDetected) oledPage = 3;
  else if (motionDoor) oledPage = 4;
  else if (motionWindow) oledPage = 5;
  else if (soundDetected) oledPage = 6;
  switch (oledPage) {
    case 0: oledTemp(); break;
    case 1: oledGas(); break;
    case 2: oledFlame(); break;
    case 3: oledRain(); break;
    case 4: oledMotionDoor(); break;
    case 5: oledMotionWindow(); break;
    case 6: oledSound(); break;
  }
  if (!danger) oledPage = (oledPage + 1) % 7;
}

// ─── LED ──────────────────────────────────────────────────────────────────────
void handleLedOn() {
  motionAutoLed = false;
  digitalWrite(LED_PIN, HIGH);
  server.send(200, "text/plain", "on");
}
void handleLedOff() {
  motionAutoLed = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "off");
}
void handleLedStatus() {
  server.send(200, "text/plain", digitalRead(LED_PIN) ? "on" : "off");
}

// ─── Door Servo ───────────────────────────────────────────────────────────────
Servo doorServo;
int curAngle = 110;
int targAngle = 110;
bool doorMoving = false;
bool doorAutoOpened = false;
unsigned long lastDoorStep = 0;
const unsigned long DOOR_STEP_MS = 12;

void handleDoorOpen() {
  if (!doorMoving) {
    targAngle = 0;
    doorMoving = true;
  }
  server.send(200, "text/plain", "opening");
}
void handleDoorClose() {
  if (!doorMoving) {
    targAngle = 110;
    doorMoving = true;
  }
  server.send(200, "text/plain", "closing");
}
void handleDoorStatus() {
  String s;
  if (doorMoving) s = (targAngle < curAngle) ? "opening" : "closing";
  else {
    if (curAngle == 0) s = "opened";
    else if (curAngle == 110) s = "closed";
    else s = "ajar";
  }
  server.send(200, "text/plain", s);
}

void updateDoor() {
  if (!doorMoving) return;
  if (millis() - lastDoorStep < DOOR_STEP_MS) return;
  lastDoorStep = millis();
  if (curAngle > targAngle) curAngle--;
  else if (curAngle < targAngle) curAngle++;
  doorServo.write(curAngle);
  if (curAngle == targAngle) doorMoving = false;
}

void checkFlameAutoDoor() {
  if (flameDetected && !doorMoving && curAngle > 55) {
    targAngle = 55;
    doorMoving = true;
    doorAutoOpened = true;
  }
  if (!flameDetected && doorAutoOpened && !doorMoving) {
    targAngle = 110;
    doorMoving = true;
    doorAutoOpened = false;
  }
}

// ─── Window Stepper ───────────────────────────────────────────────────────────
Stepper stepper(2048, IN1, IN3, IN2, IN4);
bool winOpen = false;
bool gasTrig = false;
bool winMoving = false;
int winSteps = 0;
int winDir = 0;
bool rainClosed = false;
bool userOverride = false;

void winStartOpen(bool gas = false) {
  winDir = -1;
  winSteps = 3 * 2048;
  winMoving = true;
  gasTrig = gas;
  soundAutoClosed = false;
}
void winStartClose(bool gas = false) {
  winDir = 1;
  winSteps = 3 * 2048;
  winMoving = true;
  gasTrig = gas;
}

void handleWinOpen() {
  if (!winOpen && !winMoving) {
    winStartOpen();
    if (rainDetected) userOverride = true;
    soundAutoClosed = false;
  }
  server.send(200, "text/plain", "opening");
}
void handleWinClose() {
  if (winOpen && !winMoving) {
    winStartClose();
    if (rainDetected) userOverride = true;
  }
  server.send(200, "text/plain", "closing");
}
void handleWinStatus() {
  String s;
  if (winMoving) s = (winDir == -1) ? "opening" : "closing";
  else s = winOpen ? "opened" : "closed";
  server.send(200, "text/plain", s);
}
void updateWindow() {
  if (!winMoving || winSteps <= 0) return;
  stepper.step(winDir);
  winSteps--;
  if (winSteps <= 0) {
    winMoving = false;
    winOpen = (winDir == -1);
  }
}

// ─── Gas ──────────────────────────────────────────────────────────────────────
unsigned long lastGas = 0;
void checkGas() {
  if (millis() - lastGas < 100) return;
  gasValue = analogRead(GAS_PIN);
  lastGas = millis();
  if (gasValue > GAS_THRESHOLD && !winOpen && !winMoving)
    winStartOpen(true);
  else if (gasValue <= GAS_THRESHOLD && winOpen && gasTrig && !winMoving) {
    winStartClose();
    gasTrig = false;
  }
}

// ─── Flame ────────────────────────────────────────────────────────────────────
unsigned long lastFlame = 0;
void readFlame() {
  if (millis() - lastFlame < 200) return;
  flameIntensity = analogRead(FLAME_PIN);
  flameDetected = (flameIntensity < FLAME_THRESHOLD);
  lastFlame = millis();
}
void handleFlameStatus() {
  server.send(200, "text/plain", flameDetected ? "detected" : "clear");
}

// ─── Rain ─────────────────────────────────────────────────────────────────────
unsigned long lastRain = 0;
void readRain() {
  if (millis() - lastRain < 500) return;
  rainValue = analogRead(RAIN_PIN);
  rainDetected = (rainValue < RAIN_THRESHOLD);
  lastRain = millis();
}
void checkRainCloseWindow() {
  if (rainDetected && winOpen && !winMoving && !rainClosed && !userOverride) {
    winStartClose();
    rainClosed = true;
  }
  if (!rainDetected) {
    rainClosed = false;
    userOverride = false;
  }
}
void handleRainStatus() {
  int pct = constrain((int)((float)(4095 - rainValue) / 4095.0 * 100.0), 0, 100);
  String j = "{\"rain\":" + String(rainDetected ? 1 : 0) + ",\"rainValue\":" + String(rainValue) + ",\"rainPct\":" + String(pct) + "}";
  server.send(200, "application/json", j);
}

// ─── Sound Sensor (Digital - FIXED) ───────────────────────────────────────────
unsigned long lastSoundRead = 0;

void readSound() {
  if (millis() - lastSoundRead < 50) return;  // Read every 50ms for faster response
  lastSoundRead = millis();

  bool newSoundDetected = digitalRead(SOUND_SENSOR_PIN) == HIGH;
  if (newSoundDetected && !soundDetected && (millis() - soundDebounceTime > SOUND_COOLDOWN_MS)) {
    soundDetected = true;
    soundDebounceTime = millis();
  
    
    // Auto-close window if it's open
    if (winOpen && !winMoving) {
      winStartClose();
      soundAutoClosed = true;
      Serial.println("Window closed due to glass break detection");
    } else if (winOpen && winMoving) {
      Serial.println("Window is already moving, cannot close");
    } else if (!winOpen) {
      Serial.println("Window already closed, no action needed");
    }
  }
  
  // Auto-reset sound detection after cooldown
  if (soundDetected && (millis() - soundDebounceTime > SOUND_COOLDOWN_MS)) {
    soundDetected = false;
    Serial.println("Sound detection cleared - system ready");
  }
}

void handleSoundStatus() {
  String j = "{\"sound\":" + String(soundDetected ? 1 : 0) + "}";
  server.send(200, "application/json", j);
}

// ─── PIR Motion Sensors ───────────────────────────────────────────────────────
unsigned long lastMotionRead = 0;
void readMotion() {
  if (millis() - lastMotionRead < 200) return;
  lastMotionRead = millis();

  bool prevDoor = motionDoor;
  motionDoor = !digitalRead(PIR_DOOR_PIN);

  bool prevWindow = motionWindow;
  motionWindow = !digitalRead(PIR_WINDOW_PIN);

  if (motionDoor && !prevDoor) {
    digitalWrite(LED_PIN, HIGH);
    motionAutoLed = true;
  }
  if (!motionDoor && prevDoor && motionAutoLed) {
    digitalWrite(LED_PIN, LOW);
    motionAutoLed = false;
  }

  if (motionWindow && !prevWindow) {
    Serial.println("⚠ Window motion detected!");
  }
}

void handleMotionDoorStatus() {
  server.send(200, "text/plain", motionDoor ? "detected" : "clear");
}

void handleMotionWindowStatus() {
  server.send(200, "text/plain", motionWindow ? "detected" : "clear");
}

// ─── DHT ──────────────────────────────────────────────────────────────────────
DHT dht(DHT_PIN, DHT_TYPE);
unsigned long lastDHT = 0;
void readDHT() {
  if (millis() - lastDHT < 2000) return;
  lastDHT = millis();
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t) && !isnan(h)) {
    dhtTemp = t;
    dhtHum = h;
    dhtValid = true;
  } else dhtValid = false;
}
void handleDHT() {
  String j = "{\"temperature\":" + (dhtValid ? String(dhtTemp, 1) : String("null")) + ",\"humidity\":" + (dhtValid ? String(dhtHum, 1) : String("null")) + ",\"valid\":" + (dhtValid ? "true" : "false") + "}";
  server.send(200, "application/json", j);
}

// ─── Buzzer ───────────────────────────────────────────────────────────────────
const int sirenPat[8][2] = { { 2, 100 }, { 3, 100 }, { 4, 150 }, { 5, 150 }, { 8, 200 }, { 10, 200 }, { 15, 300 }, { 20, 300 } };
bool buzOn = false, emergency = false;
int sirenStep = 0;
unsigned long lastBuzz = 0;
void updateBuzzer() {
  bool sound = (gasValue > GAS_THRESHOLD) || flameDetected || soundDetected;
  if (sound) {
    if (!emergency) {
      emergency = true;
      sirenStep = 0;
      lastBuzz = millis();
      buzOn = false;
    }
    if (millis() - lastBuzz >= (unsigned long)sirenPat[sirenStep][1]) {
      buzOn = !buzOn;
      digitalWrite(BUZZER_PIN, buzOn);
      sirenStep = (sirenStep + 1) % 8;
      lastBuzz = millis();
    }
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    buzOn = false;
    emergency = false;
    sirenStep = 0;
  }
}

// ─── Motor ────────────────────────────────────────────────────────────────────
int mSpeed = 0, mTarget = 0;
unsigned long lastMotor = 0;
void setDir(bool f) {
  digitalWrite(MOTOR_A, f ? HIGH : LOW);
  digitalWrite(MOTOR_B, f ? LOW : HIGH);
}
void handleMotorFwd() {
  setDir(true);
  mTarget = 255;
  server.send(200, "text/plain", "forward");
}
void handleMotorStop() {
  mTarget = 0;
  server.send(200, "text/plain", "stopped");
}
void handleMotorSpeed() {
  if (server.hasArg("speed")) {
    mTarget = constrain(server.arg("speed").toInt(), 0, 255);
    server.send(200, "text/plain", String(mTarget));
  } else server.send(200, "text/plain", String(mSpeed));
}
void handleMotorStatus() {
  server.send(200, "text/plain", String(mSpeed > 0 ? "forward" : "stopped") + ":" + String(mSpeed));
}
void updateMotor() {
  if (millis() - lastMotor < 20) return;
  if (mSpeed < mTarget) mSpeed++;
  else if (mSpeed > mTarget) mSpeed--;
  if (mSpeed == 0) {
    digitalWrite(MOTOR_A, LOW);
    digitalWrite(MOTOR_B, LOW);
  } else setDir(true);
  analogWrite(MOTOR_EN, mSpeed);
  lastMotor = millis();
}

// ─── Closing Actuator Servo Functions ─────────────────────────────────────────
void updateClosingActuator() {
  if (actuatorAutoMode) {
    if (rainDetected && currentActuatorAngle != 90) {
      currentActuatorAngle = 90;
      closingActuator.write(currentActuatorAngle);
      Serial.println("Actuator extended (rain detected)");
    } else if (!rainDetected && currentActuatorAngle != 0) {
      currentActuatorAngle = 0;
      closingActuator.write(currentActuatorAngle);
      Serial.println("Actuator retracted (no rain)");
    }
  }
}

void handleActuatorExtend() {
  actuatorAutoMode = false;
  currentActuatorAngle = 90;
  closingActuator.write(currentActuatorAngle);
  server.send(200, "text/plain", "extended");
  Serial.println("Actuator manually extended");
}

void handleActuatorRetract() {
  actuatorAutoMode = false;
  currentActuatorAngle = 0;
  closingActuator.write(currentActuatorAngle);
  server.send(200, "text/plain", "retracted");
  Serial.println("Actuator manually retracted");
}

void handleActuatorAutoEnable() {
  actuatorAutoMode = true;
  if (rainDetected && currentActuatorAngle != 90) {
    currentActuatorAngle = 90;
    closingActuator.write(currentActuatorAngle);
  } else if (!rainDetected && currentActuatorAngle != 0) {
    currentActuatorAngle = 0;
    closingActuator.write(currentActuatorAngle);
  }
  server.send(200, "text/plain", "auto_enabled");
  Serial.println("Actuator auto mode enabled");
}

void handleActuatorStatus() {
  String mode = actuatorAutoMode ? "auto" : "manual";
  String state = (currentActuatorAngle == 90) ? "extended" : "retracted";
  String response = "{\"state\":\"" + state + "\",\"mode\":\"" + mode + "\",\"angle\":" + String(currentActuatorAngle) + "}";
  server.send(200, "application/json", response);
}

void handleSensorData() {
  int rainPct = constrain((int)((float)(4095 - rainValue) / 4095.0 * 100.0), 0, 100);
  int soundPct = soundDetected ? 100 : 0;  // Simple: 100% when detected, 0% when clear
  String j = "{\"gas\":" + String(gasValue) + 
             ",\"flame\":" + String(flameDetected ? 1 : 0) + 
             ",\"flameIntensity\":" + String(flameIntensity) + 
             ",\"rain\":" + String(rainDetected ? 1 : 0) + 
             ",\"rainValue\":" + String(rainValue) + 
             ",\"rainPct\":" + String(rainPct) + 
             ",\"motionDoor\":" + String(motionDoor ? 1 : 0) + 
             ",\"motionWindow\":" + String(motionWindow ? 1 : 0) +
             ",\"sound\":" + String(soundDetected ? 1 : 0) +
             ",\"soundPct\":" + String(soundPct) + "}";
  server.send(200, "application/json", j);
}
void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}
void handleCSS() {
  server.send(200, "text/css", CSS_STYLES);
}

void handlePasswordSuccess() {
  if (!doorMoving && curAngle > 0) {
    targAngle = 0;
    doorMoving = true;
  }
  server.send(200, "application/json", "{\"status\":\"success\",\"action\":\"door_unlocked\"}");
}

void handlePasswordStatus() {
  server.send(200, "text/plain", "ready");
}

// ─── Setup ────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(2);
    display.setCursor(5, 10);
    display.println("   Home");
    display.setCursor(5, 32);
    display.println("Automation");
    display.setTextSize(1);
    display.setCursor(35, 54);
    display.println("Starting...");
    display.display();
    delay(2000);
  }

  doorServo.attach(DOOR_SERVO_PIN);
  doorServo.write(110);

  closingActuator.attach(CLOSING_ACTUATOR_PIN);
  closingActuator.write(0);
  currentActuatorAngle = 0;
  actuatorAutoMode = true;
  Serial.println("Closing Actuator initialized at 0° (retracted)");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  pinMode(MOTOR_EN, OUTPUT);
  pinMode(MOTOR_A, OUTPUT);
  digitalWrite(MOTOR_A, LOW);
  pinMode(MOTOR_B, OUTPUT);
  digitalWrite(MOTOR_B, LOW);
  analogWrite(MOTOR_EN, 0);
  pinMode(GAS_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(RAIN_PIN, INPUT);
  pinMode(PIR_DOOR_PIN, INPUT);
  pinMode(PIR_WINDOW_PIN, INPUT);
  pinMode(SOUND_SENSOR_PIN, INPUT_PULLDOWN);  // Digital input for sound sensor
  stepper.setSpeed(10);
  dht.begin();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println("Connecting WiFi...");
  display.display();

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nIP: " + WiFi.localIP().toString());

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(15, 15);
  display.println("WiFi Connected!");
  display.drawLine(0, 25, 127, 25, SSD1306_WHITE);
  display.setCursor(5, 35);
  display.println("IP:");
  display.setCursor(5, 46);
  display.println(WiFi.localIP().toString());
  display.display();
  delay(3000);

  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/on", handleLedOn);
  server.on("/off", handleLedOff);
  server.on("/Ledstatus", handleLedStatus);
  server.on("/openDoor", handleDoorOpen);
  server.on("/closeDoor", handleDoorClose);
  server.on("/Doorstatus", handleDoorStatus);
  server.on("/openWindow", handleWinOpen);
  server.on("/closeWindow", handleWinClose);
  server.on("/WindowStatus", handleWinStatus);
  server.on("/sensorData", handleSensorData);
  server.on("/flameStatus", handleFlameStatus);
  server.on("/rainStatus", handleRainStatus);
  server.on("/motionStatus", handleMotionDoorStatus);
  server.on("/motionDoorStatus", handleMotionDoorStatus);
  server.on("/motionWindowStatus", handleMotionWindowStatus);
  server.on("/dhtData", handleDHT);
  server.on("/motorForward", handleMotorFwd);
  server.on("/motorStop", handleMotorStop);
  server.on("/motorSpeed", handleMotorSpeed);
  server.on("/motorStatus", handleMotorStatus);
  server.on("/password_success", handlePasswordSuccess);
  server.on("/password_status", handlePasswordStatus);
  server.on("/actuatorExtend", handleActuatorExtend);
  server.on("/actuatorRetract", handleActuatorRetract);
  server.on("/actuatorAuto", handleActuatorAutoEnable);
  server.on("/actuatorStatus", handleActuatorStatus);
  server.on("/soundStatus", handleSoundStatus);

  server.begin();
  Serial.println("Server ready.");
  Serial.println("Door motion sensor on pin 34");
  Serial.println("Window motion sensor on pin 13");
  Serial.println("Sound sensor on pin 1 (DIGITAL - LED2 triggers detection)");
  Serial.println("Closing Actuator on pin 23 - auto mode: ON (rain will extend to 180°)");
  Serial.println("");
  Serial.println("=== SYSTEM READY ===");
  Serial.println("When you make a loud noise, LED2 on sensor will light up");
  Serial.println("This will trigger the web interface to show GLASS BREAK!");
}

// ─── Loop ─────────────────────────────────────────────────────────────────────
void loop() {
  server.handleClient();
  checkGas();
  readFlame();
  readRain();
  checkRainCloseWindow();
  readDHT();
  readMotion();
  readSound();      // Sound sensor reading
  updateOLED();
  updateWindow();
  updateDoor();
  checkFlameAutoDoor();
  updateBuzzer();
  updateMotor();
  updateClosingActuator();
}