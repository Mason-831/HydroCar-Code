const int TempSensor = A0;
const int Fan = 3;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Fan, OUTPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  digitalWrite(Fan, HIGH);
  delay(20);
  digitalWrite(Fan, LOW);
  delay(20);
}
