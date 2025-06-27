const int MQ135_PIN = A0;   // Analog pin connected to MQ135
float R0 = 1.0;             // Will hold calibrated baseline resistance (R0)

// Calibration in clean air will update this
void setup() {
  Serial.begin(9600);
  Serial.println("MQ135 General Air Quality Calibration");

  delay(3000);  // Let sensor stabilize for a few seconds
  R0 = calibrateSensor();  // Get baseline resistance in clean air

  Serial.print("Calibrated R0 = ");
  Serial.println(R0);
}

void loop() {
  float RS = getResistance();   // Real-time sensor resistance
  float ratio = RS / R0;        // Normalize against baseline

  // Log ratio (used to estimate pollution level)
  Serial.print("Gas Ratio (RS/R0): ");
  Serial.print(ratio, 2);

  // Qualitative interpretation
  if (ratio > 3.0) {
    Serial.println(" → Very Clean Air");
  } else if (ratio > 1.5) {
    Serial.println(" → Clean Air");
  } else if (ratio > 1.0) {
    Serial.println(" → Slight Pollution");
  } else if (ratio > 0.5) {
    Serial.println(" → High Pollution");
  } else {
    Serial.println(" → Extremely Polluted Air!");
  }

  delay(2000);  // Update every 2 seconds
}

float calibrateSensor() {
  const int samples = 100;
  float total = 0.0;

  Serial.println("Calibrating in clean air...");

  for (int i = 0; i < samples; i++) {
    total += getResistance();
    delay(100);
  }

  return total / samples;
}

float getResistance() {
  float voltage = analogRead(MQ135_PIN) * (5.0 / 1023.0);  // Convert ADC to voltage
  return (5.0 - voltage) / voltage;  // Apply voltage divider formula
}
