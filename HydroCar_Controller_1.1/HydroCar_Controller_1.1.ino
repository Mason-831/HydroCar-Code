/* 
NOTES:
Just implemented the pressure sensor and continues to work just fine
Also wrote in some in depth annotations for others to understand the code
Realised that the pressure sensor does not work on the 5Vin pin on the esp 32 so i started using the 3V3 pin instead, but it has a drastically limited range

***REMINDER***
1. Current Temperature Threshold is set to a temp achievable by covering the sensor with my fingers
2. Pressure sensor works in reverse (Gets to 200 kPa and provokes error but should be the other way round)
3. Inlet and Outlet valves are currently turned on and off in reverse
*/

#include <TaskScheduler.h>

const int inletValve = 7;
const int outletValve = 10;
const int statusRed = 16;
const int statusGreen = 15;
const int statusLoading = 17;
const int TempSensor = 4;
const int PressureSensor = 5;
const int BatteryVoltage = 6;
bool hot = false;
bool flat = false;

//Funtion for opening and closing the outlet solenoid asynchronously (So it doesnt impact the rest of the functions)
void PurgeSequence()
{
  // Purge Sequence
  digitalWrite(outletValve, HIGH);
  delay(100);
  digitalWrite(outletValve, LOW);
}

//Make the purging function run repeatedly every 9.9 seconds (9900 ms)
Task purge(9900, TASK_FOREVER, &PurgeSequence);
//Make the fuction running everythin else repeat every second (1000 ms)
Task mainLoop(1000, TASK_FOREVER, &MainLoop);

Scheduler runner;

void MainLoop()
{
  //Read and convert values from the temperature sensor to degrees celcius
  int tempVal = analogRead(TempSensor);
  float tempVoltage = tempVal * (5 / 4095.00);
  float temp = (tempVoltage - 0.13) * 100;
  if (temp < 1)
  {
    temp = 0;
  }

  //Read and convert values from the pressure sensor to kiloPascals
  int pressureVal = analogRead(PressureSensor);
  float pressureVoltage = pressureVal * (5 / 4095.00);
  float pressure = ((pressureVoltage - 3.71) / (20.0147 - 14.65)) * 200;
  if (pressure < 1)
  {
    pressure = 0;
  }

  //Read and convert values from the battery's voltage
  int batteryVal = analogRead(BatteryVoltage);
  float batteryVoltage = batteryVal * (3.3 / 4095.00);

  //Print Values in the serial monitor
  Serial.println("..");
  Serial.println(temp);
  Serial.println(pressure);
  Serial.println(batteryVoltage);

  //Check that the temperature doesnt exceed the max desired threshold
  if(temp >= 30)
  {
    if(hot == false)
    {
      if(flat == false)
      {
        digitalWrite(statusGreen, LOW); //Make the status light turn red
        digitalWrite(statusRed, HIGH);
      }
      hot = true;
    }
  }
  else
  {
    if(hot == true)
    {
      if (flat == false)
      {
        digitalWrite(statusGreen, HIGH); //Make the status light turn green
        digitalWrite(statusRed, LOW);
      }
      hot = false;
    }
  }

  //Check that the pressure doesnt drop bellow the min desired threshold
  if(pressure >= 100)
  {
    if(flat == false)
    {
      if(hot == false)
      {
        digitalWrite(statusGreen, LOW); //Make the status light turn red
        digitalWrite(statusRed, HIGH);
      }
      flat = true;
    }
  }
  else
  {
    if(flat == true)
    {
      if (hot == false)
      {
        digitalWrite(statusGreen, HIGH); //Make the status light turn green
        digitalWrite(statusRed, LOW);
      }
      flat = false;
    }
  }
}

void setup() 
{
  Serial.begin(115200);
  pinMode(inletValve, OUTPUT);
  pinMode(outletValve, OUTPUT);
  pinMode(statusRed, OUTPUT);
  pinMode(statusGreen, OUTPUT);
  pinMode(statusLoading, OUTPUT);

  //Display the initialisation of the controller on the status light
  for (int i = 0; i<2; i++)
  {
    digitalWrite(statusLoading, HIGH);
    delay(1000);
    digitalWrite(statusLoading, LOW);
    delay(1000);
  }
  //Open the inlet solenoid to let gas flow through (CURRENTLY REVERSED)
  digitalWrite(inletValve, HIGH);
 
  //Set the status light green to indicate that the initialisation is complete
  delay(5000);
  digitalWrite(statusGreen, HIGH);

  //Start running the asynchronous tasks
  runner.addTask(purge);
  runner.addTask(mainLoop);
  purge.enable();
  mainLoop.enable();
}

void loop() 
{
  runner.execute();
}

