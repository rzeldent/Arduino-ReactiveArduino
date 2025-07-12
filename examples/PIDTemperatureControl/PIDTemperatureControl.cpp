/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "ReactiveArduinoLib.h"
using namespace Reactive;

// PID Temperature Controller Example
// Hardware: Temperature sensor on A0, heater control on pin 9

auto tempSensor = ObservableAnalogInput(A0, 200);  // Read temperature every 200ms
auto pidController = FilterPID<float>(25.0, 2.0, 0.1, 0.5, 0, 255);  // Setpoint 25°C, Kp=2.0, Ki=0.1, Kd=0.5

const int HEATER_PIN = 9;
const int STATUS_LED = 13;

// Convert ADC reading to temperature (for typical thermistor)
float adcToTemperature(int adcValue) {
    float voltage = (adcValue / 1023.0) * 5.0;
    float resistance = (5.0 - voltage) / voltage * 10000.0;  // 10k pullup
    
    // Simplified temperature calculation (replace with your sensor's formula)
    float temperature = 25.0 - (resistance - 10000.0) / 100.0;
    return temperature;
}

void setup()
{
    Serial.begin(115200);
    pinMode(HEATER_PIN, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);
    
    Serial.println("PID Temperature Controller Starting...");
    Serial.println("Setpoint: 25°C");
    
    // Configure PID controller
    pidController.SetSampleTime(200);  // Match sensor reading interval
    pidController.SetOutputLimits(0, 255);  // PWM range
    
    // Temperature control system
    tempSensor
    .Select([](int adcValue) { return adcToTemperature(adcValue); })
    .Kalman(0.1, 1.0)  // Smooth temperature readings
    .PID(25.0, 2.0, 0.1, 0.5, 0, 255)  // PID control
    .Do([](float pidOutput) {
        // Apply PID output to heater
        analogWrite(HEATER_PIN, (int)pidOutput);
        
        // Status LED: on when heating
        digitalWrite(STATUS_LED, pidOutput > 10);
        
        // Debug output
        Serial.print("PID Output: ");
        Serial.print(pidOutput);
        Serial.print(" (");
        Serial.print((pidOutput / 255.0) * 100.0);
        Serial.println("% heating)");
    });
    
    // Separate monitoring chain for temperature display
    tempSensor
    .Select([](int adcValue) { return adcToTemperature(adcValue); })
    .Kalman(0.1, 1.0)
    .Throttle(1000)  // Display every second
    .Do([](float temperature) {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print("°C, Error: ");
        Serial.print(pidController.GetError());
        Serial.println("°C");
    });
}

void loop()
{
    tempSensor.Update();
    
    // Allow setpoint changes via serial input
    if (Serial.available()) {
        float newSetpoint = Serial.parseFloat();
        if (newSetpoint > 0 && newSetpoint < 50) {
            pidController.SetSetpoint(newSetpoint);
            Serial.print("New setpoint: ");
            Serial.println(newSetpoint);
        }
    }
}
