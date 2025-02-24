/*
NOTES:
Already worked out the voltages for the tother sensors
Using a potentiometer to emulate voltage drops in the battery indicating it is going flat
*/

const int batteryVoltage = 6;

void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  int val = analogRead(batteryVoltage);
  float voltage = val * (5.00 / 1023.00);

  Serial.println(voltage, 2);
  delay(1000);
}
