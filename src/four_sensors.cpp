#if 0 //For Backup Purpose Only
#include <Wire.h>
#include "bsp_ads.h" // Click here to get the library: http://librarymanager/All#SparkFun_Displacement_Sensor

ADS ads_ics_1; //Create object of the ADS class
ADS ads_ics_2; //Create object of the ADS class
ADS ads_ics_3; //Create object of the ADS class
ADS ads_ics_4; //Create object of the ADS class

void setup()
{
    Serial.begin(115200);
    while (!Serial);
    Serial.println(F("SparkFun Displacement Sensor Example"));

    Wire.begin();

    //Setup first sensor - look for it at the default address of 0x21 = 33
    if (ads_ics_1.begin(33) == false)
    {
        Serial.println(F("First sensor not detected. Check wiring. Freezing..."));
        while (1);
    }

    //Setup second sensor - look for it at the I2C address of 34.
    if (ads_ics_2.begin(34) == false)
    {
        Serial.println(F("Second sensor not detected. Check wiring. Freezing..."));
        while (1);
    }

    //Setup third sensor - look for it at the I2C address of 35.
    if (ads_ics_3.begin(35) == false)
    {
        Serial.println(F("Third sensor not detected. Check wiring. Freezing..."));
        while (1);
    }

    //Setup fourth sensor - look for it at the I2C address of 36.
    if (ads_ics_4.begin(36) == false)
    {
        Serial.println(F("Fourth sensor not detected. Check wiring. Freezing..."));
        while (1);
    }
}

void loop()
{
  if (ads_ics_1.available() == true && ads_ics_2.available() == true)
  {
    Serial.print(" 1,");
    Serial.print(ads_ics_1.getX());
    Serial.print(",");
    Serial.print(ads_ics_1.getY()); // ? may be 1-axis sensor does not have Y
    Serial.print(",");
    Serial.print(ads_ics_1.getStretchingData());
    Serial.print(" 2,");
    Serial.print(ads_ics_2.getX());
    Serial.print(",");
    Serial.print(ads_ics_2.getY());
    Serial.print(",");
    Serial.print(ads_ics_2.getStretchingData());
    Serial.print(" 3,");
    Serial.print(ads_ics_3.getX());
    Serial.print(",");
    Serial.print(ads_ics_3.getY());
    Serial.print(",");
    Serial.print(ads_ics_3.getStretchingData());
    Serial.print(" 4,");
    Serial.print(ads_ics_4.getX());
    Serial.print(",");
    Serial.print(ads_ics_4.getY());
    Serial.print(",");
    Serial.print(ads_ics_4.getStretchingData());
    Serial.println();

  }
  delay(10);
}
#endif