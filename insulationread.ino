#include <math.h>

const int numThermistors = 2;
const int thermistorPins[numThermistors] = {A0, A1};

// Constants for all thermistors
const float seriesResistor = 10000.0;
const float thermistorNominal = 10000.0;
const float temperatureNominal = 25.0;
const float betaCoefficient = 3950.0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  float temps[numThermistors];

  for (int i = 0; i < numThermistors; i++) {
    int adcValue = analogRead(thermistorPins[i]);

    // convert ADC to resistance
    float voltageRatio = (1023.0 / adcValue) - 1.0;
    float resistance = seriesResistor / voltageRatio;

    // Steinhart-Hart equation
    float steinhart;
    steinhart = resistance / thermistorNominal;      
    steinhart = log(steinhart);                      
    steinhart /= betaCoefficient;                    
    steinhart += 1.0 / (temperatureNominal + 273.15); 
    steinhart = 1.0 / steinhart;                     
    steinhart -= 273.15;                             

    temps[i] = steinhart;
  }

  // *** Print just CSV ***
  Serial.print(temps[0], 2);
  Serial.print(",");
  Serial.println(temps[1], 2);

  delay(1000);
}