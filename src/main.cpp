#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "TMPL6zcPmgn2"
#define BLYNK_DEVICE_NAME "Quickstart Template"

#include <ESP8266WiFi.h>
#include <BlynkSimpleStream.h>
#include <Servo.h>

// Your WiFi credentials.
// Set password to "" for open networks.
const char *ssid = "JTI-POLINEMA";
const char *pass = "jtifast!";

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "wRCki9bv77wMDK5p2-skONE3Oqm3IH5E";

WiFiClient wifiClient;

const int triggerPin = D6;
const int echoPin = D5;

Servo myServo;
int servoPin = D7;

long duration;
long jarak;


// This function tries to connect to the cloud using TCP
bool connectBlynk()
{
  wifiClient.stop();
  return wifiClient.connect(BLYNK_DEFAULT_DOMAIN, BLYNK_DEFAULT_PORT);
}

// This function tries to connect to your WiFi network
void connectWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (pass && strlen(pass))
  {
    WiFi.begin((char *)ssid, (char *)pass);
  }
  else
  {
    WiFi.begin((char *)ssid);
  }

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}

void readJarak() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = duration * 0.034 / 2;
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);

  
  connectWiFi();

  connectBlynk();

  Blynk.begin(wifiClient, auth);
  
}

void openGate() {
  myServo.write(180);
}

void closeGate() {
  myServo.write(0);
}

BLYNK_WRITE(V1)
{
  
  if (param.asInt())
  {
    openGate();
    delay(5000);
  }
  else
  {
    closeGate();
    delay(5000);
  }

  Blynk.virtualWrite(V1, 0);
}

void loop()
{
  // Reconnect WiFi
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWiFi();
    return;
  }

  // Reconnect to Blynk Cloud
  if (!wifiClient.connected())
  {
    connectBlynk();
    return;
  }

  readJarak();
  Blynk.virtualWrite(V2, jarak);
  if (jarak <= 10)
  {
    Serial.print(jarak);
    Serial.println(" cm jarak 1");
    Serial.println(" gerbang masuk terbuka");
    Serial.println();
    openGate();
    }
  else {
    closeGate();
  }

  Blynk.run();

  delay(100);
}

