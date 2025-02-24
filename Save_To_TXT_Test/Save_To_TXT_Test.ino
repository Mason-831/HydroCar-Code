/*
NOTES:
That took forever
*/
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoWebsockets.h>
#include <FS.h>
#include <LittleFS.h>
#include <vector>

using namespace websockets;

const char* ssid = "YourSSID";
const char* password = "123456789";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void updateScriptLine(int lineNumber, String newContent);
void updateScriptByKeyword(String keyword, String newContent);

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
    String message = String((char *)data);
    Serial.println("Received: " + message);

    if (message.startsWith("purge:")) 
    {
      String newContent = message.substring(6); 
      updateScriptLine(3, newContent);
    } 
  }
}


void setup() {
    Serial.begin(115200);

    Serial.println("ok");
    
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
  {  // true enables formatting if needed
    Serial.println("❌ LittleFS Mount Failed!");
    return;
  }

  Serial.println("✅ LittleFS Initialized!");
}

void loop() {
    ws.cleanupClients();
}

// 🔹 Modify script by line number
void updateScriptLine(int lineNumber, String newContent) {
    if (!LittleFS.exists("/data.txt")) {
        Serial.println("❌ File does not exist. Creating it...");
        File newFile = LittleFS.open("/data.txt", "w");  // Create the file
        if (!newFile) {
            Serial.println("❌ Failed to create file.");
            return;
        }
        newFile.println("Default Line 1");
        newFile.println("Default Line 2");
        newFile.println("Default Line 3");
        newFile.println("Close");
        newFile.close();
    }
    
    File file = LittleFS.open("/data.txt", "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    String lines[4];
    int lineNo = 0;
    while (file.available()) 
    {
      String line = file.readStringUntil('\n');  // Read one line
      
      lines[lineNo] = line;
      lineNo++;

      Serial.println(line);  // Print the line
        
      if (line.indexOf("Close") != -1) 
      {  // Check if the line contains "Close"
        Serial.println("Found 'Close'. Stopping read.");
        break;  // Exit loop if "Close" is found
      }
    }
    lineNo = 0;
    file.close();

    // Check if the line number is valid
    if (lineNumber < 0 || lineNumber >= 4) {
        Serial.println("Invalid line number!");
        return;
    }

    // Replace the specified line
    lines[lineNumber] = newContent;

    // Write the modified content back to the file
    file = LittleFS.open("/data.txt", "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    for (String line : lines) {
        file.println(line);
    }
    file.close();

    Serial.println("Updated line " + String(lineNumber) + " to: " + newContent);
}