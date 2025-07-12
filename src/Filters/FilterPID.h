/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEFILTERPID_h
#define _REACTIVEFILTERPID_h

template <typename T>
class FilterPID : public Operator<T, T>
{
public:
	FilterPID(T setpoint, T kp, T ki, T kd, T outputMin = -255, T outputMax = 255);

	void OnNext(T value) override;
	void Reset() override;
	
	void SetSetpoint(T setpoint);
	void SetTunings(T kp, T ki, T kd);
	void SetOutputLimits(T outputMin, T outputMax);
	void SetSampleTime(unsigned long sampleTimeMillis);
	
	T GetSetpoint() const;
	T GetOutput() const;
	T GetError() const;

private:
	T _setpoint;
	T _kp, _ki, _kd;
	T _outputMin, _outputMax;
	T _lastInput;
	T _outputSum;
	T _lastError;
	T _output;
	T _currentError;
	
	unsigned long _sampleTime;
	unsigned long _lastTime;
	bool _isFirst;
	
	void computePID(T input, unsigned long deltaTime);
};

template <typename T>
FilterPID<T>::FilterPID(T setpoint, T kp, T ki, T kd, T outputMin, T outputMax)
{
	_setpoint = setpoint;
	_kp = kp;
	_ki = ki;
	_kd = kd;
	_outputMin = outputMin;
	_outputMax = outputMax;
	_sampleTime = 100; // Default 100ms sample time
	_outputSum = 0;
	_lastError = 0;
	_output = 0;
	_isFirst = true;
}

template <typename T>
void FilterPID<T>::OnNext(T value)
{
	unsigned long currentTime = millis();
	
	if (_isFirst)
	{
		_lastInput = value;
		_lastTime = currentTime;
		_isFirst = false;
		_currentError = _setpoint - value;
		this->_childObservers.OnNext(0); // Start with zero output
		return;
	}
	
	unsigned long deltaTime = currentTime - _lastTime;
	
	if (deltaTime >= _sampleTime)
	{
		computePID(value, deltaTime);
		_lastTime = currentTime;
		_lastInput = value;
		
		this->_childObservers.OnNext(_output);
	}
}

template <typename T>
void FilterPID<T>::computePID(T input, unsigned long deltaTime)
{
	T deltaTimeSeconds = deltaTime / 1000.0;
	
	// Calculate error
	_currentError = _setpoint - input;
	
	// Proportional term
	T proportional = _kp * _currentError;
	
	// Integral term (with windup protection)
	_outputSum += _ki * _currentError * deltaTimeSeconds;
	if (_outputSum > _outputMax) _outputSum = _outputMax;
	else if (_outputSum < _outputMin) _outputSum = _outputMin;
	
	// Derivative term (on input to avoid derivative kick)
	T derivative = 0;
	if (deltaTimeSeconds > 0)
	{
		T inputDelta = input - _lastInput;
		derivative = -_kd * (inputDelta / deltaTimeSeconds);
	}
	
	// Calculate output
	_output = proportional + _outputSum + derivative;
	
	// Apply output limits
	if (_output > _outputMax) _output = _outputMax;
	else if (_output < _outputMin) _output = _outputMin;
	
	_lastError = _currentError;
}

template <typename T>
void FilterPID<T>::Reset()
{
	_isFirst = true;
	_outputSum = 0;
	_lastError = 0;
	_output = 0;
	_lastTime = millis();
}

template <typename T>
void FilterPID<T>::SetSetpoint(T setpoint)
{
	_setpoint = setpoint;
}

template <typename T>
void FilterPID<T>::SetTunings(T kp, T ki, T kd)
{
	_kp = kp;
	_ki = ki;
	_kd = kd;
}

template <typename T>
void FilterPID<T>::SetOutputLimits(T outputMin, T outputMax)
{
	_outputMin = outputMin;
	_outputMax = outputMax;
	
	// Clamp current output and integral sum to new limits
	if (_output > _outputMax) _output = _outputMax;
	else if (_output < _outputMin) _output = _outputMin;
	
	if (_outputSum > _outputMax) _outputSum = _outputMax;
	else if (_outputSum < _outputMin) _outputSum = _outputMin;
}

template <typename T>
void FilterPID<T>::SetSampleTime(unsigned long sampleTimeMillis)
{
	if (sampleTimeMillis > 0)
	{
		_sampleTime = sampleTimeMillis;
	}
}

template <typename T>
T FilterPID<T>::GetSetpoint() const
{
	return _setpoint;
}

template <typename T>
T FilterPID<T>::GetOutput() const
{
	return _output;
}

template <typename T>
T FilterPID<T>::GetError() const
{
	return _currentError;
}

#endif
