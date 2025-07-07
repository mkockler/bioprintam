#include <math.h>

const int numThermistors = 4;
const int thermistorPins[numThermistors] = {A0, A1, A2, A3};

// Constants for all thermistors (adjust if needed)
const float seriesResistor = 10000.0;
const float thermistorNominal = 10000.0;
const float temperatureNominal = 25.0;
const float betaCoefficient = 3950.0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < numThermistors; i++) {
    int adcValue = analogRead(thermistorPins[i]);

    // Prevent division by zero or invalid readings
    if (adcValue <= 0 || adcValue >= 1023) {
      Serial.print("Thermistor ");
      Serial.print(i);
      Serial.println(": ADC out of range");
      continue;
    }

    // Convert ADC to resistance
    float voltageRatio = (1023.0 / adcValue) - 1.0;
    float resistance = seriesResistor / voltageRatio;

    // Steinhart-Hart equation
    float steinhart;
    steinhart = resistance / thermistorNominal;      // (R/Ro)
    steinhart = log(steinhart);                      // ln(R/Ro)
    steinhart /= betaCoefficient;                    // 1/B * ln(R/Ro)
    steinhart += 1.0 / (temperatureNominal + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                     // Invert
    steinhart -= 273.15;                             // Convert from K to °C

    // Print temperature
    Serial.print("Thermistor ");
    Serial.print(i);
    Serial.print(" (A");
    Serial.print(thermistorPins[i] - A0);
    Serial.print("): ");
    Serial.print(steinhart, 1);
    Serial.println(" °C");
  }

  Serial.println("-------------------------");
  delay(1000);
}
