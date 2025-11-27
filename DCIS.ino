#include <Wire.h>
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

const double SPEED_OF_LIGHT = 299792458.0; 
const double G = 6.674e-11;                 

unsigned long hours = 0;
unsigned long minutes = 0;
unsigned long seconds = 0;
unsigned long milliseconds = 0;
unsigned long lastUpdate = 0;
unsigned long lastMillisUpdate = 0;

int prevDisplayHours = -1;
int prevMinutes = -1;
int prevSeconds = -1;
int prevMilliseconds = -1;
String prevAMPM = "";

double velocity = 0.0;
double mass = 0.0;
double distance = 0.0;
double dilationFactor = 1.0;
double adjustedDelay = 1.0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Relativistic");
  lcd.setCursor(0, 1);
  lcd.print("Clock Starting..");
  delay(2000);
  lcd.clear();

  Serial.println(F("========================================"));
  Serial.println(F("  RELATIVISTIC DIGITAL CLOCK"));
  Serial.println(F("========================================"));
  Serial.println();

  getInitialTime();
  selectScenario();
  calculateTimeDilation();

  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F("CALCULATED TIME DILATION:"));
  Serial.print(F("  Dilation Factor: "));
  Serial.println(dilationFactor, 10);
  Serial.print(F("  Adjusted Delay: "));
  Serial.print(adjustedDelay, 6);
  Serial.println(F(" ms"));
  Serial.println(F("========================================"));
  Serial.println();

  lastUpdate = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  
  unsigned long realTimeElapsed = currentMillis - lastMillisUpdate;
  
  double dilatedTimeMs = realTimeElapsed / dilationFactor;
  
  if (dilatedTimeMs >= 50.0) {
    lastMillisUpdate = currentMillis;
    
    unsigned long msToAdd = ((unsigned long)(dilatedTimeMs / 50.0)) * 50;
    milliseconds += msToAdd;
    
    while (milliseconds >= 1000) {
      milliseconds -= 1000;
      seconds++;

      if (seconds >= 60) {
        seconds = 0;
        minutes++;

        if (minutes >= 60) {
          minutes = 0;
          hours++;

          if (hours >= 24) {
            hours = 0;
          }
        }
      }
    }
    displayTime();
  }
}


void getInitialTime() {
  bool validInput = false;
  while (!validInput) {
    Serial.println(F("Enter initial time in 24-hour format (HH:MM:SS):"));
    Serial.println(F("Example: 14:30:00"));

    while (Serial.available() == 0);
    String timeStr = Serial.readStringUntil('\n');
    timeStr.trim();

    int firstColon = timeStr.indexOf(':');
    int secondColon = timeStr.indexOf(':', firstColon + 1);

    if (firstColon > 0 && secondColon > firstColon) {
      hours = timeStr.substring(0, firstColon).toInt();
      minutes = timeStr.substring(firstColon + 1, secondColon).toInt();
      seconds = timeStr.substring(secondColon + 1).toInt();
      milliseconds = 0;

      if ((hours >= 0 && hours <= 23) &&
          (minutes >= 0 && minutes <= 59) &&
          (seconds >= 0 && seconds <= 59)) {
        Serial.print(F("Time set to: "));
        if (hours < 10) Serial.print("0");
        Serial.print(hours);
        Serial.print(":");
        if (minutes < 10) Serial.print("0");
        Serial.print(minutes);
        Serial.print(":");
        if (seconds < 10) Serial.print("0");
        Serial.println(seconds);
        validInput = true;
      } else {
        Serial.println(F("Invalid Input, please try again.\n"));
      }
    } else {
      Serial.println(F("Invalid format, please try again.\n"));
    }
  }
  Serial.println();
}

void selectScenario() {
  bool validChoice = false;
  while (!validChoice) {
    Serial.println(F("Select scenario (1-5):"));
    Serial.println(F("1. Earth Surface"));
    Serial.println(F("2. Sun Surface"));
    Serial.println(F("3. Neutron Star Surface"));
    Serial.println(F("4. Close to lightspeed, without mass interference"));
    Serial.println(F("5. Custom"));

    while (Serial.available() == 0);
    String choiceStr = Serial.readStringUntil('\n');
    choiceStr.trim();
    int choice = choiceStr.toInt();

    switch (choice) {
      case 1:
        velocity = 465.1;
        mass = 5.972e24;
        distance = 6.371e6;
        Serial.println(F("Selected: Earth Surface"));
        validChoice = true;
        break;

      case 2:
        velocity = 0.0;
        mass = 1.989e30;
        distance = 6.9634e8;
        Serial.println(F("Selected: Sun Surface"));
        validChoice = true;
        break;

      case 3:
        velocity = 0.0;
        mass = 2.8e30;
        distance = 1.0e4;
        Serial.println(F("Selected: Neutron Star Surface"));
        validChoice = true;
        break;

      case 4:
        velocity = 0.9 * SPEED_OF_LIGHT;
        mass = 0.0;
        distance = 1.0;
        Serial.println(F("Selected: Close to lightspeed (90% c)"));
        validChoice = true;
        break;

      case 5:
        Serial.println(F("Selected: Custom"));
        getCustomParameters();
        validChoice = true;
        break;

      default:
        Serial.println(F("Invalid Input, please try again.\n"));
        break;
    }
  }

  Serial.print(F("  Velocity: "));
  Serial.print(velocity, 2);
  Serial.println(F(" m/s"));
  Serial.print(F("  Mass: "));
  printScientific(mass);
  Serial.println(F(" kg"));
  Serial.print(F("  Distance: "));
  printScientific(distance);
  Serial.println(F(" m"));
  Serial.println();
}

void getCustomParameters() {
  while (true) {
    Serial.println(F("Enter current velocity (m/s):"));
    Serial.println(F("Supports: 3e8, 3*10^8, or 300000000"));

    while (Serial.available() == 0);
    String velStr = Serial.readStringUntil('\n');
    velStr.trim();
    velocity = parseScientific(velStr);

    if (velocity < 0) {
      Serial.println(F("Invalid Input, please try again.\n"));
      continue;
    }
    Serial.print(F("Velocity set to: "));
    printScientific(velocity);
    Serial.println(F(" m/s"));
    Serial.print(F("  ("));
    Serial.print((velocity / SPEED_OF_LIGHT) * 100, 4);
    Serial.println(F("% of speed of light)\n"));
    break;
  }

  while (true) {
    Serial.println(F("Mass of gravitational body (kg):"));
    Serial.println(F("Example: 5.972e24 (Earth), 1.989e30 (Sun)"));

    while (Serial.available() == 0);
    String massStr = Serial.readStringUntil('\n');
    massStr.trim();
    mass = parseScientific(massStr);

    if (mass < 0) {
      Serial.println(F("Invalid Input, please try again.\n"));
      continue;
    }
    Serial.print(F("Mass set to: "));
    printScientific(mass);
    Serial.println(F(" kg\n"));
    break;
  }

  while (true) {
    Serial.println(F("Distance from center of mass (m):"));
    Serial.println(F("Example: 6.371e6 (Earth surface), 1.496e11 (Earth-Sun)"));

    while (Serial.available() == 0);
    String distStr = Serial.readStringUntil('\n');
    distStr.trim();
    distance = parseScientific(distStr);

    if (distance < 0) {
      Serial.println(F("Invalid Input, please try again.\n"));
      continue;
    }
    Serial.print(F("Distance set to: "));
    printScientific(distance);
    Serial.println(F(" m\n"));
    break;
  }
}

double parseScientific(String str) {
  str.trim();
  str.replace(" ", "");
  str.toLowerCase();

  int expPos = str.indexOf("^");
  if (expPos > 0 && str.indexOf("*") > 0) {
    double base = str.substring(0, str.indexOf("*")).toDouble();
    double exponent = str.substring(expPos + 1).toDouble();
    return base * pow(10.0, exponent);
  }

  int ePos = str.indexOf('e');
  if (ePos > 0) {
    double base = str.substring(0, ePos).toDouble();
    double exponent = str.substring(ePos + 1).toDouble();
    return base * pow(10.0, exponent);
  }

  return str.toDouble();
}

void printScientific(double value) {
  if (value == 0.0) {
    Serial.print("0.00");
    return;
  }
  

  int exponent = 0;
  double absValue = abs(value);
  
  if (absValue >= 10.0) {
    while (absValue >= 10.0) {
      absValue /= 10.0;
      exponent++;
    }
  } else if (absValue < 1.0) {
    while (absValue < 1.0) {
      absValue *= 10.0;
      exponent--;
    }
  }
  
  if (value < 0) Serial.print("-");
  Serial.print(absValue, 3);
  
  if (exponent != 0) {
    Serial.print("e");
    Serial.print(exponent);
  }
}

void calculateTimeDilation() {
  double velocityRatio = velocity / SPEED_OF_LIGHT;
  double velocityFactor = sqrt(1.0 - velocityRatio * velocityRatio);

  double schwarzschildTerm = (2.0 * G * mass) / (distance * SPEED_OF_LIGHT * SPEED_OF_LIGHT);
  double gravitationalFactor = sqrt(1.0 - schwarzschildTerm);

  dilationFactor = velocityFactor * gravitationalFactor;
  
  adjustedDelay = dilationFactor;

  if (adjustedDelay < 0.01) adjustedDelay = 0.01;  // Max 100x faster
  if (adjustedDelay > 10.0) adjustedDelay = 10.0;  // Max 10x slower

  Serial.print(F("Velocity Factor: "));
  Serial.println(velocityFactor, 10);
  Serial.print(F("Gravitational Factor: "));
  Serial.println(gravitationalFactor, 10);
  Serial.print(F("Clock speed: "));
  Serial.print(1.0 / dilationFactor, 4);
  Serial.println(F("x normal speed"));
}


void displayTime() {
  int displayHours = hours;
  String ampm = "AM";

  if (displayHours >= 12) {
    ampm = "PM";
    if (displayHours > 12) displayHours -= 12;
  }
  if (displayHours == 0) displayHours = 12;

  if (displayHours != prevDisplayHours || minutes != prevMinutes || seconds != prevSeconds) {
    lcd.setCursor(0, 0);
    if (displayHours < 10) lcd.print("0");
    lcd.print(displayHours);
    lcd.print(":");
    if (minutes < 10) lcd.print("0");
    lcd.print(minutes);
    lcd.print(":");
    if (seconds < 10) lcd.print("0");
    lcd.print(seconds);
    lcd.print(":");

    prevDisplayHours = displayHours;
    prevMinutes = minutes;
    prevSeconds = seconds;
  }

  if (milliseconds != prevMilliseconds) {
    lcd.setCursor(9, 0);
    if (milliseconds < 100) lcd.print("0");
    if (milliseconds < 10) lcd.print("0");
    lcd.print(milliseconds);
    prevMilliseconds = milliseconds;
  }

  String currentAMPM = ampm;
  if (currentAMPM != prevAMPM) {
    lcd.setCursor(13, 0);
    lcd.print("  ");
    lcd.setCursor(13, 0);
    lcd.print(ampm);
    prevAMPM = currentAMPM;
  }

  lcd.setCursor(0, 1);
  lcd.print("DF:");
  lcd.print(dilationFactor, 6);
  int usedChars = 3 + 8;
  for (int i = usedChars; i < 16; i++) lcd.print(" ");
}