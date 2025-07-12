# PlatformIO Development Guide

This document explains how to develop and test the ReactiveArduino library using PlatformIO.

## Quick Start

### Prerequisites
- [PlatformIO Core](https://platformio.org/install/cli) or [PlatformIO IDE](https://platformio.org/platformio-ide)
- Git

### Setup
1. Clone the repository:
   ```bash
   git clone https://github.com/rzeldent/Arduino-ReactiveArduino.git
   cd Arduino-ReactiveArduino
   ```

2. Build for your target platform:
   ```bash
   # For Arduino Uno
   pio run -e uno
   
   # For ESP32
   pio run -e esp32
   
   # For ESP8266
   pio run -e esp8266
   ```

3. Upload to your device:
   ```bash
   pio run -e uno --target upload
   ```

## Available Environments

### Target Platforms
- `uno` - Arduino Uno
- `nano` - Arduino Nano
- `mega` - Arduino Mega 2560
- `esp32` - ESP32 development board
- `esp8266` - ESP8266 (NodeMCU v2)
- `teensy` - Teensy 4.0
- `due` - Arduino Due

### Test Environments
- `test_blink` - Basic blink example
- `test_extensions` - New reactive extensions
- `test_pid` - PID controller example
- `test_distinct` - Distinct operator example
- `test_esp32_full` - Full feature test on ESP32
- `lib_test` - Unit testing framework

## Testing Examples

### Build Specific Examples
```bash
# Test throttle example
pio run -e test_extensions

# Test PID controller
pio run -e test_pid

# Test distinct operator
pio run -e test_distinct
```

### Build All Examples
```bash
# Build all examples for ESP32
pio ci --lib="." --board esp32dev examples/*/

# Build specific example for multiple boards
pio ci --lib="." --board uno --board esp32dev examples/Blink/Blink.cpp
```

## Development Workflow

### 1. Code Changes
Make your changes to the library files in `src/` directory.

### 2. Test Compilation
```bash
# Quick syntax check
pio run -e lib_test --target compiledb

# Test on multiple platforms
pio run -e uno -e esp32 -e esp8266
```

### 3. Test Examples
```bash
# Test all new extensions
pio run -e test_extensions
pio run -e test_pid
pio run -e test_distinct
```

### 4. Monitor Serial Output
```bash
# Upload and monitor
pio run -e esp32 --target upload --target monitor

# Monitor existing upload
pio device monitor --port /dev/ttyUSB0 --baud 115200
```

## New ReactiveArduino Extensions

The library now includes these new reactive methods:

### New Operators
- `Distinct()` - Remove all duplicate values
- `Throttle(ms)` - Rate limiting
- `Scan(seed, accumulator)` - Progressive accumulation
- `StartWith(value)` - Emit initial value
- `Debounce(ms)` - Advanced debouncing

### New Filters
- `Hysteresis(low, high)` - Prevent oscillation
- `Kalman(process, measurement)` - Optimal noise filtering
- `PID(setpoint, kp, ki, kd)` - Complete PID controller

### New Observables
- `AccelerometerInput()` - 3-axis motion sensing
- `UltrasonicSensor()` - Distance measurement
- `RotaryEncoder()` - Position tracking

### New Transformations
- `Interpolate(inMin, inMax, outMin, outMax)` - Linear mapping

## Memory Considerations

### Arduino Uno/Nano (2KB RAM)
- Use basic operators and filters
- Avoid complex chaining
- Limit to 2-3 operators per chain

### ESP32/ESP8266 (80KB+ RAM)
- Full feature support
- Complex operator chaining supported
- Advanced filtering and PID control

### Optimization Tips
- Use `REACTIVE_EXTENDED_MEMORY` flag for more features
- Prefer stack allocation over dynamic allocation
- Monitor memory usage with `pio run --target size`

## Continuous Integration

The project uses GitHub Actions for automated testing:

- **Build Examples**: Tests all examples on multiple platforms
- **Library Test**: Validates library compilation
- **Syntax Check**: Ensures code quality

### Local CI Testing
```bash
# Simulate CI locally
pio ci --lib="." --board uno --board esp32dev examples/Blink/Blink.cpp
pio ci --lib="." --board esp32dev src/main.cpp
```

## Troubleshooting

### Common Issues

1. **Memory Errors on Arduino Uno**
   - Solution: Use simpler examples or switch to ESP32
   - Alternative: Enable memory optimization flags

2. **Compilation Errors**
   - Check include paths in `platformio.ini`
   - Verify all header files are properly included
   - Run `pio run --target clean` and rebuild

3. **Upload Failures**
   - Check device connection and port
   - Verify correct board selection
   - Try different upload speeds

### Debug Tips
```bash
# Verbose compilation
pio run -e uno --verbose

# Check library dependencies
pio lib deps

# Clean build
pio run --target clean
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test on multiple platforms using PlatformIO
5. Submit a pull request

The CI will automatically test your changes on all supported platforms.
