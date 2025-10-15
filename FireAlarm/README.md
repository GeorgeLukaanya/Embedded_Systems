# FireAlarm

Compact Fire Alarm prototype using PlatformIO and an ESP-based microcontroller.

This repository contains firmware (PlatformIO) for a small fire/smoke alarm system. It reads one or more environmental sensors (smoke, flame, temperature), triggers a siren/buzzer and visual indicator, and exposes simple serial logs for debugging.

## What this repo contains

- `src/main.cpp` — main firmware. Edit pin definitions and thresholds here.
- `platformio.ini` — build configuration for PlatformIO (check environments and board type).
- `lib/` and `include/` — additional libraries and headers (if used).

## Features

- Reads smoke / flame / temperature sensors
- Activates a buzzer/siren and LED when an alarm condition is detected
- Serial debug output for development and troubleshooting

## Hardware (suggested)

The exact board and components depend on your setup. Typical components:

- ESP32 or ESP8266 development board (for example: ESP32-WROOM) — check `platformio.ini` for the configured board.
- Smoke sensor (e.g. MQ-2, MQ-135 or equivalent)
- (Optional) Flame sensor / IR sensor
- Buzzer or small siren (active or driven by transistor)
- Status LED (with resistor)
- Relay module (optional) if you need to switch mains-powered siren or other high-voltage equipment
- Wires, breadboard, and suitable power supply (5V/3.3V depending on sensors and board)

## Typical wiring / pinout

Update these to match your `src/main.cpp` pin definitions. The pins below are examples only:

- Smoke sensor analog output -> A0 (ADC pin on your board)
- Flame sensor digital out -> D2
- Buzzer (active) -> D5 (or a transistor driving the buzzer from a GPIO)
- Status LED -> D4 (through 220Ω resistor)
- Relay IN -> D12
- VCC / GND -> match sensor and board voltage requirements

Always confirm voltage levels: many sensors expect 5V while ESP boards are 3.3V. Use level shifting or run sensors at compatible voltages.

## Quick start — build & flash (PlatformIO)

Open the project in VS Code with the PlatformIO extension, or use the command line in the project root.

Build the firmware:

```zsh
pio run
```

Build and upload to the connected board (uses the default environment in `platformio.ini`):

```zsh
pio run -t upload
```

If you have multiple environments or a specific env name, pass `-e <env_name>`.

Open the serial monitor to view debug logs (common baud rate is 115200):

```zsh
pio device monitor -b 115200
```

If you're using VS Code, you can also use the PlatformIO GUI commands (Build / Upload / Monitor).

## Configuration

- Change pin assignments, thresholds and behavior in `src/main.cpp`. Search for clearly named constants like `SMOKE_PIN`, `BUZZER_PIN`, `ALARM_THRESHOLD`.
- Check and edit `platformio.ini` if you need to change board, upload speed, or environment settings.

## Testing

- With the board connected and serial monitor open, trigger the sensor (e.g., place a small amount of cigarette smoke or use a lighter briefly at a safe distance). The sensor reading should rise — observe serial logs to see values and whether the alarm triggers.
- For buzzer/LED testing, you can temporarily force the alarm state in `src/main.cpp` to validate wiring without triggering sensors.

# ESP32 Dual-Sensor Alert System

An ESP32-based dual-sensor alert system with Bluetooth monitoring, designed to detect fire and magnetic field events. The system features blinking fire LED with pulsing buzzer, magnetic alerts, and real-time status updates via Bluetooth.

---

## 🔥 Features

- Fire Detection
  - KY-026 flame sensor
  - Fire LED blinks in an alarm pattern
  - Buzzer pulses in sync with the LED

- Magnetic Field Detection
  - KY-035 Hall effect sensor
  - Magnetic alert LED lights up

- Bluetooth Control
  - Connect via classic Bluetooth (SPP)
  - Send commands:
    - `ack` → silence alarms
    - `status` → request current sensor readings
    - `cal` → recalibrate sensors
    - `reset` → re-enable alarms

- Real-time monitoring via Serial Monitor or Bluetooth-connected mobile app

---

## 🛠️ Hardware Required

- ESP32 WROOM-32
- KY-026 Flame Sensor
- KY-035 Hall Effect Sensor
- LEDs (Fire & Magnetic indicators)
- Buzzer
- Jumper wires and breadboard (or PCB for final design)

**Pin Configuration (as used in the code):**

| Component        | Pin on ESP32 |
| ---------------- | ------------ |
| Flame Digital    | 23           |
| Flame Analog     | 34           |
| Hall Analog      | 35           |
| Fire LED         | 5            |
| Magnetic LED     | 18           |
| Buzzer           | 19           |

> Note: These pin assignments match the repository's firmware by default. If your hardware differs, update the pins in `src/main.cpp`.

---

## 💻 Software & Libraries

- PlatformIO (recommended) or Arduino IDE
- ESP32 board support installed
- `BluetoothSerial.h` library (included with the ESP32 Arduino core)

This repository is set up for PlatformIO. If you prefer Arduino IDE, you can port `src/main.cpp` to an `.ino` file (example name used elsewhere: `ESP32_DualSensor_Alarm.ino`).

## Quick start — build & flash (PlatformIO)

Open the project in VS Code with the PlatformIO extension, or use the command line in the project root.

Build the firmware:

```zsh
pio run
```

Build and upload to the connected board (uses the default environment in `platformio.ini`):

```zsh
pio run -t upload
```

If you have multiple environments or a specific env name, pass `-e <env_name>`.

Open the serial monitor to view debug logs (common baud rate is 115200):

```zsh
pio device monitor -b 115200
```

### Quick start — Arduino IDE

1. Open `ESP32_DualSensor_Alarm.ino` (or port `src/main.cpp`) in Arduino IDE.
2. Select the correct ESP32 board and serial port.
3. Upload the sketch.
4. Open Serial Monitor at 115200 baud.

To use Bluetooth features, pair with the device name `ESP32_Security` (or the Bluetooth name set in your firmware).

---

## 📦 Setup & Usage

1. Connect your ESP32 board and sensors per the pin configuration above.
2. Build and upload firmware (PlatformIO or Arduino IDE).
3. Open Serial Monitor at 115200 baud to see sensor readings and status messages.
4. Pair a Bluetooth device (mobile app or PC) to `ESP32_Security` to receive real-time updates and send commands.

**Bluetooth Commands:**

| Command  | Description                        |
| -------- | ---------------------------------- |
| `ack`    | Silence all alarms (LEDs + buzzer) |
| `status` | Request current sensor readings    |
| `cal`    | Recalibrate all sensors            |
| `reset`  | Re-enable alarms after silence     |

---

## 🔧 How it Works

1. **Fire Detection**
   - Flame LED blinks and buzzer pulses when fire is detected.
   - The LED and buzzer stop when fire is cleared or system is silenced.

2. **Magnetic Detection**
   - Magnetic LED lights up when a strong magnetic field is detected.

3. **Sensor Calibration**
   - Automatically calibrates flame and magnetic sensors on startup or via `cal` command.

4. **Bluetooth Messaging**
   - Sends human-readable messages:
     - `🔥 FIRE DETECTED!`
     - `🧲 Magnetic field detected!`
     - `✅ NORMAL`
   - Can be parsed by a mobile app for real-time alerts and UI updates.

---

## 📱 Mobile App Integration

- App can connect to `ESP32_Security` via Bluetooth.
- Display real-time sensor status with animations:
  - Fire: blinking red background + alarm sound
  - Magnetic: blue pulse animation
  - Normal: calm green interface
- Send commands to silence or reset alarms

---

## 📌 Notes

- Adjust `flameThreshold` and `magneticThreshold` if your sensors behave differently.
- LED blink interval can be changed in the code (`blinkInterval` in `handleFireAlert()`).
- Ensure your ESP32 board supports Bluetooth SPP.
- Confirm sensor voltage requirements and use level shifting if mixing 3.3V and 5V components.

---

## Testing

- With the board connected and serial monitor open, trigger the flame sensor (carefully) and observe serial logs and LED/buzzer behavior.
- For magnetic testing, bring a magnet near the Hall effect sensor and observe the Magnetic LED and serial messages.
- Use the `status` Bluetooth command to get live readings from a paired device.

---

## Troubleshooting

- Upload fails:
  - Ensure the correct serial port is selected. Use `pio device list` or your OS device manager.
  - Install USB drivers if required (CP210x, CH34x, etc.).
  - Some boards need manual boot mode (hold BOOT while pressing EN) for flashing.

- No or garbled serial output:
  - Verify baud rate (115200) and correct port.

- Bluetooth pairing problems:
  - Ensure the ESP32 has `BluetoothSerial` initialized and the device name matches `ESP32_Security`.
  - On some phones re-pair after rebooting the ESP32.

- Inaccurate sensor readings:
  - Check wiring, power, and grounding.
  - Allow sensors to warm up if required by the datasheet.

---

## Development tips

- Add debug prints to `src/main.cpp` for raw sensor values and state transitions.
- Keep alarm handling non-blocking so Bluetooth/serial remain responsive.
- Consider adding OTA updates, watchdog timers, and persistent logging for production builds.

---

## Contributing

Contributions are welcome. Open issues for bugs or feature requests. Send pull requests with concise descriptions and test steps.

---

## 📝 License

This project is open-source under the MIT License. See the LICENSE file or add one to the repository.

---

## 🎯 Author

Created by **George Lukaanya** – ESP32 developer & embedded systems enthusiast.
