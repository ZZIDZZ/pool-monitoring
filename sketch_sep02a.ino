#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include <SoftwareSerial.h>
#include <stdlib.h>

// Initialize SoftwareSerial for ESP8266
SoftwareSerial ESP8266(3, 2); // RX, TX

// Initialize the LCD
LiquidCrystal_I2C lcd(0x27, 20, 4); // Adjust for your display

// Define pins for ultrasonic sensor
const int trigPin = 7;
const int echoPin = 8;

// Convert microseconds to inches
long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2; // 1130 feet per second
}

// Convert microseconds to centimeters
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2; // 340 meters per second
}

void setup() {
  // Initialize pins for ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Initialize and clear LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Ywrobot Arduino!");

  // Begin serial communication at 115200 bps
  Serial.begin(115200);
  ESP8266.begin(115200);  // Uncomment for ESP8266 usage
  
  Serial.println("System Initialized");
}

void loop() {
  static int iterationCount = 0; // Counter to track the number of iterations
  static long distanceSum = 0; // Accumulator for the distances
  long duration, inches, cm;
  float averageDistance;

  // Trigger the measurement
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echo
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  // Accumulate the distance
  distanceSum += cm;
  iterationCount++;

  // Print the results to the serial monitor
  Serial.print(inches);
  Serial.print(" in, ");
  Serial.print(cm);
  Serial.println(" cm");

  // Compute and display the average distance every 5 iterations
  if (iterationCount % 5 == 0) {
    averageDistance = distanceSum / 5.0; // Compute average distance
    lcd.clear();
    lcd.setCursor(3, 0); // Position at fourth row
    lcd.print("Avg Dist: ");
    lcd.print(averageDistance, 1); // Print one decimal place
    lcd.print(" cm    "); // Extra spaces to clear previously displayed characters

    // Reset the accumulator and counter
    distanceSum = 0;
  }
  
  // Delay a little before the next reading
  delay(500);
}
