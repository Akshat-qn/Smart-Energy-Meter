#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart Meter"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_AUTH_TOKEN"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>
#include <UniversalTelegramBot.h>

// ======================================================
// WIFI CREDENTIALS
// ======================================================

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// ======================================================
// TELEGRAM SETTINGS
// ======================================================

#define BOT_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
#define CHAT_ID "YOUR_CHAT_ID"

// ======================================================
// HARDWARE PINS
// ======================================================

#define RELAY_PIN 26

// ======================================================
// OBJECTS
// ======================================================

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

BlynkTimer timer;

// ======================================================
// VARIABLES
// ======================================================

float voltageVal = 230.0;
float currentVal = 0.0;
float powerVal = 0.0;

float energyUnits = 0.0;
float electricityBill = 0.0;

float electricityRate = 8.0; // Rs per kWh

bool overloadTriggered = false;

// ======================================================
// FUNCTION: GENERATE SENSOR DATA
// Replace this section with REAL SENSOR CODE
// ======================================================

void readSensorData()
{
  // Demo current values (1A to 5A)
  currentVal = random(10, 50) / 10.0;

  // Power Formula
  powerVal = voltageVal * currentVal;

  /*
    Energy Formula:

    Energy (kWh) =
    Power(W) × Time(hours) / 1000

    Timer Interval = 2 seconds

    So:
    units += (power × 2 sec) / 3600000
  */

  energyUnits += (powerVal * 2.0) / 3600000.0;

  // Bill Calculation
  electricityBill = energyUnits * electricityRate;
}

// ======================================================
// FUNCTION: SEND DATA TO BLYNK
// ======================================================

void updateBlynk()
{
  Blynk.virtualWrite(V0, currentVal);
  Blynk.virtualWrite(V1, powerVal);
  Blynk.virtualWrite(V2, energyUnits);
  Blynk.virtualWrite(V3, electricityBill);
}

// ======================================================
// FUNCTION: OVERLOAD PROTECTION
// ======================================================

void checkOverload()
{
  // Cutoff if current exceeds 5A

  if (currentVal > 5.0)
  {
    digitalWrite(RELAY_PIN, HIGH);

    if (!overloadTriggered)
    {
      overloadTriggered = true;

      bot.sendMessage(
        CHAT_ID,
        "⚠ OVERLOAD DETECTED\nRelay Turned OFF",
        ""
      );

      Blynk.virtualWrite(V4, 0);
    }
  }
  else
  {
    overloadTriggered = false;
  }
}

// ======================================================
// FUNCTION: SEND PERIODIC TELEGRAM STATUS
// ======================================================

void sendTelegramStatus()
{
  String msg = "⚡ Smart Meter Update\n\n";

  msg += "Current : ";
  msg += String(currentVal, 2);
  msg += " A\n";

  msg += "Power : ";
  msg += String(powerVal, 2);
  msg += " W\n";

  msg += "Units : ";
  msg += String(energyUnits, 4);
  msg += " kWh\n";

  msg += "Bill : ₹";
  msg += String(electricityBill, 2);

  bot.sendMessage(CHAT_ID, msg, "");
}

// ======================================================
// FUNCTION: HANDLE TELEGRAM COMMANDS
// ======================================================

void handleTelegramCommands()
{
  int newMessages =
    bot.getUpdates(bot.last_message_received + 1);

  while (newMessages)
  {
    for (int i = 0; i < newMessages; i++)
    {
      String text = bot.messages[i].text;

      // ==========================================
      // START
      // ==========================================

      if (text == "/start")
      {
        String help = "🤖 ESP32 Smart Meter\n\n";

        help += "/status - Show Readings\n";
        help += "/on - Relay ON\n";
        help += "/off - Relay OFF\n";
        help += "/bill - Show Bill\n";
        help += "/reset - Reset Units";

        bot.sendMessage(CHAT_ID, help, "");
      }

      // ==========================================
      // STATUS
      // ==========================================

      else if (text == "/status")
      {
        sendTelegramStatus();
      }

      // ==========================================
      // RELAY ON
      // ==========================================

      else if (text == "/on")
      {
        digitalWrite(RELAY_PIN, LOW);

        Blynk.virtualWrite(V4, 1);

        bot.sendMessage(
          CHAT_ID,
          "⚡ Relay Turned ON",
          ""
        );
      }

      // ==========================================
      // RELAY OFF
      // ==========================================

      else if (text == "/off")
      {
        digitalWrite(RELAY_PIN, HIGH);

        Blynk.virtualWrite(V4, 0);

        bot.sendMessage(
          CHAT_ID,
          "❌ Relay Turned OFF",
          ""
        );
      }

      // ==========================================
      // BILL
      // ==========================================

      else if (text == "/bill")
      {
        String msg = "💰 Estimated Bill : ₹";
        msg += String(electricityBill, 2);

        bot.sendMessage(CHAT_ID, msg, "");
      }

      // ==========================================
      // RESET ENERGY
      // ==========================================

      else if (text == "/reset")
      {
        energyUnits = 0;
        electricityBill = 0;

        bot.sendMessage(
          CHAT_ID,
          "🔄 Energy Data Reset",
          ""
        );
      }
    }

    newMessages =
      bot.getUpdates(bot.last_message_received + 1);
  }
}

// ======================================================
// BLYNK RELAY CONTROL
// ======================================================

BLYNK_WRITE(V4)
{
  int state = param.asInt();

  digitalWrite(RELAY_PIN, !state);

  if (state == 1)
  {
    bot.sendMessage(
      CHAT_ID,
      "⚡ Relay Turned ON From Blynk",
      ""
    );
  }
  else
  {
    bot.sendMessage(
      CHAT_ID,
      "❌ Relay Turned OFF From Blynk",
      ""
    );
  }
}

// ======================================================
// FUNCTION: WIFI RECONNECT
// ======================================================

void checkWiFi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Reconnecting WiFi...");

    WiFi.disconnect();
    WiFi.begin(ssid, pass);

    int retry = 0;

    while (WiFi.status() != WL_CONNECTED &&
           retry < 20)
    {
      delay(500);
      Serial.print(".");
      retry++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nWiFi Reconnected");
    }
  }
}

// ======================================================
// SETUP
// ======================================================

void setup()
{
  Serial.begin(115200);

  // Relay Setup

  pinMode(RELAY_PIN, OUTPUT);

  // Relay OFF initially

  digitalWrite(RELAY_PIN, HIGH);

  // ==========================================
  // CONNECT WIFI
  // ==========================================

  Serial.println("\nConnecting WiFi...");

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");

  // ==========================================
  // TELEGRAM SSL
  // ==========================================

  client.setInsecure();

  // ==========================================
  // BLYNK CONNECT
  // ==========================================

  Blynk.begin(
    BLYNK_AUTH_TOKEN,
    ssid,
    pass
  );

  // ==========================================
  // STARTUP MESSAGE
  // ==========================================

  bot.sendMessage(
    CHAT_ID,
    "🔥 ESP32 Smart Meter Online",
    ""
  );

  // ==========================================
  // TIMERS
  // ==========================================

  // Every 2 sec -> Read Sensor
  timer.setInterval(2000L, readSensorData);

  // Every 2 sec -> Update Blynk
  timer.setInterval(2000L, updateBlynk);

  // Every 5 sec -> Telegram Commands
  timer.setInterval(5000L, handleTelegramCommands);

  // Every 30 sec -> Auto Status
  timer.setInterval(30000L, sendTelegramStatus);

  // Every 10 sec -> WiFi Check
  timer.setInterval(10000L, checkWiFi);

  // Every 2 sec -> Overload Check
  timer.setInterval(2000L, checkOverload);
}

// ======================================================
// LOOP
// ======================================================

void loop()
{
  Blynk.run();
  timer.run();
}
