/* 
NOTES:
Implemented the system which writes the variables to the involitile memory
This means that you can permenantly change wifi names, passwords and purge times

***REMINDER***
1. Current Temperature Threshold is set to a temp achievable by covering the sensor with my fingers
2. Pressure sensor works in reverse (Gets to 200 kPa and provokes error but should be the other way round)
3. Inlet and Outlet valves are currently turned on and off in reverse
*/

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>   
#include <ArduinoWebsockets.h>
#include <FS.h>
#include <LittleFS.h>
#include <vector>

//Pinouts
const int inletValve = 7;
const int outletValve = 10;
const int statusRed = 16;
const int statusGreen = 15;
const int statusLoading = 17;
const int TempSensor = 4;
const int PressureSensor = 5;
const int BatteryVoltage = 6;
//WiFi login info
String ssid = "ESP32";
String password = "123456789";

//General variables
bool hot = false;
bool flat = false;
int purgeDelay = 9900;
int purgeLapse = 100;

//Initialise WebSocket Server
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

//WebSocket Host (For receiving connections from clients)
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) 
{
  //If a new client is connected
  if (type == WS_EVT_CONNECT) 
  {
    Serial.println("New Client Connected");
  } 
  //If an existing client is sending data
  else if (type == WS_EVT_DATA) 
  {
    //Convert that data to a readable message
    String message = String((char*)data).substring(0, len);
    //Read the message and act accordingly
    if (message == "WORKING")
    {
      Serial.println("it works");
      //Send a message back to all clients
      ws.textAll("Ok");
    }

    else if (message.startsWith("Purge Duration: "))
    {
      String val = message.substring(16);
      purgeDelay = val.toInt() - 100;
      updateVariables(5, String(purgeDelay));
    }

    else if (message.startsWith("Purge Lapse: "))
    {
      String val = message.substring(13);
      purgeLapse = val.toInt();
      updateVariables(7, String(purgeLapse));
    }

    else if (message.startsWith("WiFi Name: "))
    {
      ssid = message.substring(11);
      updateVariables(1, String(ssid));
    }

    else if (message.startsWith("WiFi Password: "))
    {
      password = message.substring(15);
      updateVariables(3, String(password));
    }
  }
}

//Update the file which holds all of the Variable values in the involitile memory
void updateVariables(int lineNumber, String newContent) {
  //Check to see if the variable file exists
  if (!LittleFS.exists("/Variables.txt")) 
  {
    // Create the file
    File newFile = LittleFS.open("/Variables.txt", "w");  
    if (!newFile) 
    {
      //Cancel the function if the file cant be made
      return;
    }

    //Initialise the variables in the file
    newFile.println("WiFi SSID");
    newFile.println("SSID");
    newFile.println("WiFi Password");
    newFile.println("Password123");
    newFile.println("Purge Duration");
    newFile.println("9900");
    newFile.println("Purge lapse");
    newFile.println("100");
    newFile.println("Close");
    //Close and save the file
    newFile.close();
  }
  //Otherwise
  //Open the variable file 
  File file = LittleFS.open("/Variables.txt", "r");
  if (!file) 
  {
    return;
  }
  //Read every line in the file and write it to the lines array
  String lines[9];
  int lineNo = 0;
  while (file.available()) 
  {
    String line = file.readStringUntil('\n');  // Read one line
      
    lines[lineNo] = line;
    lineNo++;
    // Print the line
    Serial.println(line);  
    // Check if the line contains "Close"
    if (line.indexOf("Close") != -1) 
    {  
      // Exit loop if "Close" is found
      break;  
    }
  }
  lineNo = 0;
  file.close();

  // Check if the line number is valid
  if (lineNumber < 0 || lineNumber >= 9) 
  {
   return;
  }

  // Replace the specified line
  lines[lineNumber] = newContent;

  // Write the modified content back to the file
  file = LittleFS.open("/Variables.txt", "w");
  if (!file) 
  {
   return;
  }

  //Write everything back to the file
  for (String line : lines) 
  {
   file.println(line);
  }
  file.close();

  Serial.println("Updated line " + String(lineNumber) + " to: " + newContent);
}

//Funtion for opening and closing the outlet solenoid asynchronously (So it doesnt impact the rest of the functions)
void PurgeSequence(void *parameter)
{
  while (1)
  {
    // Purge Sequence
    digitalWrite(outletValve, LOW);
    vTaskDelay(purgeLapse / portTICK_PERIOD_MS);
    digitalWrite(outletValve, HIGH);
    vTaskDelay(purgeDelay / portTICK_PERIOD_MS); // Wait 9.9s before running again
  }
}

void MainLoop(void *parameter)
{
  while (1)
  {
    //Read and convert values from the temperature sensor to degrees celcius
    int tempVal = analogRead(TempSensor);
    float tempVoltage = tempVal * (5.0 / 4095.00);
    float temp = (tempVoltage - 0.05) * 100;
    if (temp < 1)
    {
      temp = 0;
    }

    //Read and convert values from the pressure sensor to kiloPascals
    int pressureVal = analogRead(PressureSensor);
    float pressureVoltage = pressureVal * (5.0 / 4095.00);
    float pressure = ((pressureVoltage - 3.71) / (20.0147 - 14.65)) * 40;
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

    String Temp = String(temp, 2);
    String Pressure = String(pressure, 2);
    String Voltage = String(batteryVoltage, 2);

    //Send values in a websocket message to clients
    ws.textAll("Temperature: " + Temp);
    ws.textAll("Pressure: " + Pressure);
    ws.textAll("Battery Voltage: " + Voltage);

    //Check that the temperature doesnt exceed the max desired threshold
    if (temp >= 30)
    {
      if (!hot)
      {
        if (!flat)
        {
          digitalWrite(statusGreen, LOW); //Make the status light turn red
          digitalWrite(statusRed, HIGH);
        }
        hot = true;
      }
    }
    else
    {
      if (hot)
      {
        if (!flat)
        {
          digitalWrite(statusGreen, HIGH); //Make the status light turn green
          digitalWrite(statusRed, LOW);
        }
        hot = false;
      }
    }

    //Check that the pressure doesnt drop bellow the min desired threshold
    if (pressure >= 100)
    {
      if (!flat)
      {
        if (!hot)
        {
          digitalWrite(statusGreen, LOW); //Make the status light turn red
          digitalWrite(statusRed, HIGH);
        }
        flat = true;
      }
    }
    else
    {
      if (flat)
      {
        if (!hot)
        {
          digitalWrite(statusGreen, HIGH); //Make the status light turn green
          digitalWrite(statusRed, LOW);
        }
        flat = false;
      }
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Run every second
    //Remove disconnected clients
    ws.cleanupClients();
  }
}

void setup()
{
  //Initialise Serial Monitor
  Serial.begin(115200);
  //Initialise pinout modes
  pinMode(inletValve, OUTPUT);
  pinMode(outletValve, OUTPUT);
  pinMode(statusRed, OUTPUT);
  pinMode(statusGreen, OUTPUT);
  pinMode(statusLoading, OUTPUT);

  digitalWrite(inletValve, HIGH);


  //Initialise the ESP-32 in an Access Point (AP) mode
  WiFi.softAP(ssid, password);
  //Read the ESP-32's IP address
  Serial.println(WiFi.softAPIP());
  //Initialise where the ESP-32 should go to handle any websocket message sent to it's IP address
  ws.onEvent(onWebSocketEvent);
  //Initialise what channel it should be looking for these messages on
  server.addHandler(&ws);
  //Initialise the server with all its information
  server.begin();

  if (!LittleFS.begin(true)) 
  {  
    // true enables formatting if needed
    return;
  }

  //Read and write variables
  File file = LittleFS.open("/Variables.txt", "r");
  if (file) 
  {
    //Read every line in the file and write it to the lines array
    String lines[9];
    int lineNo = 0;
    while (file.available()) 
    {
      String line = file.readStringUntil('\n');  // Read one line
        
      lines[lineNo] = line;
      lineNo++;
      // Print the line
      Serial.println(line);  
      // Check if the line contains "Close"
      if (line.indexOf("Close") != -1) 
      {  
        // Exit loop if "Close" is found
        break;  
      }
    }
    lineNo = 0;
    file.close();

    ssid = lines[1];
    password = lines[3];
    purgeDelay = lines[5].toInt();
    purgeLapse = lines[7].toInt(); 
  }

  //Display the initialisation of the controller on the status light
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(statusLoading, HIGH);
    delay(1000);
    digitalWrite(statusLoading, LOW);
    delay(1000);
  }
  //Open the inlet solenoid to let gas flow through (CURRENTLY REVERSED)
  digitalWrite(inletValve, LOW);

  //Set the status light green to indicate that the initialisation is complete
  delay(5000);
  digitalWrite(statusGreen, HIGH);

  //Pin functions to seperate cores
  xTaskCreatePinnedToCore(PurgeSequence, "PurgeTask", 10000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(MainLoop, "MainLoopTask", 10000, NULL, 1, NULL, 1);
}

void loop()
{
  vTaskDelay(portMAX_DELAY); // Let FreeRTOS handle the tasks
}