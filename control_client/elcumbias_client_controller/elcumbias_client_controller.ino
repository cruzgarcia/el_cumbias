#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>

const int joystickXPin = 34;
const int joystickYPin = 35;

// variable for storing the potentiometer value
int joysticXVal = 0;
int joysticYVal = 0;

/*
const char* ssid = "elcumbias";
const char* password = "";
*/



//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.4.1:80/update-sensor";

//const char* serverIp = "192.168.4.1";
const char* serverIp = "192.168.178.43";
const int udpPort = 44444;
WiFiUDP udp;


void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Initiates the UDP port
  udp.begin(udpPort);

}

void loop() {

  // Read joystick status
  joysticXVal = analogRead(joystickXPin);
  joysticXVal = joysticXVal >> 2;
  joysticYVal = analogRead(joystickYPin);
  joysticYVal = joysticYVal >> 2;
  Serial.print("Joystick X: ");
  Serial.print(joysticXVal);
  Serial.print(" Y: ");
  Serial.println(joysticYVal);

  
  //data will be sent to server
  //uint8_t buffer[50] = "hello world";
  uint8_t buffer[50] = {0};
  buffer[0] = joysticXVal;
  buffer[1] = joysticYVal;
  
  //send hello world to server
  udp.beginPacket(serverIp, udpPort);
  udp.write(buffer, 2);
  udp.endPacket();
  memset(buffer, 0, 50);

  /*
  //processing incoming packet, must be called before reading the buffer
  udp.parsePacket();
  //receive response from server, it will be HELLO WORLD
  if(udp.read(buffer, 50) > 0){
    Serial.print("Server to client: ");
    Serial.println((char *)buffer);
  }
  */
  
  //Wait for 1 second
  delay(100);

}
