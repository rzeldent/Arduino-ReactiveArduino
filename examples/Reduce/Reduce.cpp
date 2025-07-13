/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
 
#include "ReactiveArduinoLib.h"
using namespace Reactive;

int values[] = { 0, 1, 4, 6, 2, 5, 7, 3, 5, 8 };
int valuesLength = sizeof(values) / sizeof(values[0]);

// Reduce function: Start with a float value of 10.0 and add 1.5 times the integer value
float accumulateValues(float acc, int value) {
    return acc + value * 1.5f;
}

// Action to print the final reduced value
void printFinalResult(float x) {
    Serial.print("Final reduced result: ");
    Serial.println(x);
}

// Action when reduction is complete
void printComplete() {
    Serial.println("Reduction complete!");
}

void setup()
{
	Serial.begin(115200);
	while (!Serial) delay(1);
	
	Serial.println("=== Reduce Operator Example ===");
	Serial.println("Values: 0, 1, 4, 6, 2, 5, 7, 3, 5, 8");
	Serial.println("Formula: acc + value * 1.5, starting with 10.0");
	Serial.println();
}

void loop()
{
	Serial.println("Running reduction...");
	
	// Using ReduceToFinal - only emits the final accumulated result
	FromArray(values, valuesLength)
    .Reduce(accumulateValues, 10.0f)
	.DoAndFinally(printFinalResult, printComplete);

	Serial.println();
	delay(3000);
}