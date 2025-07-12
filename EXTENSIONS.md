# ReactiveArduino Extensions

This document describes the new reactive methods and operators added to the ReactiveArduino library.

## New Operators

### OperatorThrottle
Limits the rate at which values are emitted from an observable.

```cpp
timer.Throttle(500)  // Only emit once every 500ms
```

**Use cases:**
- Rate limiting sensor readings
- Preventing spam in serial output
- Reducing computational load

### OperatorScan
Applies an accumulator function over the observable sequence and emits each intermediate result.

```cpp
sensor.Scan<int>(0, [](int acc, float value) { 
    return acc + (value > threshold ? 1 : 0); 
})  // Count values above threshold
```

**Use cases:**
- Running totals and counters
- State machines
- Progressive calculations

### OperatorStartWith
Emits specified values before beginning to emit values from the source observable.

```cpp
sensor.StartWith(0)  // Start with initial value of 0
```

**Use cases:**
- Providing default values
- Initializing UI components
- Setting baseline measurements

### OperatorDebounce
Suppresses values from an observable until a specified time period has passed without another value.

```cpp
button.Debounce(100)  // Wait 100ms after last change
```

**Use cases:**
- Button debouncing
- Noise filtering
- Preventing rapid-fire events

### OperatorDistinct
Filters out all duplicate values from the observable sequence (not just consecutive ones).

```cpp
sensor.Distinct()  // Only emit unique values
```

**Use cases:**
- Removing duplicate sensor readings
- Event deduplication
- Unique value collection
- Memory-conscious duplicate filtering (limited to 32 unique values by default)

## New Filters

### FilterHysteresis
Implements hysteresis filtering to prevent oscillation around threshold values.

```cpp
sensor.Hysteresis(10.0, 20.0)  // Low threshold 10, high threshold 20
```

**Use cases:**
- Thermostat control
- Motion detection
- Level sensing with noise immunity

### FilterKalman
Applies Kalman filtering for optimal estimation in the presence of noise.

```cpp
sensor.Kalman(0.1, 4.0)  // Process variance 0.1, measurement variance 4.0
```

**Use cases:**
- Sensor fusion
- Position tracking
- Noise reduction in measurements

### FilterPID
Implements a complete PID (Proportional-Integral-Derivative) controller for closed-loop control systems.

```cpp
auto pidController = FilterPID<float>(25.0, 2.0, 0.1, 0.5, 0, 255);
sensor.PID(25.0, 2.0, 0.1, 0.5, 0, 255)  // Setpoint, Kp, Ki, Kd, output min, max
```

**Features:**
- Configurable P, I, D gains
- Integral windup protection
- Derivative kick prevention
- Adjustable output limits
- Real-time tuning methods

**Use cases:**
- Temperature control
- Motor speed control
- Position control
- Level control
- Process automation

## New Observables

### ObservableAccelerometer
Monitors 3-axis accelerometer data from analog pins.

```cpp
auto accel = ObservableAccelerometer<AccelerometerData>(A0, A1, A2, 100);
accel.SetSensitivity(3.3);
accel.SetZeroOffset(0.0, 0.0, 0.0);
```

**Features:**
- Configurable sensitivity and zero offset
- Magnitude calculation
- Adjustable sampling rate

### ObservableUltrasonic
Measures distance using HC-SR04 ultrasonic sensors.

```cpp
auto ultrasonic = ObservableUltrasonic<float>(7, 8, 250);
ultrasonic.SetMaxDistance(200.0);
ultrasonic.SetTemperature(25.0);  // Temperature compensation
```

**Features:**
- Temperature compensation for accuracy
- Configurable maximum range
- Built-in timeout handling

### ObservableRotaryEncoder
Monitors rotary encoder rotation and button presses.

```cpp
auto encoder = ObservableRotaryEncoder<RotaryEncoderData>(2, 3, 4);
```

**Features:**
- Direction detection
- Position tracking
- Button state monitoring
- Built-in debouncing

## New Transformations

### TransformationInterpolate
Maps input values from one range to another with optional constraining.

```cpp
sensor.Interpolate(0, 1023, 0.0, 5.0)  // Map ADC to voltage
```

**Features:**
- Linear interpolation
- Optional value constraining
- Supports any numeric type

## Usage Examples

### Smart Thermostat
```cpp
auto thermistor = AnalogInput(A0, 1000);

thermistor
.AdcToVoltage()
.Select([](float voltage) { return voltageToTemperature(voltage); })
.Kalman(0.1, 1.0)  // Noise filtering
.Hysteresis(20.0, 22.0)  // Prevent oscillation
.Do([](float temp) {
    digitalWrite(HEATER_PIN, temp < 21.0);
});
```

### Motion-Activated Light
```cpp
auto motion = ObservableAccelerometer<AccelerometerData>(A0, A1, A2);

motion
.Select([](AccelerometerData data) { return data.magnitude; })
.Where([](float mag) { return mag > 1.5; })  // Motion threshold
.Throttle(5000)  // Stay on for 5 seconds minimum
.Do([](float mag) {
    digitalWrite(LED_PIN, HIGH);
    // Set timer to turn off later
});
```

### Distance Warning System
```cpp
auto distance = ObservableUltrasonic<float>(7, 8);

distance
.Kalman(0.1, 2.0)  // Smooth readings
.Where([](float d) { return d < 20.0; })  // Danger zone
.Scan<int>(0, [](int count, float d) { 
    return d < 10.0 ? count + 1 : 0;  // Count close readings
})
.Where([](int count) { return count > 3; })  // Sustained proximity
.Do([](int count) {
    digitalWrite(BUZZER_PIN, HIGH);
});
```

### PID Temperature Controller
```cpp
auto tempSensor = AnalogInput(A0, 200);
auto pidController = FilterPID<float>(25.0, 2.0, 0.1, 0.5, 0, 255);

tempSensor
.Select([](int adc) { return adcToTemperature(adc); })
.Kalman(0.1, 1.0)  // Smooth readings
.PID(25.0, 2.0, 0.1, 0.5, 0, 255)  // PID control
.Do([](float output) {
    analogWrite(HEATER_PIN, (int)output);
});

// Dynamic setpoint adjustment
pidController.SetSetpoint(newTarget);
pidController.SetTunings(kp, ki, kd);
```

### PID Motor Speed Control
```cpp
auto encoder = ObservableRotaryEncoder<RotaryEncoderData>(2, 3);
auto speedTimer = IntervalMillis(250);

speedTimer
.Select([](unsigned long t) { return calculateRPM(); })
.PID(targetRPM, 1.5, 0.2, 0.1, -255, 255)
.Do([](float output) {
    setMotorSpeed(output);  // Apply to motor driver
});
```

### Sensor Event Detection
```cpp
auto motionSensor = AnalogInput(A0, 100);

motionSensor
.Where([](int value) { return value > 512; })  // Motion threshold
.Distinct()  // Only report unique motion events
.Do([](int value) {
    Serial.println("Motion detected!");
    triggerAlarm();
});
```

## Performance Considerations

- **Memory Usage**: New operators allocate minimal additional memory
- **CPU Impact**: Kalman filter requires floating-point arithmetic
- **Update Frequency**: Call `Update()` methods in your main loop
- **Debouncing**: Built-in debouncing reduces CPU load from rapid changes

## Best Practices

1. **Chain Wisely**: Order operators for efficiency (filters before transformations)
2. **Throttle Output**: Use throttling for serial output and actuator control
3. **Calibrate Sensors**: Always calibrate accelerometers and other analog sensors
4. **Temperature Compensation**: Use temperature correction for precise measurements
5. **Error Handling**: Check sensor validity before processing data
6. **PID Tuning**: 
   - Start with P-only control, then add I and D
   - Use Ziegler-Nichols or other systematic tuning methods
   - Monitor for integral windup and oscillation
   - Adjust sample time based on system response speed
   - Set appropriate output limits to prevent actuator saturation
