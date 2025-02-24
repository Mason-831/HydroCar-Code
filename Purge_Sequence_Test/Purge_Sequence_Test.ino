/*
NOTES:
Easy enough, basic on and off writing
*/

const int outletValve = 10;
const int inletValve = 7;
const int statusLight = 15;


void setup() 
{
  pinMode(inletValve, OUTPUT);
  pinMode(outletValve, OUTPUT);
  pinMode(statusLight, OUTPUT);

  for (int i = 0; i<2; i++)
  {
    digitalWrite(statusLight, HIGH);
    delay(1000);
    digitalWrite(statusLight, LOW);
    delay(1000);
  }
  digitalWrite(inletValve, HIGH);
 
  // digitalWrite(fanPin, HIGH);
  delay(5000);
  digitalWrite(statusLight, HIGH);
}

void loop() 
{
  digitalWrite(outletValve, HIGH);
  delay(100);
  digitalWrite(outletValve, LOW);
  delay(9900);
}

