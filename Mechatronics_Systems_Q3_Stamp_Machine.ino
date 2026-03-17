/*
 * ============================================================
 *
 * Name: Christopher Redfern
 * BEng Electrical and Electronics Engineering
 * University of Lincoln
 * ELE2003M - Mechatronics Systems
 * Question 3: Automatic Stamp Machine
 *
 * ============================================================
 *
 * Description:
 *   Converts servo rotational motion into a linear stamping
 *   action using a crank-slider mechanical linkage.
 *   The servo sweeps between a minimum and maximum angle,
 *   driving the stamp arm up and down continuously.
 *
 *   Optional: Potentiometer to adjust the sweep range
 *   Optional: LCD display to show the current range setting
 *
 * Hardware:
 *   - Arduino (Freenove)
 *   - Servo Motor (SG90) -> Signal to Pin 9 (PWM)
 *   - Potentiometer (optional) -> A0
 *   - LCD 16x2 (optional) -> Pins 2,3,4,5,6,7
 *
 * Servo Wiring:
 *   - Orange wire (Signal) -> Pin 9
 *   - Red wire   (Power)   -> 5V
 *   - Brown wire (Ground)  -> GND
 * ============================================================
 */

#include <Servo.h>   // Include Arduino Servo library

// --- Uncomment the line below if you have an LCD connected ---
// #define LCD_CONNECTED

#ifdef LCD_CONNECTED
  #include <LiquidCrystal.h>
  // LCD pins: RS, EN, D4, D5, D6, D7
  LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
#endif

// --- Pin Definitions ---
const int SERVO_PIN = 9;   // Servo signal wire (must be PWM pin)
const int POT_PIN   = A0;  // Optional potentiometer for range control

// --- Servo Object ---
Servo stampServo;

// --- Motion Parameters ---
int minAngle  = 30;   // Minimum servo angle (stamp raised position)
int maxAngle  = 150;  // Maximum servo angle (stamp lowered/stamping position)
int stepDelay = 15;   // Delay between each degree of movement (controls speed)
                      // Lower = faster stamp, Higher = slower stamp


// ============================================================
// SETUP - Runs once when Arduino powers on
// ============================================================
void setup() {
  // Attach the servo to its pin
  stampServo.attach(SERVO_PIN);

  // Start serial monitor for debugging
  Serial.begin(9600);
  Serial.println("Stamp Machine Started");

  // Move servo to starting (raised) position
  stampServo.write(minAngle);
  delay(500);

  #ifdef LCD_CONNECTED
    // Initialise the LCD
    lcd.begin(16, 2);
    lcd.print("Stamp Machine");
    lcd.setCursor(0, 1);
    lcd.print("Ready!");
    delay(2000);
    lcd.clear();
  #endif
}


// ============================================================
// LOOP - Runs repeatedly after setup()
// ============================================================
void loop() {

  // Step 1: Read potentiometer to set the sweep range (if connected)
  // Map pot value (0-1023) to a usable angle range (10 to 80 degrees of sweep)
  int potValue  = analogRead(POT_PIN);
  int sweepRange = map(potValue, 0, 1023, 10, 80);

  // Calculate actual min/max angles based on sweep range around centre (90 degrees)
  minAngle = 90 - sweepRange;
  maxAngle = 90 + sweepRange;

  // Print current settings to Serial Monitor
  Serial.print("Sweep Range: +/-");
  Serial.print(sweepRange);
  Serial.print(" degrees | Min: ");
  Serial.print(minAngle);
  Serial.print(" | Max: ");
  Serial.println(maxAngle);

  // Step 2: Update LCD with current range (if connected)
  #ifdef LCD_CONNECTED
    lcd.setCursor(0, 0);
    lcd.print("Range: ");
    lcd.print(sweepRange);
    lcd.print(" deg   ");

    lcd.setCursor(0, 1);
    lcd.print("Min:");
    lcd.print(minAngle);
    lcd.print(" Max:");
    lcd.print(maxAngle);
    lcd.print("  ");
  #endif

  // Step 3: Sweep servo DOWN (raise to lower = stamp action)
  Serial.println("Stamping DOWN...");
  for (int angle = minAngle; angle <= maxAngle; angle++) {
    stampServo.write(angle);
    delay(stepDelay);
  }

  // Brief pause at the bottom of the stamp (contact point)
  delay(200);

  // Step 4: Sweep servo UP (return stamp to raised position)
  Serial.println("Returning UP...");
  for (int angle = maxAngle; angle >= minAngle; angle--) {
    stampServo.write(angle);
    delay(stepDelay);
  }

  // Brief pause at top before next stamp
  delay(300);
}
