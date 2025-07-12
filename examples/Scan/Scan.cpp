/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
 
#include "ReactiveArduinoLib.h"
using namespace Reactive;

int values[] = { 1, 2, 3, 4, 5 };
int valuesLength = sizeof(values) / sizeof(values[0]);

// Accumulator function for sum calculation
int addValues(int acc, int value) {
    return acc + value;
}

// Accumulator function for product calculation
int multiplyValues(int acc, int value) {
    return acc * value;
}

// Action to print intermediate scan results
void printScanResult(int x) {
    Serial.print("Scan result: ");
    Serial.println(x);
}

// Action when scan is complete
void printScanComplete() {
    Serial.println("Scan complete!");
    Serial.println();
}

// Example showing running average calculation
float runningAverageValues[] = { 10.0, 20.0, 30.0, 40.0, 50.0 };
int avgValuesLength = sizeof(runningAverageValues) / sizeof(runningAverageValues[0]);
int valueCount = 0;

// Running average accumulator
float calculateRunningAverage(float acc, float value) {
    valueCount++;
    return ((acc * (valueCount - 1)) + value) / valueCount;
}

// Action to print running average
void printRunningAverage(float x) {
    Serial.print("Running average: ");
    Serial.println(x, 2);
}

// Action when running average is complete
void printAverageComplete() {
    Serial.println("Running average complete!");
    Serial.println();
    valueCount = 0; // Reset for next iteration
}

void setup()
{
    Serial.begin(115200);
    while (!Serial) delay(1);
    
    Serial.println("=== Scan Operator Example ===");
    Serial.println("Scan emits intermediate accumulated results");
    Serial.println("Unlike Reduce which only emits the final result");
    Serial.println();
}

void loop()
{
    Serial.println("Example 1: Running Sum with Scan");
    Serial.println("Values: 1, 2, 3, 4, 5 (starting with 0)");
    
    // Scan emits: 1, 3, 6, 10, 15
    FromArray(values, valuesLength)
    .Scan<int>(0, addValues)  // Start with 0, add each value
    .DoAndFinally(printScanResult, printScanComplete);
    
    delay(1000);
    
    Serial.println("Example 2: Running Product with Scan");
    Serial.println("Values: 1, 2, 3, 4, 5 (starting with 1)");
    
    // Scan emits: 1, 2, 6, 24, 120
    FromArray(values, valuesLength)
    .Scan<int>(1, multiplyValues)  // Start with 1, multiply each value
    .DoAndFinally(printScanResult, printScanComplete);
    
    delay(1000);
    
    Serial.println("Example 3: Running Average with Scan");
    Serial.println("Values: 10.0, 20.0, 30.0, 40.0, 50.0");
    
    // Reset counter for this example
    valueCount = 0;
    
    // Scan emits running averages: 10.0, 15.0, 20.0, 25.0, 30.0
    FromArray(runningAverageValues, avgValuesLength)
    .Scan<float>(0.0f, calculateRunningAverage)
    .DoAndFinally(printRunningAverage, printAverageComplete);
    
    delay(3000);
    Serial.println("========================================");
}
