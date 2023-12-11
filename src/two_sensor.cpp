#if 0 //For Backup Purpose Only
#include <Wire.h>
#include "bsp_ads.h" // Click here to get the library: http://librarymanager/All#SparkFun_Displacement_Sensor

ADS ads_ics_1; //Create object of the ADS class
ADS ads_ics_2; //Create object of the ADS class

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

  //Setup second sensor - look for it at the I2C address of 34. You should have set this up in example 6
  if (ads_ics_2.begin(18) == false)
  {
    Serial.println(F("Second sensor not detected. Check wiring. Freezing..."));
    while (1);
  }
}

void loop()
{
  if (ads_ics_1.available() == true && ads_ics_2.available() == true)
  {
    Serial.print("1,");
    Serial.print(ads_ics_1.getX());
    Serial.print(",");
    Serial.print(ads_ics_1.getY());
    Serial.print("   2,");
    Serial.print(ads_ics_2.getX());
    Serial.print(",");
    Serial.print(ads_ics_2.getY());
    Serial.println();
  }
  delay(10);
}
#endif