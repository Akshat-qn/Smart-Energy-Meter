# ⚡ ESP32 Smart Energy Meter

An IoT-based Smart Energy Meter built using ESP32, Blynk IoT, and Telegram Bot integration.

This project can:
- Monitor current, power, energy usage, and electricity bill
- Control electrical loads using a relay
- Send live updates to Blynk Dashboard
- Send alerts and status messages through Telegram
- Provide overload protection
- Automatically reconnect WiFi

---

# 🚀 Features

✅ Real-time Energy Monitoring  
✅ Blynk IoT Dashboard  
✅ Telegram Bot Commands  
✅ Relay ON/OFF Control  
✅ Electricity Bill Estimation  
✅ Overload Protection  
✅ WiFi Auto Reconnect  
✅ Non-Blocking Timer Architecture  
✅ ESP32 Based IoT System  

---

# 🛠 Hardware Required

| Component | Quantity |
|-----------|----------|
| ESP32 Dev Board | 1 |
| Relay Module | 1 |
| ACS712 / PZEM004T Sensor (Optional) | 1 |
| Jumper Wires | Few |
| Breadboard | 1 |
| WiFi Connection | Required |

---

# 📲 Blynk Virtual Pins

| Virtual Pin | Function |
|-------------|----------|
| V0 | Current |
| V1 | Power |
| V2 | Energy Units |
| V3 | Electricity Bill |
| V4 | Relay Control |

---

# 🤖 Telegram Commands

| Command | Action |
|---------|--------|
| /start | Show all commands |
| /status | Show current readings |
| /on | Turn relay ON |
| /off | Turn relay OFF |
| /bill | Show electricity bill |
| /reset | Reset energy data |

---

# 🔌 Circuit Connections

| ESP32 Pin | Component |
|-----------|-----------|
| GPIO 26 | Relay IN |
| GND | Relay GND |
| 5V / VIN | Relay VCC |

---

# 📈 Energy Calculation Formula

Energy calculation used:

Energy (kWh) = Power(W) × Time(hours) / 1000

---

# ⚠ Current Status

This project currently uses simulated sensor data using random values.

Replace the demo code with:
- ACS712
- PZEM004T
- INA219

for real-world energy monitoring.

---

# 🧠 Future Improvements

- EEPROM/Preferences storage
- OLED display support
- Firebase database logging
- MQTT support
- OTA updates
- Real sensor integration
- Mobile app analytics

---

# 📷 Project Screenshots

Add screenshots here:
- Blynk Dashboard
- Serial Monitor
- Hardware Setup
- Telegram Bot

---

# 🔒 Security Note

Do NOT upload:
- WiFi passwords
- Telegram bot token
- Blynk auth token

Replace them with placeholders before pushing to GitHub.

---

# 👨‍💻 Developed By

Akshat Singh

ESP32 + IoT + Embedded Systems Project
