/*
 * Project myProject
 * Author: Your Name
 * Date:
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"

// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

const long vitalsInterval = 60 * 15; // 15 mins
const long blinkInterval = 5 * 1000; // 5 sec
const long potInterval = 5 * 1000;   // 5 sec

unsigned long blinkPrevMillis = 0;
unsigned long potPrevMillis = 0;

int potPin = A0;
int led1 = D7;
int led2 = D6;
int blinkState = 0;
int potValue;

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

void setup()
{
    // setup
    pinMode(potPin, INPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);

    // initial state
    ledToggle("off");

    // cloud registration
    Particle.function("led", ledToggle);
    Particle.publishVitals(vitalsInterval);
}

void loop()
{
    unsigned long currentMillis = millis();

    // blink onboard LED
    if (currentMillis - blinkPrevMillis >= blinkInterval)
    {
        blinkPrevMillis = currentMillis;
        if (blinkState == 0)
        {
            blinkState = ledToggle("on");
        }
        else
        {
            blinkState = ledToggle("off");
        }
    }

    // read potentiometer (0-4095)
    int currPotValue = analogRead(potPin);

    // write it to the LED
    if (currPotValue > 20)
    {
        analogWrite(led2, currPotValue / 4);
    }
    else
    {
        analogWrite(led2, 0);
    }

    // publish potentiometer value
    if (currentMillis - potPrevMillis >= potInterval)
    {
        potPrevMillis = currentMillis;
        if (currPotValue != potValue)
        {
            potValue = currPotValue;
            Particle.publish("test/pot", String(potValue));
        }
    }
}
