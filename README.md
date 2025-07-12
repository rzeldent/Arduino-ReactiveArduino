# Library ReactiveArduino

ReactiveArduino implements observable-observer pattern on a processor like Arduino. The purpose is to provide declarative programming approach, within the capacity constraints of a low-power MCU.

ReactiveArduino is heavily based on [ReactiveX](http://reactivex.io/) and [ReactiveUI](https://reactiveui.net/), adapted to the needs and limitations in a MCU.

## 🚀 New Extensions Available!

This fork includes powerful new reactive extensions with comprehensive operator support:

### 🔧 Advanced Operators
- **`Select()`** - Transform values with custom mapping functions
- **`Distinct()`** - Filter out duplicate values from stream
- **`DistinctUntilChanged()`** - Emit only when value changes from previous
- **`Scan()`** - Accumulate values with intermediate emissions
- **`Reduce()`** - Accumulate to final result only
- **`StartWith()`** - Begin stream with initial values
- **`Throttle()`** - Rate limiting with configurable intervals
- **`Debounce()`** - Eliminate rapid value changes

### 🌡️ Professional Filters
- **`Kalman()`** - Advanced noise reduction filter
- **`Hysteresis()`** - Prevent oscillation around thresholds
- **`PID()`** - Proportional-Integral-Derivative controller

### 📡 Sensor Observables
- **`ObservableAccelerometer`** - 3-axis acceleration data streams
- **`ObservableUltrasonic`** - Distance measurement observables
- **`ObservableRotaryEncoder`** - Position and direction tracking

### 🔄 Enhanced Transformations
- **`Interpolate()`** - Linear mapping between value ranges

All operators include Arduino-compatible function pointer support and comprehensive examples.

See [EXTENSIONS.md](EXTENSIONS.md) for complete documentation and advanced usage examples.

## Development Setup

### Arduino IDE
1. Download and install as a library
2. Include `#include "ReactiveArduinoLib.h"`

### PlatformIO (Recommended)
```bash
# Clone and build
git clone https://github.com/rzeldent/Arduino-ReactiveArduino.git
cd Arduino-ReactiveArduino
pio run -e esp32

# Test new operator examples
pio run -e scan              # Scan operator with intermediate emissions
pio run -e reduce            # Reduce operator with final result only
pio run -e distinct          # Distinct value filtering
pio run -e throttle          # Rate limiting demonstration
pio run -e accelerometer     # 3-axis sensor data processing
pio run -e ultrasonic        # Distance measurement streams
pio run -e rotary_encoder    # Position tracking with debouncing
```

See [PLATFORMIO.md](PLATFORMIO.md) for detailed development guide.

## Instructions for use
The general use of ReactiveArduino consists of:
* Define an observable (Timer, Interval, FromArray, FromProperty...)
* Chain with one or more operators (Distinct, Where, Select, First, Last, Sum...) 
* Subscribe an observer (Do, DoFinally, ToProperty, ToArray...)

For example:
```c++
	FromRange(10, 20)
	.Select([](int x) { return x + 10; })
	.Do( [](int x) { Serial.println(x); });
```

More examples in Wiki/[Examples](https://github.com/luisllamasbinaburo/Arduino-ReactiveArduino/wiki/Examples)


### Observable, observers and operators legend
More info about the Observables, Observers, and Operators available in the [Wiki](https://github.com/luisllamasbinaburo/Arduino-ReactiveArduino/wiki)

```
┌─────────────────────────────────────────────────────────────────────────────────────┐
│                        🔄 REACTIVE ARDUINO COMPONENT LEGEND 🔄                      │
├─────────────────────────────────────────────────────────────────────────────────────┤
│                                                                                     │
│  📡 OBSERVABLES (Data Sources)                                                     │
│  ┌─────────────────────────────────────────────────────────────────────────────┐   │
│  │ • FromArray()        • FromProperty()     • FromSerial()                    │   │
│  │ • FromRange()        • AnalogInput()      • DigitalInput()                  │   │
│  │ • TimerMillis()      • TimerMicros()      • IntervalMillis()                │   │
│  │ • Property<T>()      • ManualDefer()      • IntervalMicros()                │   │
│  │ • Accelerometer()    • Ultrasonic()       • RotaryEncoder()                 │   │
│  └─────────────────────────────────────────────────────────────────────────────┘   │
│                                 ⬇️                                                  │
│  🔧 OPERATORS (Data Processing)                                                    │
│  ┌─────────────────────────────────────────────────────────────────────────────┐   │
│  │ • Where()            • Distinct()         • DistinctUntilChanged()          │   │
│  │ • First()            • Last()             • Skip() / Take()                 │   │
│  │ • SkipWhile()        • TakeWhile()        • SkipUntil() / TakeUntil()       │   │
│  │ • Batch()            • Repeat()           • Loop()                          │   │
│  │ • StartWith()        • Throttle()         • Debounce()                      │   │
│  └─────────────────────────────────────────────────────────────────────────────┘   │
│                                 ⬇️                                                  │
│  🔄 TRANSFORMATIONS (Data Conversion)                                              │
│  ┌─────────────────────────────────────────────────────────────────────────────┐   │
│  │ • Select()           • Map<T>()           • Cast<T>()                       │   │
│  │ • SelectTo<T>()      • Scan()             • Reduce()                        │   │
│  │ • Scale()            • Interpolate()      • Limit() / LimitUpper()          │   │
│  │ • Millis()           • Micros()           • Timestamp()                     │   │
│  └─────────────────────────────────────────────────────────────────────────────┘   │
│                                 ⬇️                                                  │
│  🌡️ FILTERS (Conditional Processing)                                               │
│  ┌─────────────────────────────────────────────────────────────────────────────┐   │
│  │ • IsEqual()          • IsNotEqual()       • IsGreater() / IsLess()          │   │
│  │ • IsZero()           • IsNotZero()        • OnRising() / OnFalling()        │   │
│  │ • DebounceMillis()   • LowPass()          • HighPass()                      │   │
│  │ • MovingAverage()    • Median3/5()        • WindowMillis()                  │   │
│  │ • Kalman()           • Hysteresis()       • PID()                           │   │
│  └─────────────────────────────────────────────────────────────────────────────┘   │
│                                 ⬇️                                                  │
│  📊 AGGREGATES (Statistical Operations)                                            │
│  ┌─────────────────────────────────────────────────────────────────────────────┐   │
│  │ • Count()            • CountDown()        • Sum()                           │   │
│  │ • Min() / Max()      • Average()          • RMS()                           │   │
│  │ • Any()              • All()              • None()                          │   │
│  └─────────────────────────────────────────────────────────────────────────────┘   │
│                                 ⬇️                                                  │
│  🎯 OBSERVERS (Data Consumers)                                                     │
│  ┌─────────────────────────────────────────────────────────────────────────────┐   │
│  │ • Do()               • Finally()          • DoAndFinally()                  │   │
│  │ • ToSerial()         • ToProperty()       • ToArray()                       │   │
│  │ • ToDigitalOutput()  • ToAnalogOutput()   • ToCircularBuffer()              │   │
│  │ • DoNothing()        • (Custom Actions)                                     │   │
│  └─────────────────────────────────────────────────────────────────────────────┘   │
│                                                                                     │
│  💡 USAGE PATTERN: Observable → [Operators] → [Transformations] → [Filters] →     │
│                    [Aggregates] → Observer                                         │
│                                                                                     │
│  Example: FromArray(data, size).Where(condition).Select(transform).ToSerial()      │
│                                                                                     │
└─────────────────────────────────────────────────────────────────────────────────────┘
```

### Creating Observables
Observables are generally generated through factory methods provided by the Reactive class.
For example:
```c++
FromArray<int>(values, valuesLength)
```

### Chaining operators and observers
Chaining operators are usually make using 'Fluent Notation' which allows to combine observable and observers.
For example:
```c++
observableInt.ToSerial();
```

To chain existing operators ReactiveArduino uses overload of operator `>>`.
For example:
```c++
observableInt >> ToSerial<bool>();
```

### Cool and hot observables
ReactiveArduino has two types of observables, hot and cold.
Hot observables emits the sequence when an observer subscribes to it. For example, `FromArray(...)` is a Hot Observable.
```c++
FromArray(values, valuesLength)
```
Cold observable does not emits any item when a observer subscribes to it. You have to explicitly call the `Next()` method whenever you want. For example, `FromArrayDefer(...)`
```c++
FromArrayDefer(values, valuesLength)
```
### Dynamic memory considerations
On many occasions we generate operators directly when we chain them, for example in the `Setup()`. However, creating an operator allocates dynamic memory. Therefore, you should avoid creating them in `Loop()`, or you could run out of memory.
If you need to reuse (typically, call some operator method later in your code) set it as a global variable, and chain as normal.
```c++
auto counter = Count<int>();
...
//(later in code)
...
obsString >> counter >> ToSerial<String>();
```

### Arduino Compatibility Notes

#### Function Pointers vs Lambdas
The new operators support both Arduino-compatible function pointers and modern lambda expressions:

```c++
// Arduino-compatible function pointer approach
float addValues(float a, float b) { return a + b; }

FromArray(data, size)
.Scan<float>(addValues, 0.0)  // Use function pointer
.Do([](float x) { Serial.println(x); });

// Lambda expressions (where supported)
FromArray(data, size)
.Select<float>([](int x) { return x * 2.0; })  // Simple transformations
.Do([](float x) { Serial.println(x); });
```

#### Custom Struct Comparison
When using custom structs with operators like `Distinct()` or `Debounce()`, implement comparison operators:

```c++
struct SensorData {
    float temperature;
    float humidity;
    
    // Required for Distinct and DistinctUntilChanged
    bool operator==(const SensorData& other) const {
        return temperature == other.temperature && humidity == other.humidity;
    }
    
    bool operator!=(const SensorData& other) const {
        return !(*this == other);
    }
};
```

## Examples

### Basic Usage Pattern
This example shows how to use Reactive Operators to perform a 3-elements Median filter, then a 4-elements Moving Average Filter, and make some action with the final filtered value.
```c++
#include "ReactiveArduinoLib.h"
using namespace Reactive;

int values[] = { 0, 1, 4, 6, 2, 5, 7, 3, 5, 8 };
int valuesLength = sizeof(values) / sizeof(values[0]);

void setup()
{
	Serial.begin(9600);
	while (!Serial) delay(1);

	FromArray(values, valuesLength)
	.Cast<float>()
	.Median5()
	.MovingAverage(4)
	.DoAndFinally(
		[](float x) { Serial.println(x); },
		[]() { Serial.println("No more items"); }
	);
}

void loop()
{
	delay(2000);
}
```

### New Operator Examples

#### Scan vs Reduce Operations
```c++
// Scan emits intermediate results: 1, 3, 6, 10, 15
FromArray(data, 5)
.Scan<int>(addValues, 0)  // Running sum with intermediate emissions
.Do([](int x) { Serial.print("Running sum: "); Serial.println(x); });

// Reduce emits only final result: 15
FromArray(data, 5)
.ReduceToFinal<int>(addValues, 0)  // Only final accumulated result
.Do([](int x) { Serial.print("Final sum: "); Serial.println(x); });
```

#### Distinct Value Filtering
```c++
int values[] = {1, 1, 2, 2, 2, 3, 1, 3, 3};
FromArray(values, 9)
.Distinct()  // Outputs: 1, 2, 3, 1, 3 (removes consecutive duplicates)
.Do([](int x) { Serial.println(x); });
```

#### Sensor Data Processing with Debouncing
```c++
// Accelerometer with noise filtering
ObservableAccelerometer(scl, sda, 100)  // 100ms interval
.Debounce<AccelerometerData>(50)         // 50ms debounce
.Select<float>([](AccelerometerData data) { 
    return sqrt(data.x*data.x + data.y*data.y + data.z*data.z); 
})
.Do([](float magnitude) { 
    Serial.print("Magnitude: "); 
    Serial.println(magnitude); 
});
```

#### Throttling and Rate Limiting
```c++
// Limit sensor readings to once per second
ObservableAnalogInput(A0, 10)  // Read every 10ms
.Throttle<int>(1000)           // But emit maximum once per second
.Do([](int value) { Serial.println(value); });
```

#### Advanced Filtering with PID Controller
```c++
float targetValue = 25.0;  // Target temperature
ObservableAnalogInput(A0, 100)  // Temperature sensor
.Cast<float>()
.Select<float>([](float raw) { return (raw * 5.0 / 1023.0 - 0.5) * 100; })  // Convert to Celsius
.PID<float>(2.0, 1.0, 0.1, targetValue)  // PID(Kp, Ki, Kd, setpoint)
.Do([](float output) { 
    analogWrite(9, constrain(output, 0, 255));  // PWM output to heater
});
```

More examples in Wiki/[Examples](https://github.com/luisllamasbinaburo/Arduino-ReactiveArduino/wiki/Examples)
