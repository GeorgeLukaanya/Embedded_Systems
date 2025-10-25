# 🔥 FireAlarm — ESP32 Dual-Sensor Alert System

A **compact fire and magnetic alert system** built on **PlatformIO** and **ESP32**.
This firmware reads **flame (KY-026)** and **magnetic (KY-035)** sensors, triggers an **LED and buzzer alarm**, and supports **Bluetooth-based monitoring and control**.

---

## 🧭 Overview

This project demonstrates how to build a **dual-sensor emergency alert system** capable of detecting fire and magnetic field events.
It can be integrated into **safety doors, smart alarms, or IoT-based monitoring systems**.

When a fire is detected, the system:

* Blinks a red LED and pulses the buzzer.
* Sends alerts via Serial Monitor and Bluetooth.
* Allows remote acknowledgment or reset through Bluetooth commands.

---

## 📂 Repository Contents

| File/Folder           | Description                                                            |
| --------------------- | ---------------------------------------------------------------------- |
| `src/main.cpp`        | Main firmware — edit pin definitions, thresholds, and logic here       |
| `platformio.ini`      | PlatformIO build configuration (defines board, environment, libraries) |
| `lib/` and `include/` | Optional custom libraries and header files                             |
| `README.md`           | Project documentation (this file)                                      |

---

## ✨ Features

* 🔥 **Flame Detection** (KY-026 Sensor)
  Detects infrared light from fire sources. Blinks LED and activates buzzer.

* 🧲 **Magnetic Field Detection** (KY-035 Sensor)
  Detects the presence of magnetic fields (door lock or contact sensors).

* 📶 **Bluetooth Monitoring**
  Connect via classic Bluetooth SPP (`ESP32_Security`) to:

  * Receive real-time alerts.
  * Send commands: `ack`, `status`, `cal`, `reset`.

* 💡 **LED Indicators**
  Separate LEDs for fire and magnetic alerts.

* 🧠 **Non-blocking alarm handling**
  Keeps Bluetooth and Serial responsive during alerts.

---

## 🛠️ Hardware Setup

| Component                    | ESP32 Pin | Description                            |
| ---------------------------- | --------- | -------------------------------------- |
| Flame Sensor (Digital Out)   | 23        | Detects fire (LOW when flame detected) |
| Flame Sensor (Analog Out)    | 34        | Measures flame intensity               |
| Magnetic Sensor (Analog Out) | 35        | Detects magnetic field strength        |
| Fire LED                     | 5         | Indicates flame alert                  |
| Magnetic LED                 | 18        | Indicates magnetic field presence      |
| Buzzer                       | 19        | Sound alarm output                     |

> ⚡ Ensure all components are compatible with ESP32’s 3.3V logic.
> The KY-026 and KY-035 can run on 3.3V or 5V depending on model — verify your module version.

---

## 🔧 Adjusting Sensitivity

### KY-026 Flame Sensor

* **Clockwise** rotation on the onboard potentiometer → Increases sensitivity.
* **Counterclockwise** → Decreases sensitivity.
* Use digital output for ON/OFF flame detection or analog for flame intensity.

### KY-035 Hall Effect Sensor

* Calibrate the analog threshold in code using:

  ```cpp
  int magneticThreshold = 1800; // Adjust based on your sensor’s readings
  ```
* Use a magnet to test detection distance and responsiveness.

---

## ⚙️ Quick Start — PlatformIO

1. **Open project** in VS Code with the **PlatformIO extension**.
2. **Build** the firmware:

   ```bash
   pio run
   ```
3. **Upload** to your ESP32 board:

   ```bash
   pio run -t upload
   ```
4. **Monitor serial output:**

   ```bash
   pio device monitor -b 115200
   ```

> You can also use the PlatformIO GUI (Build / Upload / Monitor buttons).

---

## 🧠 How It Works

1. The **flame sensor** outputs a LOW signal when infrared light from fire is detected.
   The system blinks the LED and pulses the buzzer.

2. The **magnetic sensor** outputs analog readings depending on magnetic field strength.
   When readings exceed the set threshold, the magnetic LED turns ON.

3. **Bluetooth Control**:
   The ESP32 initializes a Bluetooth SPP connection (`ESP32_Security`), allowing command-based control.

### Supported Bluetooth Commands

| Command  | Description                           |
| -------- | ------------------------------------- |
| `ack`    | Silence all alarms (LED + buzzer)     |
| `status` | Report current sensor values          |
| `cal`    | Recalibrate sensors                   |
| `reset`  | Re-enable alarms after being silenced |

---

## 🔍 Serial & Bluetooth Messages

* `🔥 FIRE DETECTED!`
* `🧲 Magnetic field detected!`
* `✅ System Normal`
* `🔇 Alarm silenced`
* `♻️ System reset`

All messages are visible both via **Serial Monitor** and **Bluetooth terminal**.

---

## 📱 Mobile App Integration (Optional)

A simple Bluetooth terminal app (e.g., **Serial Bluetooth Terminal** for Android) can:

* Display system messages in real-time.
* Send commands to control the ESP32 remotely.
* Integrate easily with custom mobile dashboards.

---

## 🧪 Testing Procedure

1. **Flame Sensor Test**
   Bring a lighter flame near the sensor (without touching it).
   The **Fire LED** and **buzzer** should trigger, and “🔥 FIRE DETECTED!” should appear in Serial Monitor.

2. **Magnetic Sensor Test**
   Bring a magnet near the KY-035 sensor.
   The **Magnetic LED** should turn ON and “🧲 Magnetic field detected!” should appear.

3. **Bluetooth Test**
   Pair your phone with `ESP32_Security`, open a Bluetooth terminal, and send:

   * `status` → Shows sensor data.
   * `ack` → Silences alarms.
   * `reset` → Re-enables them.

---

## 🧰 Troubleshooting

| Issue                   | Possible Cause                                | Fix                                                         |
| ----------------------- | --------------------------------------------- | ----------------------------------------------------------- |
| Upload fails            | Wrong serial port / missing drivers           | Check `pio device list` and install CP210x or CH34x drivers |
| No serial output        | Wrong baud rate                               | Use `115200`                                                |
| Bluetooth not visible   | Bluetooth not initialized or disabled in code | Ensure `BluetoothSerial.begin("ESP32_Security");` exists    |
| False fire triggers     | Sensor too sensitive                          | Adjust KY-026 potentiometer or code threshold               |
| Weak magnetic detection | Sensor too far / wrong polarity               | Move magnet closer or recalibrate threshold                 |

---

## 💡 Development Tips

* Use `Serial.print()` to debug sensor raw values.
* Keep alarm loops non-blocking with `millis()` timing.
* Consider adding:

  * OTA updates
  * EEPROM logging for alarm history
  * WiFi support for IoT monitoring

---

## 🤝 Contributing

Contributions are welcome!
Open an issue for bugs or suggestions, and submit pull requests with clear commit messages.

---

## 🧾 License

This project is open-source under the **MIT License**.
See the LICENSE file for details.

---

## 👨‍💻 Author

**George Lukaanya**
Embedded Systems & IoT Developer
*Exploring AI-driven embedded intelligence and safety systems.*

