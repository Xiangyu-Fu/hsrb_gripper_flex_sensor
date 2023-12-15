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

unsigned long samples = 0; //Allows us to calculate the actual read rate in Hz
byte deviceType;           //Keeps track of if this sensor is a one axis of two axis sensor

// Izhikevich model parameters
double a = 0.02;
double b = 0.2;
double c = -65;
double d = 6;
double v = -70; // Initial membrane potential
double u = b * v; // Initial recovery variable


void updateIzhikevichModel(double sensorValue, int sensorNumber);

void setup()
{
  pinMode(dataReadyPin, INPUT);

  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println(F("SparkFun Displacement Sensor Example"));

  Wire.begin();
  Wire.setClock(400000); //Note this sensor supports 400kHz I2C

  if (ads_1.begin(32) == false)
  {
    Serial.println(F("No sensor detected. Check wiring. Freezing..."));
    while (1)
      ;
  }
  ads_1.run(); //Begin sensor outputting readings

  if (ads_2.begin(33) == false)
  {
    Serial.println(F("No sensor detected. Check wiring. Freezing..."));
    while (1)
      ;
  }
}

void loop()
{

  if (digitalRead(dataReadyPin) == LOW)
  {
    if (ads_1.available() == true)
    {
      samples++;
      double sensorValue = ads_1.getX(); // Read the sensor value

      updateIzhikevichModel(sensorValue, 1);
    }

    if (ads_2.available() == true) 
    {
      samples++;
      double sensorValue = ads_2.getX(); // Read the sensor value

      updateIzhikevichModel(sensorValue, 2);
    }
  }

}


void updateIzhikevichModel(double sensorValue, int sensorNumber) {
  // Convert sensor value to current input for the Izhikevich model
  double I = abs(sensorValue);

  // Update the Izhikevich model
  v += 0.04 * v * v + 5 * v + 140 - u + I;
  u += a * (b * v - u);

  // Check for spike condition
  if (v >= 30) {
    v = c;
    u += d;
    Serial.print("Neuron ");
    Serial.print(sensorNumber);
    Serial.println(" spike!");
  }

  bool __verbose = false;

  if (__verbose){
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