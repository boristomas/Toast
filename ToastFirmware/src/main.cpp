#include <Arduino.h>
#include "SparkFunMMA8452Q.h"
#include "BluetoothSerial.h"

int freq = 5000;
int LedRChannel = 0;
int LedGChannel = 1;
int LedBChannel = 2;
int LedRPin = GPIO_NUM_12;
int LedGPin = GPIO_NUM_27;
int LedBPin = GPIO_NUM_33;
int resolution = 8;

bool hasBTclient = false;

BluetoothSerial SerialBT;

// Create an MMA8452Q object, used throughout the rest of the sketch.
MMA8452Q accel; // Default constructor, SA0 pin is HIGH

// The above works if the MMA8452Q's address select pin (SA0) is high.
// If SA0 is low (if the jumper on the back of the SparkFun MMA8452Q breakout
// board is closed), initialize it like this:
// MMA8452Q accel(MMA8452Q_ADD_SA0_);

// printAccelGraph prints a simple ASCII bar graph for a single accelerometer axis value.
// Examples:
//	printAccelGraph(-0.1, "X", 20, 2.0) will print:
// 		X:                    =|                     (0.1 g)
//	printAccelGraph(1.0, "Z", 20, 2.0) will print:
//		Z:                     |==========           (1.0 g)
// Input:
//	- [value]: calculated value of an accelerometer axis (e.g accel.cx, accel.cy)
//	- [name]: name of the axis (e.g. "X", "Y", "Z")
//	- [numBarsFull]: Maximum number of bars either right or left of 0 point.
//	- [rangeAbs]: Absolute value of the maximum acceleration range
void printAccelGraph(float value, String name, int numBarsFull, float rangeAbs)
{
    // Calculate the number of bars to fill, ignoring the sign of numBars for now.
    int numBars = abs(value / (rangeAbs / numBarsFull));

    Serial.print(name + ": "); // Print the axis name and a colon:

    // Do the negative half of the graph first:
    for (int i = 0; i < numBarsFull; i++)
    {
        if (value < 0) // If the value is negative
        {
            // If our position in the graph is in the range we want to graph
            if (i >= (numBarsFull - numBars))
                Serial.print('='); // Print an '='
            else
                Serial.print(' '); // print spaces otherwise
        }
        else // If our value is positive, just print spaces
            Serial.print(' ');
    }

    Serial.print('|'); // Print a pipe (|) to represent the 0-point

    // Do the positive half of the graph last:
    for (int i = 0; i < numBarsFull; i++)
    {
        if (value > 0)
        { // If our position in the graph is in the range we want to graph
            if (i <= numBars)
                Serial.print('='); // Print an '='
            else
                Serial.print(' '); // otherwise print spaces
        }
        else // If value is negative, just print spaces
            Serial.print(' ');
    }

    // To end the line, print the actual value:
    Serial.println(" (" + String(value, 2) + " g)");
}

void setLed(byte r, byte g, byte b, int blink, int delayOn, int delayOff)
{
    if(blink == 0)
    {
        //TOOD: optimize this
        ledcWrite(LedRChannel, r);
        ledcWrite(LedGChannel, g);
        ledcWrite(LedBChannel, b);
        return;
    }
    for(int i = 0; i < blink; i++)
    {
        ledcWrite(LedRChannel, r);
        ledcWrite(LedGChannel, g);
        ledcWrite(LedBChannel, b);
        delay(delayOn);
        if(blink >0)
        {
            ledcWrite(LedRChannel, 0);
            ledcWrite(LedGChannel, 0);
            ledcWrite(LedBChannel, 0);
            if(blink >1)
            {
                delay(delayOff);
            }
        }
    }
}
void setLed(byte r, byte g, byte b)
{
    setLed(r, g, b, 0, 0, 0);
}
void setup()
{
    Serial.begin(9600);

    //LED setup
    ledcSetup(LedRChannel, freq, resolution);
    ledcSetup(LedGChannel, freq, resolution);
    ledcSetup(LedBChannel, freq, resolution);
    ledcAttachPin(LedRPin, LedRChannel);
    ledcAttachPin(LedGPin, LedGChannel);
    ledcAttachPin(LedBPin, LedBChannel);

    setLed(128, 0, 128, 5,500,500);

    SerialBT.begin("ESP32test"); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");

    // Initialize the accelerometer with begin():
    // begin can take two parameters: full-scale range, and output data rate (ODR).
    // Full-scale range can be: SCALE_2G, SCALE_4G, or SCALE_8G (2, 4, or 8g)
    // ODR can be: ODR_800, ODR_400, ODR_200, ODR_100, ODR_50, ODR_12, ODR_6 or ODR_1
    accel.begin(SCALE_8G, ODR_800); // Set up accel with +/-2g range, and slowest (1Hz) ODR
}

void loop()
{
    if(!hasBTclient)
    {
        if (SerialBT.hasClient())
        {
            hasBTclient = true;
            setLed(0,120,255,4,400,400);
        }
    }
    if(hasBTclient)
    {
        if (accel.available())
        {
            setLed(0,128,0);
            accel.read();

        /*  printAccelGraph(accel.cx, "X", 20, 2.0);
            printAccelGraph(accel.cy, "Y", 20, 2.0);
            printAccelGraph(accel.cz, "Z", 20, 2.0);*/
            if(SerialBT.hasClient())
            {
                SerialBT.println(String(accel.cx,10) + ";" +String(accel.cy,10)+";"+ String(accel.cz,10)+";");
            }
            else
            {
                hasBTclient = false;
            }
        }
    }
    else
    {
        setLed(255, 0, 0);
    }

    // No need to delay, since our ODR is set to 1Hz, accel.available() will only return 1
    // about once per second.
}

