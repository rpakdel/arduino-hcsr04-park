// http://arduinobasics.blogspot.ca/2012/11/hc-sr04-ultrasonic-sensor.html

#include "LedControl_HW_SPI.h"
#include "nums.h"

const byte ALL_ON[][8] = {
    {
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111,
        B11111111
    } };

const byte STEADY_ON[][8] = {
    {
        B10000001,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B00000000,
        B10000001
    } };


#define ECHO_PIN 7 // Echo Pin
#define TRIG_PIN 8 // Trigger Pin

#define MAX_RANGE 200
#define MIN_RANGE 0

#define LED_ADDR 0
// | Arduino  | MAX7219 / MAX7221 |
// | -------- | ----------------- |
// | MOSI(11) | DIN (1)           |
// | SCK (13) | CLK (13)          |
// | I/O (10) | LOAD/CS (12)      |
#define LED_CS 10
#define MAX_DISPLAY_DISTANCE 150
#define FLASH_DISTANCE 60

#define MAX_LED_DISPLAY_MILLIS 30000
unsigned long ledDisplayOnMillis = 0;

#define LED_CS_PIN 10

LedControl_HW_SPI lc(LED_CS, 1);

void setup() 
{
    Serial.begin(115200);

    lc.shutdown(LED_ADDR, false);
    lc.setIntensity(LED_ADDR, 7);
    lc.clearDisplay(LED_ADDR);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

long getDistance()
{
    /* The following trigPin/echoPin cycle is used to determine the
    distance of the nearest object by bouncing soundwaves off of it. */
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);

    //Calculate the distance (in cm) based on the speed of sound.
    long distance = duration / 58.2;

    if (distance >= MAX_RANGE || distance <= MIN_RANGE)
    {
        return -1;
    }

    return distance;
}

void writeNumber(long dist)
{
    if (dist >= 0 && dist < IMAGES_LEN)
    {
        for (byte row = 0; row < 8; row++)
        {
            lc.setRow(0, row, IMAGES[dist][row]);
        }
    }
}

void showDistance(long dist)
{
    unsigned long m = millis();
    if (dist >= 0 && dist < MAX_DISPLAY_DISTANCE)
    {
        unsigned long ledDisplayMillis = m - ledDisplayOnMillis;
        // don't show flash/distance forever
        if (ledDisplayMillis < MAX_LED_DISPLAY_MILLIS)
        {
            if (dist < FLASH_DISTANCE)
            {
                lc.setIntensity(LED_ADDR, 3);
                flashDisplay();
            }
            else
            {
                lc.setIntensity(LED_ADDR, 10);
                long displayDistance = dist - FLASH_DISTANCE;
                writeNumber(displayDistance);
            }
        }
        else
        {
            // show steady on leds if we've shown distance/flash too long
            writeSteadyOn();
        }
    }
    else
    {
        ledDisplayOnMillis = m;
    }
}


bool flashOn = false;
void flashDisplay()
{
    if (flashOn)
    {
        flashOn = false;
    }
    else
    {
        flashOn = true;
        for (byte row = 0; row < 8; row++)
        {
            lc.setRow(0, row, ALL_ON[0][row]);
        }
    }
}

void writeSteadyOn()
{
    for (byte row = 0; row < 8; row++)
    {
        lc.setRow(0, row, STEADY_ON[0][row]);
    }
}


unsigned long prevMillis = 0;
void loop() 
{
    unsigned long m = millis();
    unsigned long diff = m - prevMillis;
    if (diff >= 200)
    {
        prevMillis = m;

        lc.shutdown(LED_ADDR, true);
        delay(10);

        long dist = getDistance();
        Serial.println(dist);

        if (dist >= 0)
        {
            lc.shutdown(LED_ADDR, false);
            delay(10);
        }
        showDistance(dist);
    }
}