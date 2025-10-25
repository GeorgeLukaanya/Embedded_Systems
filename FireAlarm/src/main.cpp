#include <Arduino.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// 🔹 Pin Definitions
#define FLAME_DIGITAL_PIN 23
#define FLAME_ANALOG_PIN  34
#define HALL_ANALOG_PIN   35
#define FIRE_LED_PIN      5
#define MAGNETIC_LED_PIN  18
#define BUZZER_PIN        19

// 🔹 Variables
int flameAmbient = 0;
int flameThreshold = 0;
int magneticBaseline = 0;
int magneticThreshold = 0;

bool fireDetected = false;
bool magneticDetected = false;
bool systemSilenced = false;

unsigned long lastSend = 0;
const unsigned long sendInterval = 1000;
unsigned long lastCalib = 0;
const unsigned long recalibInterval = 300000;  // 5 minutes

// 🔹 Noise filtering
int flameStableCount = 0;
const int requiredStableReadings = 5;  // consecutive confirms

// 🔹 Setup pins
void setupPins() {
  pinMode(FLAME_DIGITAL_PIN, INPUT);
  pinMode(FIRE_LED_PIN, OUTPUT);
  pinMode(MAGNETIC_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(FIRE_LED_PIN, LOW);
  digitalWrite(MAGNETIC_LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

// 🔹 Calibration
void calibrateSensors() {
  const int samples = 50;
  long sumFl = 0, sumMag = 0;

  Serial.println(">> Calibrating sensors...");

  for (int i = 0; i < samples; i++) {
    sumFl += analogRead(FLAME_ANALOG_PIN);
    sumMag += analogRead(HALL_ANALOG_PIN);
    delay(20);
  }

  flameAmbient = sumFl / samples;
  magneticBaseline = sumMag / samples;

  // 🔸 Adjusted sensitivity: less affected by bright light
  flameThreshold = max(50, flameAmbient - 100);
  magneticThreshold = min(4095, magneticBaseline + 300);

  Serial.println(">> Calibration complete!");
  Serial.print("Flame baseline: "); Serial.println(flameAmbient);
  Serial.print("Magnetic baseline: "); Serial.println(magneticBaseline);
}

// 🔹 Fire alert blinking and buzzer
void handleFireAlert(bool on) {
  static unsigned long lastBlink = 0;
  static bool ledState = false;
  const unsigned long blinkInterval = 500;

  if (on && !systemSilenced) {
    unsigned long now = millis();
    if (now - lastBlink >= blinkInterval) {
      ledState = !ledState;
      digitalWrite(FIRE_LED_PIN, ledState);
      if (ledState)
        tone(BUZZER_PIN, 1000);
      else
        noTone(BUZZER_PIN);
      lastBlink = now;
    }
  } else {
    digitalWrite(FIRE_LED_PIN, LOW);
    noTone(BUZZER_PIN);
    lastBlink = millis();
    ledState = false;
  }
}

// 🔹 Magnetic alert
void handleMagneticAlert(bool on) {
  if (on && !systemSilenced)
    digitalWrite(MAGNETIC_LED_PIN, HIGH);
  else
    digitalWrite(MAGNETIC_LED_PIN, LOW);
}

// 🔹 Status message
void sendStatus() {
  String status = "STATUS: ";
  if (fireDetected)
    status += "🔥 FIRE ALERT ";
  else if (magneticDetected)
    status += "🧲 MAGNETIC ALERT ";
  else
    status += "✅ NORMAL ";

  status += "| Flame: " + String(analogRead(FLAME_ANALOG_PIN));
  status += " | Magnetic: " + String(analogRead(HALL_ANALOG_PIN));

  Serial.println(status);
  if (SerialBT.connected()) SerialBT.println(status);
}

// 🔹 Bluetooth commands
void handleCommand(String cmd) {
  cmd.trim(); cmd.toLowerCase();

  if (cmd == "ack") {
    systemSilenced = true;
    noTone(BUZZER_PIN);
    digitalWrite(FIRE_LED_PIN, LOW);
    digitalWrite(MAGNETIC_LED_PIN, LOW);
    Serial.println(">> Alerts silenced");
    if (SerialBT.connected()) SerialBT.println(">> Alerts silenced");
  } 
  else if (cmd == "status") {
    sendStatus();
  } 
  else if (cmd == "cal") {
    calibrateSensors();
  } 
  else if (cmd == "reset") {
    systemSilenced = false;
    Serial.println(">> System reset");
    if (SerialBT.connected()) SerialBT.println(">> System reset");
  } 
  else {
    Serial.println(">> Unknown command: ack | status | cal | reset");
  }
}

// 🔹 Setup
void setup() {
  Serial.begin(115200);
  setupPins();

  Serial.println("====================================");
  Serial.println("ESP32 Dual Protection System (Refined Flame Stability)");
  Serial.println("====================================");

  if (!SerialBT.begin("ESP32_Security")) {
    Serial.println(">> Bluetooth init failed!");
  } else {
    Serial.println(">> Bluetooth Ready: ESP32_Security");
  }

  calibrateSensors();
  lastCalib = millis();
}

// 🔹 Main loop
void loop() {
  int dFlame = digitalRead(FLAME_DIGITAL_PIN);
  int aFlame = analogRead(FLAME_ANALOG_PIN);
  int aMag = analogRead(HALL_ANALOG_PIN);

  bool fireNow = (dFlame == HIGH || aFlame < flameThreshold);
  bool magneticNow = (aMag > magneticThreshold);

  // 🔸 Flame noise filtering (requires consecutive readings)
  if (fireNow) {
    if (flameStableCount < requiredStableReadings) flameStableCount++;
  } else {
    flameStableCount = 0;
  }

  bool fireConfirmed = (flameStableCount >= requiredStableReadings);

  // 🔸 Fire detection logic
  if (fireConfirmed && !fireDetected) {
    fireDetected = true;
    Serial.println("🔥 FIRE DETECTED!");
    if (SerialBT.connected()) SerialBT.println("🔥 FIRE DETECTED!");
  } else if (!fireConfirmed && fireDetected) {
    fireDetected = false;
    Serial.println("🔥 Fire cleared.");
    if (SerialBT.connected()) SerialBT.println("🔥 Fire cleared.");
  }

  // 🔸 Magnetic detection
  if (magneticNow && !magneticDetected) {
    magneticDetected = true;
    Serial.println("🧲 Magnetic field detected!");
    if (SerialBT.connected()) SerialBT.println("🧲 Magnetic field detected!");
  } else if (!magneticNow && magneticDetected) {
    magneticDetected = false;
    Serial.println("🧲 Magnetic field cleared.");
    if (SerialBT.connected()) SerialBT.println("🧲 Magnetic field cleared.");
  }

  // 🔸 Alert handlers
  handleFireAlert(fireDetected);
  handleMagneticAlert(magneticDetected);

  // 🔸 Auto recalibration (every 5 minutes)
  if (millis() - lastCalib > recalibInterval) {
    Serial.println(">> Auto recalibration triggered...");
    calibrateSensors();
    lastCalib = millis();
  }

  // 🔸 Status update
  if (millis() - lastSend > sendInterval) {
    sendStatus();
    lastSend = millis();
  }

  // 🔸 Bluetooth or Serial commands
  if (SerialBT.available()) handleCommand(SerialBT.readStringUntil('\n'));
  if (Serial.available()) handleCommand(Serial.readStringUntil('\n'));

  delay(50);
}
