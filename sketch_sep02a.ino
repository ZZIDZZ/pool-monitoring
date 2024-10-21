#define BLYNK_TEMPLATE_ID "TMPL6EHzskoIV"
#define BLYNK_TEMPLATE_NAME "embedded"
#define BLYNK_AUTH_TOKEN "C2UCxAoghZVDQNRI9Lc486CdmpQsGxJr"
char auth[] = "C2UCxAoghZVDQNRI9Lc486CdmpQsGxJr";

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
// Your Wi-Fi credentials
const char* ssid = "Afano8";
const char* pass = "jangkrik2022";

// Your Blynk authentication token

const int oneWireBus = 5;    

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Define the GPIO pins for the ultrasonic sensor
#define trigPin 2
#define echoPin 4

// Create an instance of the NewPing library
NewPing sonar(trigPin, echoPin);

void setup() {
  Serial.begin(115200);
  Serial.println("System Initializing...");


  Blynk.begin(auth, ssid, pass);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.init();
  lcd.backlight();
  //Setup Awal Ketika Running
  lcd.setCursor(2,0);
  lcd.print("KELAS");
  lcd.setCursor(0,1);
  lcd.print("SISTEM TERTANAM");
  delay(4000);
  lcd.clear();


  sensors.begin();

  
  Serial.println("System Initialized");


}

void loop() {
  Blynk.run();

  // Trigger an ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the distance
  unsigned int distance_cm = sonar.ping_cm();
  unsigned int distance_mm = (distance_cm*10);

  // Send the distance data to Blynk
  Blynk.virtualWrite(V1, distance_cm);  // Use V1 as a display widget in your Blynk app
  
  Serial.print("Stem Displacement: ");
  Serial.print(distance_cm);
  Serial.println(" mm");

  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("DIST");
  lcd.setCursor(3,1);
  lcd.print(distance_mm);
  delay(400);

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");

  Blynk.virtualWrite(V2, temperatureC);  // Use V1 as a display widget in your Blynk app


  delay(1000); // Delay between measurements
}
