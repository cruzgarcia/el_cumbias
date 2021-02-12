#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
// OLED Display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// RTC
#include <ThreeWire.h>  
#include <RtcDS1302.h>

// RTC
ThreeWire myWire(18,5,19); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);


// Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


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

  // Display
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //display.display();
  //delay(2000); // Pause for 2 seconds
  // Clear the buffer
  //display.clearDisplay();


#if 0
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
#endif

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    //printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid()) 
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime();
    if (now < compiled) 
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) 
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) 
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

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
  
#if 0
  //send hello world to server
  udp.beginPacket(serverIp, udpPort);
  udp.write(buffer, 2);
  udp.endPacket();
  memset(buffer, 0, 50);
#endif

  /*
  //processing incoming packet, must be called before reading the buffer
  udp.parsePacket();
  //receive response from server, it will be HELLO WORLD
  if(udp.read(buffer, 50) > 0){
    Serial.print("Server to client: ");
    Serial.println((char *)buffer);
  }
  */

  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  //display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write("Holy Frijoles!");

  // RTC
  RtcDateTime now = Rtc.GetDateTime();
  char str[8];
  //display.setCursor(0, 30);
  display.write("  ");
  itoa(now.Hour(), str, 10);
  display.write(str);
  display.write(":");
  itoa(now.Minute(), str, 10);
  display.write(str);
  //display.write(":");
  //itoa(now.Second(), str, 10);
  //display.write(str);

  display.setCursor(0, 40);
  display.write("x: ");
  itoa(joysticXVal, str, 10);
  display.write(str);
  display.write("  y: ");
  itoa(joysticYVal, str, 10);
  display.write(str);

  display.display();
  //Wait for 1 second
  delay(1000);

}
