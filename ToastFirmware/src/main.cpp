#include <Arduino.h>
#include "SparkFunMMA8452Q.h"
#include "BluetoothSerial.h"

int freq = 5000;
int fanPin = GPIO_NUM_15;
int LedRChannel = 0;
int LedGChannel = 1;
int LedBChannel = 2;
int LedRPin = GPIO_NUM_12;
int LedGPin = GPIO_NUM_27;
int LedBPin = GPIO_NUM_33;
int resolution = 8;
String input = "";
bool hasBTclient = false;

BluetoothSerial SerialBT;

// Create an MMA8452Q object, used throughout the rest of the sketch.
MMA8452Q accel; // Default constructor, SA0 pin is HIGH
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
void processCommand(String command)
{
    if(command.length() > 2)
    {
        if(command =="fon")
        {
            digitalWrite(fanPin, HIGH);
        }
        else if (command == "foff")
        {
            digitalWrite(fanPin, LOW);
        }
    }
}
void setup()
{
    Serial.begin(9600);
    //fan setup
    pinMode(fanPin, OUTPUT);

    //LED setup
    ledcSetup(LedRChannel, freq, resolution);
    ledcSetup(LedGChannel, freq, resolution);
    ledcSetup(LedBChannel, freq, resolution);
    ledcAttachPin(LedRPin, LedRChannel);
    ledcAttachPin(LedGPin, LedGChannel);
    ledcAttachPin(LedBPin, LedBChannel);

    setLed(128, 0, 128, 5,100,100);

    SerialBT.begin("ESP32test"); //Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");

    // Initialize the accelerometer with begin():
    // begin can take two parameters: full-scale range, and output data rate (ODR).
    // Full-scale range can be: SCALE_2G, SCALE_4G, or SCALE_8G (2, 4, or 8g)
    // ODR can be: ODR_800, ODR_400, ODR_200, ODR_100, ODR_50, ODR_12, ODR_6 or ODR_1
    accel.begin(SCALE_4G, ODR_800); // Set up accel with +/-2g range, and slowest (1Hz) ODR
}

void loop()
{
    if(!hasBTclient)
    {
        if (SerialBT.hasClient())
        {
            hasBTclient = true;
            setLed(0,120,255,3,200,200);
            delay(30);
        }
    }
    if(hasBTclient)
    {
        if (accel.available())
        {
            setLed(0,128,0);
            accel.read();
            if(SerialBT.hasClient())
            {
                input ="";
                while(SerialBT.available())
                {
                    input = input + (char)SerialBT.read();
                }
                input.trim();
                processCommand(input);
                //send accel data.
                SerialBT.println(String(accel.cx, 10) + ";" + String(accel.cy, 10) + ";" + String(accel.cz, 10) + ";");
                SerialBT.flush();
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

