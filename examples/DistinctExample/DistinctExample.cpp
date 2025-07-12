/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "ReactiveArduinoLib.h"
using namespace Reactive;

// Example demonstrating the difference between Distinct and DistinctUntilChanged
int values[] = { 1, 2, 3, 2, 4, 1, 5, 3, 6, 2, 1 };
int valuesLength = sizeof(values) / sizeof(values[0]);

void setup()
{
    Serial.begin(115200);
    while (!Serial) delay(1);
    
    Serial.println("=== Distinct vs DistinctUntilChanged Demo ===");
    
    Serial.println("\nOriginal sequence:");
    FromArray(values, valuesLength)
    .Do([](int value) {
        Serial.print(value);
        Serial.print(" ");
    })
    .Finally([]() {
        Serial.println();
    });
    
    Serial.println("\nWith Distinct() - removes ALL duplicates:");
    FromArray(values, valuesLength)
    .Distinct()  // New operator: removes all duplicate values
    .Do([](int value) {
        Serial.print(value);
        Serial.print(" ");
    })
    .Finally([]() {
        Serial.println();
    });
    
    Serial.println("\nWith DistinctUntilChanged() - removes consecutive duplicates only:");
    FromArray(values, valuesLength)
    .DistinctUntilChanged()  // Existing operator: removes only consecutive duplicates
    .Do([](int value) {
        Serial.print(value);
        Serial.print(" ");
    })
    .Finally([]() {
        Serial.println();
    });
    
    Serial.println("\n=== Sensor Reading Example ===");
    Serial.println("Simulating sensor with noise and repeated readings...");
}

void loop()
{
    // Simulate sensor readings with noise and duplicates
    static int sensorReadings[] = { 23, 23, 24, 23, 25, 25, 24, 26, 26, 25, 27, 23, 23 };
    static int readingIndex = 0;
    static unsigned long lastReading = 0;
    
    if (millis() - lastReading > 500) // Read every 500ms
    {
        if (readingIndex < sizeof(sensorReadings) / sizeof(sensorReadings[0]))
        {
            int currentReading = sensorReadings[readingIndex];
            
            // Process with Distinct to get unique values only
            auto obs = Property<int>();
            obs.SetValue(currentReading);
            
            obs
            .Distinct()
            .Do([](int uniqueValue) {
                Serial.print("New unique sensor value: ");
                Serial.println(uniqueValue);
            });
            
            readingIndex++;
            lastReading = millis();
        }
        else
        {
            Serial.println("\nDemo complete. Restarting in 3 seconds...");
            delay(3000);
            readingIndex = 0;
        }
    }
}
