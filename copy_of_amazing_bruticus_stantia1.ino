#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

// Pin configurations
#define MOTOR_PIN 3
#define POTENTIOMETER_PIN A0
#define GAS_SENSOR_PIN A1
#define ALCOHOL_INDICATOR_PIN 10 // Pin for NeoPixel strip
#define ALCOHOL_THRESHOLD 500    // Alcohol threshold for gas sensor
int alcoholSpeedLimit = 30;   // Speed limit in km/h when alcohol is detected

// Initialize the NeoPixel strip (number of pixels and pin number)
#define NUMPIXELS 4
Adafruit_NeoPixel strip(NUMPIXELS, ALCOHOL_INDICATOR_PIN, NEO_GRB + NEO_KHZ800);

// Initialize the LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int potValue = 0;        // Value from potentiometer
int alcoholLevel = 0;    // Value from gas sensor
int motorSpeed = 0;      // Motor speed in km/h
int normalSpeedLimit = 100; // Default speed limit in km/h (modifiable)
int currentSpeedLimit = normalSpeedLimit; // Active speed limit

String inputCommand = "";  // To store input from the serial monitor
String inputPassword = ""; // To store password input
String correctPassword = "ABCD"; // Hardcoded password
bool isAuthenticated = false; // Authentication status

void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
  lcd.begin(16, 2); // Initialize 16x2 LCD

  // Initialize the NeoPixel strip
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // Initial LCD Display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000); // Pause to show the initializing message
  lcd.clear();

  Serial.begin(9600); // Initialize Serial Monitor for input
  lcd.setCursor(0, 0);
  lcd.print("Enter password...");
}

void loop() {
  // Check if password is entered for authentication
  if (!isAuthenticated) {
    // Listen for Serial input and update the inputPassword
    if (Serial.available() > 0) {
      inputPassword = Serial.readStringUntil('\n'); // Read the input password
      inputPassword.trim();  // Remove any unwanted spaces or newline characters

      // Check if password is correct
      if (inputPassword == correctPassword) {
        isAuthenticated = true;
        Serial.println("Authenticated!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Authenticated!");
        delay(2000);  // Pause to show authenticated message
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter command...");
      } else {
        Serial.println("Incorrect password. Try again.");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Incorrect password");
        delay(2000);  // Pause to show incorrect message
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter password...");
      }
    }
    return; // Wait for authentication before continuing
  }

  // Listen for Serial input and update the inputCommand
  if (Serial.available() > 0) {
    inputCommand = Serial.readStringUntil('\n'); // Read the input command
    inputCommand.trim();  // Remove any unwanted spaces or newline characters

    // Check if the input is a valid command
    if (inputCommand.startsWith("SET LIMIT")) {
      // Extract the speed limit value
      int newSpeedLimit = inputCommand.substring(10).toInt(); // Get the value after "SET LIMIT "
      
      if (newSpeedLimit > 0 && newSpeedLimit <= 120) {  // Check if speed is within valid range
        normalSpeedLimit = newSpeedLimit; // Update the normal speed limit
        Serial.print("New speed limit set: ");
        Serial.println(normalSpeedLimit);
      } else {
        Serial.println("Invalid speed limit. Please set between 1 and 120.");
      }
    } else {
      Serial.println("Invalid command. Use 'SET LIMIT <value>' to set speed.");
    }
  }

  // Read the potentiometer and gas sensor
  potValue = analogRead(POTENTIOMETER_PIN);
  alcoholLevel = analogRead(GAS_SENSOR_PIN);

  // Convert potentiometer value to speed
  motorSpeed = map(potValue, 0, 1023, 0, 120);

  // Check alcohol level
  if (alcoholLevel > ALCOHOL_THRESHOLD) {
    currentSpeedLimit = alcoholSpeedLimit; // Set speed limit to alcohol mode
    turnOnAlcoholIndicator(); // Turn on the LED strip
  } else {
    currentSpeedLimit = normalSpeedLimit;  // Default speed limit
    turnOffAlcoholIndicator();  // Turn off the LED strip
  }

  // Enforce speed limit
  motorSpeed = min(motorSpeed, currentSpeedLimit);

  // Display speed and alcohol status on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Speed: ");
  lcd.print(motorSpeed);
  lcd.print(" km/h");

  lcd.setCursor(0, 1);
  if (alcoholLevel > ALCOHOL_THRESHOLD) {
    lcd.print("Alcohol Detected");
  } else {
    lcd.print("Normal");
  }

  // Output motor speed as PWM
  analogWrite(MOTOR_PIN, map(motorSpeed, 0, 120, 0, 255));

  // Debugging via Serial Monitor
  Serial.print("Potentiometer: ");
  Serial.print(potValue);
  Serial.print(" | Alcohol: ");
  Serial.print(alcoholLevel);
  Serial.print(" | Speed: ");
  Serial.println(motorSpeed);

  delay(500); // Update every 500ms
}

// Function to turn on the NeoPixel strip when alcohol is detected
void turnOnAlcoholIndicator() {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red color for alcohol detection
  }
  strip.show();
}

// Function to turn off the NeoPixel strip
void turnOffAlcoholIndicator() {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Off color (black)
  }
  strip.show();
}
