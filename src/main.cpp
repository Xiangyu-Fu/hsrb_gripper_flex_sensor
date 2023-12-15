#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

#include "Arduino.h"
#include "bsp_ads.h"
#include "Wire.h"

ADS myFlexSensor; //Create object of the ADS class

const byte dataReadyPin = 4; //This can be any pin, but avoid pin 2. See: https://learn.sparkfun.com/tutorials/sparkfun-pro-nrf52840-mini-hookup-guide
// "Because pin 2's state at reset is sampled by the bootloader, be careful using it with any component that may pull the pin low on startup."

long lastTime;

unsigned long samples = 0; //Allows us to calculate the actual read rate in Hz
byte deviceType;           //Keeps track of if this sensor is a one axis of two axis sensor

// Izhikevich model parameters
float a = 0.02;
float b = 0.2;
float c = -65;
float d = 6;
float v = -70; // Initial membrane potential
float u = b * v; // Initial recovery variable


void setup()
{
  pinMode(dataReadyPin, INPUT);

  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println(F("SparkFun Displacement Sensor Example"));

  Wire.begin();
  Wire.setClock(400000); //Note this sensor supports 400kHz I2C

  if (myFlexSensor.begin(18) == false)
  {
    Serial.println(F("No sensor detected. Check wiring. Freezing..."));
    while (1)
      ;
  }

  deviceType = myFlexSensor.getDeviceType();
  if (deviceType == ADS_ONE_AXIS)
    Serial.println(F("One axis displacement sensor detected"));
  else if (deviceType == ADS_TWO_AXIS)
    Serial.println(F("Two axis displacement sensor detected"));

  myFlexSensor.run(); //Begin sensor outputting readings
}

void loop()
{

  if (digitalRead(dataReadyPin) == LOW)
  {
    if (myFlexSensor.available() == true) //We still need to call .available because it loads the X and Y variables
    {
      samples++;
      float sensorValue = myFlexSensor.getX(); // Read the sensor value

      // Convert sensor value to current input for the Izhikevich model
      float I = sensorValue; // Simple direct conversion, adjust as needed

      // Update the Izhikevich model
      v += 0.04 * v * v + 5 * v + 140 - u + I;
      u += a * (b * v - u);

      // Check for spike condition
      if (v >= 30) {
        v = c;
        u += d;
        Serial.println("Neuron spike!");
      }

      // Output sensor value and model variables
      Serial.print(samples / (millis() / 1000.0), 2);
      Serial.print("Hz, Sensor: ");
      Serial.print(sensorValue);
      Serial.print(", Membrane Potential: ");
      Serial.print(v);
      Serial.print(", Recovery Variable: ");
      Serial.println(u);
    }
  }

}