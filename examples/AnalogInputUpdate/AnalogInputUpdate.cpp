/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "ReactiveArduinoLib.h"
using namespace Reactive;

// ObservableAnalogInput Update() Method Example
// Demonstrates automatic timer-based analog input reading

// Create analog input observable with automatic 500ms update interval
auto analogSensor = ObservableAnalogInput<int>(A0, 500);

// Helper function for printing values
void printAnalogValue(int value) {
    Serial.print("Analog Value: ");
    Serial.print(value);
    Serial.print(" (");
    Serial.print((value / 1023.0) * 100.0);
    Serial.println("%)");
}

// Helper function for voltage conversion
float convertToVoltage(int adcValue) {
    return (adcValue / 1023.0) * 5.0;  // Assuming 5V reference
}

// Helper function for printing voltage
void printVoltage(float voltage) {
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println("V");
}

// Helper function for printing smoothed voltage
void printSmoothedVoltage(float smoothedVoltage) {
    Serial.print("Smoothed Voltage: ");
    Serial.print(smoothedVoltage);
    Serial.println("V (5-sample average)");
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) delay(1);
    
    Serial.println("=== ObservableAnalogInput Update() Example ===");
    Serial.println("Reading A0 every 500ms automatically using Update() method");
    Serial.println("Raw ADC values and voltage conversion will be displayed");
    
    // Chain 1: Display raw ADC values
    analogSensor
    .Do(printAnalogValue);
    
    // Chain 2: Convert to voltage and display
    analogSensor
    .Map<float>(convertToVoltage)
    .Do(printVoltage);
    
    // Chain 3: Demonstrate filtering with automatic updates
    analogSensor
    .Map<float>(convertToVoltage)
    .MovingAverage(5)  // 5-sample moving average
    .Do(printSmoothedVoltage);
    
    Serial.println("Setup complete. Automatic updates every 500ms...");
}

void loop()
{
    // Call Update() to trigger automatic timer-based reading
    analogSensor.Update();
    
    // You can also manually trigger readings using Next()
    // analogSensor.Next();  // Immediate reading, ignoring timer
    
    // Change interval dynamically if needed
    static unsigned long lastIntervalChange = 0;
    if (millis() - lastIntervalChange > 10000) {  // Every 10 seconds
        static bool fastMode = false;
        fastMode = !fastMode;
        
        if (fastMode) {
            analogSensor.SetInterval(100);  // Fast: 100ms
            Serial.println(">> Switched to FAST mode (100ms intervals)");
        } else {
            analogSensor.SetInterval(1000);  // Slow: 1000ms
            Serial.println(">> Switched to SLOW mode (1000ms intervals)");
        }
        
        lastIntervalChange = millis();
    }
    
    // The loop can do other work while analog reading happens automatically
    delay(10);  // Small delay to prevent tight loop
}
