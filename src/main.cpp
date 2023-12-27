/*
 * Project: HSRB Gripper with flex sensors
 * Description: This code is used to read the flex sensors and send the data to the computer
 * Author: Xiangyu Fu
 * Date: 2023-12-27
 */

#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

#include "Arduino.h"
#include "bsp_ads.h"
#include "Wire.h"

ADS ads_1; 
ADS ads_2;

const byte dataReadyPin = 4; //This can be any pin, but avoid pin 2. See: https://learn.sparkfun.com/tutorials/sparkfun-pro-nrf52840-mini-hookup-guide
// "Because pin 2's state at reset is sampled by the bootloader, be careful using it with any component that may pull the pin low on startup."

long lastTime;

unsigned long samples_1 = 0; //Allows us to calculate the actual read rate in Hz
unsigned long samples_2 = 0; //Allows us to calculate the actual read rate in Hz
byte deviceType;           //Keeps track of if this sensor is a one axis of two axis sensor

// Izhikevich model parameters
double a = 0.02;
double b = 0.2;
double c = -65;
double d = 6;
double v_1 = -70; // Initial membrane potential
double u_1 = b * v_1; // Initial recovery variable
double v_2 = -70; // Initial membrane potential
double u_2 = b * v_2; // Initial recovery variable


void calibrate(ADS myFlexSensor);

void setup()
{
  // pinMode(dataReadyPin, INPUT);

  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println(F("SparkFun Displacement Sensor Example"));

  Wire.begin();
  Wire.setClock(400000); //Note this sensor supports 400kHz I2C

  if (ads_1.begin(33) == false)
  {
    Serial.println(F("No sensor detected. Check wiring. Freezing..."));
    while (1)
      ;
  }

  Serial.println(F("Sensor 1 detected!"));

  if (ads_2.begin(34) == false)
  {
    Serial.println(F("No sensor detected. Check wiring. Freezing..."));
    while (1)
      ;
  }
  Serial.println(F("Sensor 2 detected!"));
  ads_1.enableStretching(true); // Enable receiving stretching data
  ads_2.enableStretching(true); // Enable receiving stretching data
}

void loop()
{

  if (ads_1.available() == true  && ads_2.available() == true)
  {    
    double X_1 = ads_1.getX();
    double I_1 = abs(X_1);

    v_1 += 0.04 * v_1 * v_1 + 5 * v_1 + 140 - u_1 + I_1;
    u_1 += a * (b * v_1 - u_1);

    // Check for ads_1 spike condition
    if (v_1 >= 30) {
      v_1 = c;
      u_1 += d;
      Serial.println("Neuron 1 spike");
    }

    double X_2 = ads_2.getX();
    double I_2 = abs(X_2);

    v_2 += 0.04 * v_2 * v_2 + 5 * v_2 + 140 - u_2 + I_2;
    u_2 += a * (b * v_2 - u_2);

    // Check for ads_2 spike condition
    if (v_2 >= 30) {
      v_2 = c;
      u_2 += d;
      Serial.println("Neuron 2 spike");
    }
  }

    if (Serial.available())
  {
    byte incoming = Serial.read();

    if (incoming == 'a')
    {
      calibrate(ads_1);
    }
      
    else if (incoming == 'b')
    {
      calibrate(ads_2);
    }

    else
    {
      Serial.println("press a or b to calibrate \n OR Reset the Arduino to back to read the sensor");
    }
  }

}

void calibrate(ADS myFlexSensor)
{
  Serial.println(F("Calibration routine"));

  while (Serial.available() > 0)
    Serial.read(); //Flush all characters
  Serial.println(F("Press a key when the sensor is flat and straight on a table"));
  while (Serial.available() == 0)
  {
    myFlexSensor.available();
    delay(10); //Wait for user to press character
  }

  myFlexSensor.calibrateZero(); //Call when sensor is straight on both axis

  while (Serial.available() > 0)
    Serial.read(); //Flush all characters
  Serial.println(F("Good. Now press a key when the sensor is flat on table but bent at 90 degrees (along X axis)."));
  while (Serial.available() == 0)
  {
    myFlexSensor.available();
    delay(10); //Wait for user to press character
  }

  myFlexSensor.calibrateX(); //Call when sensor is straight on Y axis and 90 degrees on X axis

  Serial.println(F("Calibration complete."));
}

// void updateIzhikevichModel(double sensorValue, int sensorNumber) {
//   // Convert sensor value to current input for the Izhikevich model
//   double I = abs(sensorValue);

//   // Update the Izhikevich model
//   v += 0.04 * v * v + 5 * v + 140 - u + I;
//   u += a * (b * v - u);

//   // Check for spike condition
//   if (v >= 30) {
//     v = c;
//     u += d;
//     Serial.print("Neuron ");
//     Serial.print(sensorNumber);
//     Serial.println(" spike!");
//   }

//   bool __verbose = false;

//   if (__verbose){
//     // Output sensor value and model variables
//     Serial.print(samples_1 / (millis() / 1000.0), 2);
//     Serial.print("Hz, Sensor: ");
//     Serial.print(sensorValue);
//     Serial.print(", Membrane Potential: ");
//     Serial.print(v);
//     Serial.print(", Recovery Variable: ");
//     Serial.println(u);

//   }
// }