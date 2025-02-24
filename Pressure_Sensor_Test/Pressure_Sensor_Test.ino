/*
NOTES:
Did some research on volatges etc so I knew how to convert pressure sensor to kPa
Tried it and got values greater than 0 with no pressure
Realised that I would need to calibrate to specific sensor
Played around with values until the pressure printed "0" kPa with no pressure and approximately "200" kPa when I pushed 200 kPa into it with a bike pump
Now set up with a ESP32 and an external 5V power supply
*/

const int pressureSensor = 5;

void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  int val = analogRead(pressureSensor);
  //float voltage = val * (5.00 / 1023.00);
  //float pressure = ((voltage - 2.478) / (4 - 2.478)) * 121;
  float voltage = val * (5 / 4095.0000);
  float pressure = ((voltage - 0.68) / (6.5 - 0.68)) * 100;



  Serial.println("...");
  Serial.println(voltage, 6);
  Serial.println(pressure, 0);
  Serial.println("..");
  delay(1000);
}
