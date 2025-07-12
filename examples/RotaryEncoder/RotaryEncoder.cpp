/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "ReactiveArduinoLib.h"
using namespace Reactive;

// Example demonstrating rotary encoder observable
auto encoder = ObservableRotaryEncoder<RotaryEncoderData>(2, 3, 4); // pin A, pin B, button pin

void setup()
{
	Serial.begin(115200);

	// Menu navigation system
	encoder
	.Where([](RotaryEncoderData data) { return data.direction != 0; })
	.Map<int>([](RotaryEncoderData data) { return data.position; })
	.Scale(-10, 10, 0, 100)  // Map encoder range to menu range
	.Do([](int menuPosition) {
		Serial.print("Menu position: ");
		Serial.println(menuPosition);
	});

	// Button press detection
	encoder
	.Where([](RotaryEncoderData data) { return data.buttonPressed; })
	.Debounce(100)  // Debounce button presses
	.Do([](RotaryEncoderData data) {
		Serial.println("Button pressed! Menu item selected.");
	});

	// Speed-based actions
	encoder
	.Map<int>([](RotaryEncoderData data) { return abs(data.direction); })
	.StartWith(0)  // Start with zero speed
	.Scan<int>(0, [](int speed, int direction) { 
		return direction > 0 ? min(speed + 1, 10) : max(speed - 1, 0); 
	})
	.Do([](int speed) {
		if (speed > 5) {
			Serial.println("Fast rotation detected!");
		}
	});
}

void loop()
{
	encoder.Update();
}
