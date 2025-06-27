//Airotix Pms5003_Calliberation
#include <SoftwareSerial.h>

//PMS5003 is connected to TX to pin 8, RX to pin 9
SoftwareSerial pmsSerial(8, 9);

// Data structure to store sensor output
struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um;
  uint16_t particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

pms5003data data;

void setup() {
  Serial.begin(9600);
  pmsSerial.begin(9600);
}

void loop() {
  if (readPMSdata(&pmsSerial)) {
    // Only print if data passed integrity checks
    Serial.println("---- PM Sensor Readings ----");
    Serial.print("PM1.0: "); Serial.print(data.pm10_standard); Serial.println(" µg/m³");
    Serial.print("PM2.5: "); Serial.print(data.pm25_standard); Serial.println(" µg/m³");
    Serial.print("PM10 : "); Serial.print(data.pm100_standard); Serial.println(" µg/m³");
    Serial.println("----------------------------");
  }
  delay(1000);
}

bool readPMSdata(Stream *s) {
  // 1. Wait for start byte 0x42 (packet begins here)
  if (!s->available()) return false;
  if (s->peek() != 0x42) {
    s->read(); // Discard bad byte and wait for proper frame
    return false;
  }

  // 2. Make sure we have full 32-byte frame
  if (s->available() < 32) return false;

  uint8_t buffer[32];
  s->readBytes(buffer, 32);  // Read entire frame

  // 3. Calculate checksum (sum of first 30 bytes)
  uint16_t checksum_calc = 0;
  for (uint8_t i = 0; i < 30; i++) {
    checksum_calc += buffer[i];
  }

  // 4. Parse 16-bit words (big endian)
  uint16_t buffer_u16[15];
  for (uint8_t i = 0; i < 15; i++) {
    buffer_u16[i] = (buffer[2 + i * 2] << 8) + buffer[2 + i * 2 + 1];
  }

  // 5. Copy parsed values into data struct
  memcpy(&data, buffer_u16, sizeof(data));

  // 6. Verify checksum
  if (checksum_calc != data.checksum) {
    Serial.println("Checksum mismatch – discarding reading.");
    return false;
  }

  // All good!
  return true;
}
