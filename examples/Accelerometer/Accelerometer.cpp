/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "ReactiveArduinoLib.h"
using namespace Reactive;

// Helper functions for type conversion
float getMagnitude(AccelerometerData data) {
	return data.magnitude;
}

float getTiltAngle(AccelerometerData data) {
	return atan2(data.y, data.x) * 180.0 / PI;
}

bool magnitudeThreshold(float magnitude) {
	return magnitude > 1.2;
}

void printStepCount(float magnitude) {
	static int stepCount = 0;
	stepCount++;
	Serial.print("Steps counted: ");
	Serial.println(stepCount);
}

void printTiltBrightness(float mappedAngle) {
	int intAngle = (int)mappedAngle;
	Serial.print("Tilt mapped to LED brightness: ");
	Serial.println(intAngle);
	analogWrite(9, intAngle);
}

// Example demonstrating accelerometer observable and scan operator
auto accelerometer = Reactive::ObservableAccelerometer<AccelerometerData>(A3, A4, A5, 100); // X, Y, Z pins, 100ms interval

void setup()
{
	Serial.begin(115200);

	// Configure accelerometer
	accelerometer.SetSensitivity(3.3);  // 3.3V sensitivity
	accelerometer.SetZeroOffset(0.0, 0.0, 0.0);

	// Step counter using scan operator
	accelerometer
	.Select(getMagnitude)
	.Where(magnitudeThreshold)
	.DistinctUntilChanged()  // Only count distinct movements
	.Do(printStepCount);

	// Tilt detection
	accelerometer
	.Select(getTiltAngle)
	.Scale(-90.0, 90.0, 0, 255)  // Map angle to 0-255 range
	.Do(printTiltBrightness);
}

void loop()
{
	accelerometer.Update();
}
