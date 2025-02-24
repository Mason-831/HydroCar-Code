/*
NOTES:
Forgot to convert to temperature originally
Once that was fixed, worked perfectly
*/

const int TempSensor = A0;
void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int val = analogRead(TempSensor);
  float voltage = val * (5.00 / 1023.00);
  float temp = voltage * 100;
  Serial.println(temp);
  delay(1000);
}
