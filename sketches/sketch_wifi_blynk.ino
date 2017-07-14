//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial

#include <BlynkSimpleShieldEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
#include <IRremote.h>
#include <IRremoteInt.h>

#define NEC
// AC IR codes
const int nbByte = 32;
const long POWER = 0x10AF8877;
const long UP = 0x10AF708F;
const long DOWN = 0x10AFB04F;
const long SLOWER = 0x10AF20DF;
const long FASTER = 0x10AFB04F;
const long SENSING_START = 0x10AFF5C2;
const long SENSING_STOP = 0x10AFF708;

// TV IR codes
const long TV_POWER = 0xE0E040BF;
const long TV_OFF = 0xE0E0C03F;
const long TV_VOLUME_UP = 0xE0E0E01F;
const long TV_VOLUME_DOWN = 0xE0E0D02F;

//LED pins
int redPin = 9;
int greenPin = 10;
int bluePin = 11;

char auth[] = "9243ed0ef7d048b591d4456915ca1903";
char SSID[] = "Dan";
char WIFI_PASSWORD[] = "thenewyorker";

SimpleTimer timer;
IRsend irSend;
SoftwareSerial esp8266(7, 6); // RX | TX
ESP8266 wifi(&esp8266);

void setup() 
{
  Serial.begin(9600);
  Serial.println("Setup Start");
  esp8266.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Blynk.begin(auth, wifi, SSID, WIFI_PASSWORD);
  Serial.println("Setup end");
}

//TV on
BLYNK_WRITE(V1)
{
  int pinData = param.asInt();
  Serial.println(pinData);
  sendSamsung(TV_POWER);
}

//TV on timer
BLYNK_WRITE(V7)
{
  int pinData = param.asInt();
  Serial.println(pinData);
  sendSamsung(TV_POWER);
}

//AC on/off
BLYNK_WRITE(V2)
{
  int pinData = param.asInt();
  Serial.println(pinData);
  if (pinData == 1) {
    blinkLed(0, 255, 0);
    Serial.println("Pin on");
  } else {
    blinkLed(255, 0, 0);
    Serial.println("Pin off");
  }
  blinkLed(0, 0, 255);
  sendAC(POWER);
}

//Timer
BLYNK_WRITE(V3)
{
  int pinData = param.asInt();
  if (pinData == 1) {
    Serial.println("Timer start");
  } else {
    Serial.println("Timer stop");
  }
  sendAC(POWER);
}

//AC - DOWN
BLYNK_WRITE(V4)
{
  int pinData = param.asInt();
  Serial.println(pinData);
  if (pinData == 1) {
    Serial.println("AC Down");
    sendAC(DOWN);
    blinkLed(0, 0, 255);
  }
}

//AC - UP
BLYNK_WRITE(V5)
{
  int pinData = param.asInt();
  Serial.println(pinData);
  if (pinData == 1) {
    Serial.println("AC Down");
    sendAC(UP);
    blinkLed(255, 0, 0);
  }
}

void sendSamsung(long code)
{
  Serial.println("sending TV signal");  
  irSend.sendSAMSUNG(code, 32);
  //blinkLed(0, 255, 0);
}

void blinkLed(int red, int green, int blue)
{
  setColor(red, green, blue);  // green
  delay(100);
  setColor(0, 0, 0);
}

void setColor(int red, int green, int blue)
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void sendAC(long code) 
{
  Serial.println("sending AC signal");
  irSend.sendNEC(code, 32);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  Blynk.run();
}

