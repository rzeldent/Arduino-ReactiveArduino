/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#include "ReactiveArduinoLib.h"
using namespace Reactive;

// PID Motor Speed Controller Example
// Hardware: Encoder on pins 2&3, motor driver on pin 9, potentiometer on A0

auto encoder = ObservableRotaryEncoder<RotaryEncoderData>(2, 3);
auto potentiometer = AnalogInput(A0, 100);  // Read setpoint every 100ms
auto speedTimer = IntervalMillis(250);  // Calculate speed every 250ms

auto pidController = FilterPID<float>(0, 1.5, 0.2, 0.1, -255, 255);

const int MOTOR_PIN = 9;
const int DIR_PIN = 8;

int lastEncoderPosition = 0;
float currentSpeed = 0;  // RPM

void setup()
{
    Serial.begin(115200);
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(DIR_PIN, OUTPUT);
    
    Serial.println("PID Motor Speed Controller Starting...");
    
    // Speed measurement system
    speedTimer
    .Do([](unsigned long timestamp) {
        int currentPosition = encoder.GetPosition();
        int deltaPosition = currentPosition - lastEncoderPosition;
        
        // Calculate RPM (assuming 100 pulses per revolution)
        currentSpeed = (deltaPosition / 100.0) * (60.0 / 0.25);  // RPM
        lastEncoderPosition = currentPosition;
        
        // Feed current speed to PID controller
        pidController.OnNext(currentSpeed);
    });
    
    // Setpoint control via potentiometer
    potentiometer
    .Interpolate(0, 1023, 0, 100)  // Map to 0-100 RPM
    .DistinctUntilChanged()  // Only update when setpoint changes
    .Do([](int targetRPM) {
        pidController.SetSetpoint((float)targetRPM);
        Serial.print("Target RPM: ");
        Serial.println(targetRPM);
    });
    
    // PID output to motor control
    // Note: This would typically be connected via a separate observable
    // but for demonstration, we'll use a callback approach
    
    // Status monitoring
    speedTimer
    .Throttle(1000)  // Display every second
    .Do([](unsigned long timestamp) {
        Serial.print("Speed: ");
        Serial.print(currentSpeed);
        Serial.print(" RPM, Output: ");
        Serial.print(pidController.GetOutput());
        Serial.print(", Error: ");
        Serial.println(pidController.GetError());
        
        // Apply motor control
        float output = pidController.GetOutput();
        if (output >= 0) {
            digitalWrite(DIR_PIN, HIGH);
            analogWrite(MOTOR_PIN, min(255, (int)abs(output)));
        } else {
            digitalWrite(DIR_PIN, LOW);
            analogWrite(MOTOR_PIN, min(255, (int)abs(output)));
        }
    });
}

void loop()
{
    encoder.Update();
    potentiometer.Update();
    speedTimer.Update();
    
    // Allow PID tuning via serial commands
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command.startsWith("kp ")) {
            float kp = command.substring(3).toFloat();
            pidController.SetTunings(kp, 0.2, 0.1);
            Serial.print("Kp set to: ");
            Serial.println(kp);
        }
        else if (command.startsWith("ki ")) {
            float ki = command.substring(3).toFloat();
            pidController.SetTunings(1.5, ki, 0.1);
            Serial.print("Ki set to: ");
            Serial.println(ki);
        }
        else if (command.startsWith("kd ")) {
            float kd = command.substring(3).toFloat();
            pidController.SetTunings(1.5, 0.2, kd);
            Serial.print("Kd set to: ");
            Serial.println(kd);
        }
    }
}
