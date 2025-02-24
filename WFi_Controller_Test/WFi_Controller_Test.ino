/*
NOTES: Had issues sending over from the app and realised that I had defined the sendWiFi funtion in the button function so nothing was actually being called
After fixing that, I can receive messages easily
*/

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>   
#include <ArduinoWebsockets.h> 

const char *ssid = "ESP32";
const char *password = "123456789";

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
  }
}

void setup() 
{
  //Initialise Serial Monitor
  Serial.begin(115200);
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
}

void loop() 
{
  //Remove disconnected clients
  ws.cleanupClients();
}
