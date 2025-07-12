# ReactiveArduino Extensions Documentation

This document provides comprehensive documentation for all the new reactive extensions implemented in this fork of ReactiveArduino.

## 🔧 Advanced Operators

### OperatorSelect
Transforms each emitted value using a provided function.

**Signature:**
```c++
template<typename Tdest> auto Select(Tdest (*selector)(T)) -> OperatorSelect<T, Tdest>&
template<typename Tdest> auto SelectTo() -> OperatorSelect<T, Tdest>&
```

**Example:**
```c++
FromArray(values, size)
.Select<float>([](int x) { return x * 2.5; })  // Convert int to float with scaling
.Do([](float x) { Serial.println(x); });
```

### OperatorDistinct
Emits only values that haven't been seen before in the stream.

**Signature:**
```c++
auto Distinct() -> OperatorDistinct<T>&
```

**Example:**
```c++
int values[] = {1, 2, 2, 3, 1, 4, 3, 5};
FromArray(values, 8)
.Distinct()  // Outputs: 1, 2, 3, 4, 5
.Do([](int x) { Serial.println(x); });
```

### OperatorDistinctUntilChanged
Emits only when the value changes from the previous emission.

**Signature:**
```c++
auto DistinctUntilChanged() -> OperatorDistinctUntilChanged<T>&
```

**Example:**
```c++
int values[] = {1, 1, 2, 2, 2, 3, 3, 1};
FromArray(values, 8)
.DistinctUntilChanged()  // Outputs: 1, 2, 3, 1
.Do([](int x) { Serial.println(x); });
```

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

**Signature:**
```c++
template<typename TAcc> auto Scan(TAcc (*accumulator)(TAcc, T), TAcc seed) -> OperatorScan<T, TAcc>&
```

**Example:**
```c++
// Function pointer for Arduino compatibility
float addValues(float acc, int current) {
    return acc + current;
}

FromArray(values, size)
.Scan<float>(addValues, 0.0)  // Running sum: 1, 3, 6, 10, 15
.Do([](float x) { Serial.print("Running sum: "); Serial.println(x); });
```

**Use cases:**
- Running totals and counters
- State machines
- Progressive calculations

### OperatorReduce
Accumulates values and emits only the final result.

**Signature:**
```c++
template<typename Tdest> auto ReduceToFinal(Tdest (*accumulator)(Tdest, T), Tdest seed) -> OperatorReduce<T, Tdest>&
```

**Example:**
```c++
float multiplyValues(float acc, int current) {
    return acc * current;
}

FromArray(values, size)
.ReduceToFinal<float>(multiplyValues, 1.0)  // Only final product
.Do([](float x) { Serial.print("Final product: "); Serial.println(x); });
```

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

## 🌡️ Professional Filters

### FilterKalman
Advanced noise reduction filter using Kalman filtering algorithm.

**Signature:**
```c++
auto Kalman(T processNoise, T measurementNoise, T estimatedError) -> FilterKalman<T>&
```

**Example:**
```c++
ObservableAnalogInput(A0, 100)
.Cast<float>()
.Kalman<float>(0.01, 0.1, 1.0)  // Process, measurement, error noise
.Do([](float filtered) { Serial.println(filtered); });
```

### FilterHysteresis
Prevents oscillation around threshold values.

**Signature:**
```c++
auto Hysteresis(T lowerThreshold, T upperThreshold) -> FilterHysteresis<T>&
```

**Example:**
```c++
ObservableAnalogInput(A0, 50)
.Cast<float>()
.Hysteresis<float>(2.0, 3.0)  // Switch high at 3.0, low at 2.0
.Do([](float state) { digitalWrite(13, state > 2.5); });
```

### FilterPID
Proportional-Integral-Derivative controller for feedback systems.

**Signature:**
```c++
auto PID(T kp, T ki, T kd, T setpoint) -> FilterPID<T>&
```

**Example:**
```c++
float targetTemp = 25.0;
ObservableAnalogInput(A0, 100)  // Temperature sensor
.Cast<float>()
.Select<float>([](float raw) { return (raw * 5.0 / 1023.0 - 0.5) * 100; })
.PID<float>(2.0, 1.0, 0.1, targetTemp)  // Kp, Ki, Kd, setpoint
.Do([](float output) { 
    analogWrite(9, constrain(output, 0, 255)); 
});
```

## 📡 Sensor Observables

### ObservableAccelerometer
3-axis accelerometer data stream with configurable sampling rate.

**Data Structure:**
```c++
struct AccelerometerData {
    float x, y, z;
    unsigned long timestamp;
    
    bool operator==(const AccelerometerData& other) const;
    bool operator!=(const AccelerometerData& other) const;
};
```

**Signature:**
```c++
ObservableAccelerometer(int sclPin, int sdaPin, unsigned long intervalMs)
```

**Example:**
```c++
ObservableAccelerometer(21, 22, 100)  // SCL=21, SDA=22, 100ms interval
.Select<float>([](AccelerometerData data) {
    return sqrt(data.x*data.x + data.y*data.y + data.z*data.z);
})
.Do([](float magnitude) {
    Serial.print("Acceleration magnitude: ");
    Serial.println(magnitude);
});
```

### ObservableUltrasonic
Distance measurement stream using HC-SR04 or similar sensors.

**Signature:**
```c++
ObservableUltrasonic(int triggerPin, int echoPin, unsigned long intervalMs)
```

**Example:**
```c++
ObservableUltrasonic(7, 8, 200)  // Trigger=7, Echo=8, 200ms interval
.Do([](float distance) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
});
```

### ObservableRotaryEncoder
Rotary encoder position and direction tracking.

**Data Structure:**
```c++
struct RotaryEncoderData {
    long position;
    int direction;  // -1, 0, or 1
    
    bool operator==(const RotaryEncoderData& other) const;
    bool operator!=(const RotaryEncoderData& other) const;
};
```

**Signature:**
```c++
ObservableRotaryEncoder(int pinA, int pinB, unsigned long intervalMs)
```

**Example:**
```c++
ObservableRotaryEncoder(2, 3, 50)  // Pin A=2, Pin B=3, 50ms interval
.Debounce<RotaryEncoderData>(100)  // Debounce encoder noise
.Do([](RotaryEncoderData data) {
    Serial.print("Position: ");
    Serial.print(data.position);
    Serial.print(", Direction: ");
    Serial.println(data.direction);
});
```

## 🔄 Enhanced Transformations

### TransformationInterpolate
Linear interpolation between input and output ranges.

**Signature:**
```c++
auto Interpolate(T inputMin, T inputMax, T outputMin, T outputMax) -> TransformationInterpolate<T>&
```

**Example:**
```c++
ObservableAnalogInput(A0, 100)  // 0-1023 ADC range
.Cast<float>()
.Interpolate<float>(0.0, 1023.0, -10.0, 50.0)  // Map to temperature range
.Do([](float temperature) {
    Serial.print("Temperature: ");
    Serial.println(temperature);
});
```

## 🎯 Usage Patterns

### Chaining Multiple Operators
```c++
ObservableAnalogInput(A0, 50)
.Cast<float>()                           // Convert to float
.Interpolate<float>(0, 1023, 0, 100)     // Scale to 0-100
.Kalman<float>(0.01, 0.1, 1.0)          // Noise reduction
.DistinctUntilChanged()                  // Only on change
.Throttle<float>(1000)                   // Max once per second
.Do([](float value) {
    Serial.print("Processed value: ");
    Serial.println(value);
});
```

### Smart Thermostat Example
```cpp
ObservableAnalogInput(A0, 1000)
.Cast<float>()
.Select<float>([](float raw) { return (raw * 5.0 / 1023.0 - 0.5) * 100; })  // Convert to Celsius
.Kalman<float>(0.1, 1.0, 1.0)  // Noise filtering
.Hysteresis<float>(20.0, 22.0)  // Prevent oscillation
.Do([](float temp) {
    digitalWrite(HEATER_PIN, temp < 21.0);
});
```

### Motion-Activated Light
```cpp
ObservableAccelerometer(21, 22, 100)
.Select<float>([](AccelerometerData data) { 
    return sqrt(data.x*data.x + data.y*data.y + data.z*data.z); 
})
.Where([](float mag) { return mag > 1.5; })  // Motion threshold
.Throttle<float>(5000)  // Stay on for 5 seconds minimum
.Do([](float mag) {
    digitalWrite(LED_PIN, HIGH);
});
```

### PID Temperature Controller
```cpp
float targetTemp = 25.0;
ObservableAnalogInput(A0, 200)
.Cast<float>()
.Select<float>([](float raw) { return (raw * 5.0 / 1023.0 - 0.5) * 100; })
.Kalman<float>(0.1, 1.0, 1.0)  // Smooth readings
.PID<float>(2.0, 1.0, 0.1, targetTemp)  // PID control
.Do([](float output) {
    analogWrite(HEATER_PIN, constrain(output, 0, 255));
});
```

### Scan vs Reduce Comparison
```cpp
// Scan emits intermediate results: 1, 3, 6, 10, 15
FromArray(data, 5)
.Scan<int>(addValues, 0)  // Running sum with intermediate emissions
.Do([](int x) { Serial.print("Running sum: "); Serial.println(x); });

// Reduce emits only final result: 15
FromArray(data, 5)
.ReduceToFinal<int>(addValues, 0)  // Only final accumulated result
.Do([](int x) { Serial.print("Final sum: "); Serial.println(x); });
```

## 🔧 Arduino-Specific Considerations

### Function Pointer Compatibility
All operators support Arduino-compatible function pointers:

```c++
// Define functions outside of classes/lambdas
float scaleValue(float x) { return x * 2.0; }
float addValues(float a, float b) { return a + b; }

// Use with operators
FromArray(data, size)
.Select<float>(scaleValue)
.Scan<float>(addValues, 0.0)
.Do([](float x) { Serial.println(x); });
```

### Custom Struct Requirements
Implement comparison operators for custom structs:

```c++
struct CustomData {
    int value;
    float measurement;
    
    bool operator==(const CustomData& other) const {
        return value == other.value && measurement == other.measurement;
    }
    
    bool operator!=(const CustomData& other) const {
        return !(*this == other);
    }
};
```

### Pin Configuration
New sensor observables automatically configure pins as INPUT:

```c++
// Pins are automatically set to INPUT mode
ObservableAccelerometer(21, 22, 100);  // I2C pins configured
ObservableRotaryEncoder(2, 3, 50);     // Digital pins configured
```

## 📊 Performance Considerations

- **Memory Usage**: Each operator allocates approximately 20-50 bytes
- **Processing Time**: Most operators add < 1μs processing overhead
- **Sensor Observables**: Use appropriate intervals to balance responsiveness and CPU usage
- **Function Pointers**: Slightly faster than lambda expressions on Arduino
- **Chaining**: Prefer operator chaining over creating intermediate variables

## 🔍 Best Practices

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

## 🔍 Debugging Tips

1. **Use Serial Output**: Chain `.Do()` operators to monitor data flow
2. **Memory Monitoring**: Check available RAM with `Serial.println(ESP.getFreeHeap())`
3. **Timing Analysis**: Use `.Timestamp()` to measure processing delays
4. **Validation**: Test with known input sequences using `FromArray()`

```c++
// Debug chain example
FromArray(testData, size)
.Do([](int x) { Serial.print("Input: "); Serial.println(x); })
.Select<float>(transform)
.Do([](float x) { Serial.print("Transformed: "); Serial.println(x); })
.Kalman<float>(0.01, 0.1, 1.0)
.Do([](float x) { Serial.print("Filtered: "); Serial.println(x); });
```
