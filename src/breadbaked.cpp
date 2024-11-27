/*
 * Project Breadbaked
 * Author: Jon Jaques
 * Date: 11/26/2024
 */

#include "Particle.h"
#include "../lib/LiquidCrystal/src/LiquidCrystal.h"
#include "breadbaked.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(SEMI_AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

// LCD Pinout
// ==========
int dE = D0;
int dRS = D1;

int dD0 = D2;
int dD1 = D3;
int dD2 = D4;
int dD3 = D5;

/**
 * LiquidCrystal::LiquidCrystal(uint8_t rs,  uint8_t enable,
 *      uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
 */
LiquidCrystal lcd(dRS, dE, dD0, dD1, dD2, dD3);

const long vitalsInterval = 60 * 15; // 15 mins (in sec)
const long blinkInterval = 5 * 1000; // 5 sec

// Onboard LED
int led1 = D7;

unsigned long blinkPrevMillis = 0;
int blinkState = 0;

void setup()
{
    // cloud registration
    Particle.function("updateMessage", updateMessage);
    Particle.publishVitals(vitalsInterval);

    // LED setup
    pinMode(led1, OUTPUT);
    ledToggle("off");

    // LCD setup
    updateMessage("Hello World");

    // Connect to the Particle Cloud
    Particle.connect();
}

void loop()
{
}

void blinkLed(int times, int interval)
{
    for (int i = 0; i < times; i++)
    {
        ledToggle("on");
        delay(interval);
        ledToggle("off");
        delay(interval);
    }
}

int ledToggle(String command)
{
    if (command == "on")
    {
        digitalWrite(led1, HIGH);
        return 1;
    }
    else if (command == "off")
    {
        digitalWrite(led1, LOW);
        return 0;
    }
    else
    {
        return -1;
    }
}

int updateMessage(String message)
{
    blinkLed(5, 100);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
    return 1;
}