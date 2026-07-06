#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD address
Servo myservo;

// IR Sensor Pins
int IR1 = 2;   // Entry sensor
int IR2 = 3;   // Exit sensor

int Slot = 4;  // Total parking slots

bool entryDetected = false;
bool exitDetected = false;

void setup() {

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  myservo.attach(9);
  myservo.write(100); // gate closed (100° position)

  // Startup screen
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" ARDUINO PARKING ");
  lcd.setCursor(0,1);
  lcd.print("     SYSTEM      ");
  delay(2500);
  lcd.clear();
}

void loop() {

  int inSensor = digitalRead(IR1);   // 0 when object detected
  int outSensor = digitalRead(IR2);  // 0 when object detected

  Serial.print("IR1 = ");
  Serial.print(inSensor);
  Serial.print("   IR2 = ");
  Serial.println(outSensor);

  // ENTRY LOGIC
  if (inSensor == LOW && !entryDetected) {
    entryDetected = true;

    if (Slot > 0) {
      myservo.write(0);  // open gate
      Slot--;
      delay(1000);
    } else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  PARKING FULL ");
      lcd.setCursor(0,1);
      lcd.print("    SORRY :(    ");
      delay(2000);
      lcd.clear();
    }
  }

  // EXIT LOGIC
  if (outSensor == LOW && !exitDetected) {
    exitDetected = true;

    myservo.write(0);  // open gate
    Slot++;
    delay(1000);
  }

  // RESET & CLOSE GATE
  if (inSensor == HIGH && outSensor == HIGH) {
    entryDetected = false;
    exitDetected = false;
    myservo.write(100); // close gate
  }

  // LCD DISPLAY
  lcd.setCursor(0,0);
  lcd.print("  WELCOME USER  ");

  lcd.setCursor(0,1);
  lcd.print("Slots Left: ");
  lcd.print(Slot);

  delay(150);
}

