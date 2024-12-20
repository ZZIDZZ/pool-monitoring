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
const char* ssid = "zzidzz";
const char* pass = "blablablabla";

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

#define PH_SENSOR_PIN 34

// Define pins for relay module
#define RELAY1_PIN 25
#define RELAY2_PIN 26

// XXX Function to read pH level from the sensor (dummy implementation for illustration)
float readPH() {
  return analogRead(PH_SENSOR_PIN) * (14.0 / 4095.0);  // Example conversion for pH range 0-14
}


void setup() {
  Serial.begin(115200);
  Serial.println("System Initializing...");

  Blynk.begin(auth, ssid, pass);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(PH_SENSOR_PIN, INPUT);
  pinMode(RELAY1_PIN, OUTPUT);

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
  unsigned int distance_mm = (distance_cm * 10);

  // Send the distance data to Blynk
  Blynk.virtualWrite(V1, distance_cm);

  Serial.print("Stem Displacement: ");
  Serial.print(distance_cm);
  Serial.println(" mm");

  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("DIST");
  lcd.setCursor(3, 1);
  lcd.print(distance_mm);
  delay(400);

  // Activate relay if water depth exceeds limit
  if (distance_cm > 200) {
    digitalWrite(RELAY1_PIN, LOW); // Turn on the pump
    Blynk.logEvent("water_depth_high", String("Water depth exceeds limit: ") + distance_cm);
    Serial.println("Pump activated due to high water depth.");
  } else {
    digitalWrite(RELAY1_PIN, HIGH); // Turn off the pump
  }

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("°C");
  Serial.print(temperatureF);
  Serial.println("°F");

  Blynk.virtualWrite(V2, temperatureC);

  // XXX Read pH level
  float ph = readPH();
  Serial.print("pH Level: ");
  Serial.println(ph);
  Blynk.virtualWrite(V3, ph);

  // XXX Notifications for pH level
  if (ph < 6.5 || ph > 8.5) {
    Blynk.logEvent("ph_warning", String("pH level is outside the normal range: ") + ph);
  }


  // XXX Notifications for temperature
  if (temperatureC < 20.0) {
    Blynk.logEvent("temp_low", String("Water temperature is too low: ") + temperatureC);
  } else if (temperatureC > 30.0) {
    Blynk.logEvent("temp_high", String("Water temperature is too high: ") + temperatureC);
  }

  delay(1000); // Delay between measurements
}
