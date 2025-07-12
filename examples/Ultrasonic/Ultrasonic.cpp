/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "ReactiveArduinoLib.h"
using namespace Reactive;

// Example demonstrating ultrasonic sensor observable
auto ultrasonic = ObservableUltrasonic<float>(7, 8, 250); // trigger pin 7, echo pin 8, 250ms interval

void setup()
{
	Serial.begin(115200);

	// Set maximum detection distance to 200cm
	ultrasonic.SetMaxDistance(200.0);
	
	// Apply Kalman filter for noise reduction
	ultrasonic
	.Kalman(0.1, 4.0)  // Process variance, measurement variance
	.Hysteresis(10.0, 20.0)  // Create hysteresis between 10cm and 20cm
	.Do([](float distance) {
		Serial.print("Filtered distance: ");
		Serial.print(distance);
		Serial.println(" cm");
	});

	// Create distance-based alerts
	ultrasonic
	.Where([](float distance) { return distance < 15.0; })
	.Throttle(1000)  // Only alert once per second
	.Do([](float distance) {
		Serial.println("WARNING: Object too close!");
	});
}

void loop()
{
	ultrasonic.Update();
}
