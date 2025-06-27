// Calibration values
float NO2_BASELINE = 0.0;
float CO_BASELINE = 0.0;
const int CALIBRATION_DURATION = 30000; // 30 seconds

// Smoothing variables
float smoothedNO2 = 0.0;
float smoothedCO = 0.0;
const float SMOOTHING_FACTOR = 0.2; 

void setup() {
  pinMode(PRE_PIN, OUTPUT);
  Serial.begin(19200);
  
  while (!Serial) {
    ; // Wait for serial port
  }

  Serial.println("MiCS-4514 Air Quality Monitor");
  
  // Pre-heat sequence
  Serial.println("Pre-heating sensor (90s)...");
  digitalWrite(PRE_PIN, HIGH);
  for (int i = 0; i < 90; i++) {
    delay(1000);
    if (i % 10 == 0) Serial.print(".");
  }
  
  // Calibration
  Serial.println("\nCalibrating in clean air...");
  calibrateSensor();
  Serial.println("Calibration complete!");
  Serial.print("Baselines - NO2: ");
  Serial.print(NO2_BASELINE, 4);
  Serial.print(" ppm, CO: ");
  Serial.print(CO_BASELINE, 4);
  Serial.println(" ppm");
  Serial.println("------------------------");
}

void calibrateSensor() {
  float no2Sum = 0.0;
  float coSum = 0.0;
  int readings = 0;
  
  unsigned long startTime = millis();
  while (millis() - startTime < CALIBRATION_DURATION) {
    no2Sum += analogRead(NOX_PIN) * 0.000978;
    coSum += analogRead(RED_PIN) * 0.18; // Adjusted multiplier
    readings++;
    delay(100);
  }
  
  NO2_BASELINE = no2Sum / readings;
  CO_BASELINE = coSum / readings;
  
  // Initialize smoothed values
  smoothedNO2 = NO2_BASELINE;
  smoothedCO = CO_BASELINE;
}

void loop() {
  // Read raw values
  float rawNO2 = (analogRead(NOX_PIN) * 0.000978) - NO2_BASELINE;
  float rawCO = (analogRead(RED_PIN) * 0.18) - CO_BASELINE; // Adjusted multiplier
  
  // Apply smoothing
  smoothedNO2 = (SMOOTHING_FACTOR * rawNO2) + ((1 - SMOOTHING_FACTOR) * smoothedNO2);
  smoothedCO = (SMOOTHING_FACTOR * rawCO) + ((1 - SMOOTHING_FACTOR) * smoothedCO);
  
  // Clean air threshold
  if (smoothedCO < 0.15) smoothedCO = 0.0;
  smoothedNO2 = max(smoothedNO2, 0.0); // Ensure never negative
  
  // Print results
  Serial.print("NO2: ");
  Serial.print(smoothedNO2, 3);
  Serial.print(" ppm\tCO: ");
  Serial.print(smoothedCO, 1);
  Serial.println(" ppm");
  
  delay(2000);
}