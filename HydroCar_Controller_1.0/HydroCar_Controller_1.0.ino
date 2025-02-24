/* 
NOTES:
Realised before testing that C++ has no way to run certain tasks asynchronously natively
Added the "Scheduler" library and found that it isnt compatible with all arduino like platforms
Added the "TaskScheduler" library after some research
Now everything works perfectly

***REMINDER***
Current Temperature Threshold is set to a temp achievable by covering the sensor with my fingers
*/

#include <TaskScheduler.h>

const int inletValve = 10;
const int outletValve = 11;
const int statusRed = 12;
const int statusGreen = 13;
const int statusLoading = 9;
const int TempSensor = A0;
bool hot = false;

void PurgeSequence()
{
  // Purge Sequence
  digitalWrite(outletValve, HIGH);
  delay(100);
  digitalWrite(outletValve, LOW);
}

Task purge(9900, TASK_FOREVER, &PurgeSequence);  // Run every 1000ms indefinitely
Task mainLoop(1000, TASK_FOREVER, &MainLoop);   // Run every 500ms indefinitely

Scheduler runner;

void MainLoop()
{
  int val = analogRead(TempSensor);
  float voltage = val * (5.00 / 1023.00);
  float temp = voltage * 100;
  if(temp >= 30)
  {
    if(hot != true)
    {
      digitalWrite(statusGreen, LOW);
      digitalWrite(statusRed, HIGH);
      hot = true;
    }
  }
  else
  {
    if(hot != false)
    {
      digitalWrite(statusGreen, HIGH);
      digitalWrite(statusRed, LOW);
      hot = false;
    }
  }
  Serial.println(temp);
}

void setup() 
{
  Serial.begin(9600);
  pinMode(inletValve, OUTPUT);
  pinMode(outletValve, OUTPUT);
  pinMode(statusRed, OUTPUT);
  pinMode(statusGreen, OUTPUT);
  pinMode(statusLoading, OUTPUT);

  for (int i = 0; i<2; i++)
  {
    digitalWrite(statusLoading, HIGH);
    delay(1000);
    digitalWrite(statusLoading, LOW);
    delay(1000);
  }
  digitalWrite(inletValve, HIGH);
 
  delay(5000);
  digitalWrite(statusGreen, HIGH);

  runner.addTask(purge);
  runner.addTask(mainLoop);
  purge.enable();
  mainLoop.enable();
}

void loop() 
{
  runner.execute();
}

