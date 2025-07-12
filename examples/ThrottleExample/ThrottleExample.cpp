/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "ReactiveArduinoLib.h"
using namespace Reactive;

// Example demonstrating new throttle operator
auto timer = IntervalMillis(50); // Fast timer - 20Hz
const int LED_BUILT_IN = 13;

void setup()
{
	Serial.begin(115200);
	pinMode(LED_BUILT_IN, OUTPUT);

	// Throttle the fast timer to only emit every 500ms
	timer
	.Throttle(500)  // New operator: only emit once every 500ms
	.Toggle()
	.ToDigitalOutput(LED_BUILT_IN);

	// Also send throttled messages to serial
	timer
	.Throttle(1000)  // Different throttle rate for serial
	.Do([](unsigned long value) {
		Serial.print("Throttled value: ");
		Serial.println(value);
	});
}

void loop()
{
	timer.Update();
}
