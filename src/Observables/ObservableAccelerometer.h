/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOBSERVABLEACCELEROMETER_h
#define _REACTIVEOBSERVABLEACCELEROMETER_h

struct AccelerometerData
{
	float x;
	float y;
	float z;
	float magnitude;
};

template <typename T>
class ObservableAccelerometer : public Observable<AccelerometerData>
{
public:
	ObservableAccelerometer(int pinX, int pinY, int pinZ, unsigned long intervalMillis = 100);
	
	void Subscribe(IObserver<AccelerometerData> &observer) override;
	void UnSubscribe(IObserver<AccelerometerData> &observer) override;
	void Update();
	void Reset() override;
	
	void SetSensitivity(float sensitivity);
	void SetZeroOffset(float offsetX, float offsetY, float offsetZ);

private:
	int _pinX, _pinY, _pinZ;
	unsigned long _intervalMillis;
	unsigned long _lastUpdate;
	float _sensitivity;
	float _offsetX, _offsetY, _offsetZ;
	AccelerometerData _lastData;
	
	ObserverList<AccelerometerData> _childObservers;
	
	float readAxis(int pin, float offset);
};

template <typename T>
ObservableAccelerometer<T>::ObservableAccelerometer(int pinX, int pinY, int pinZ, unsigned long intervalMillis)
{
	_pinX = pinX;
	_pinY = pinY;
	_pinZ = pinZ;
	_intervalMillis = intervalMillis;
	_lastUpdate = 0;
	_sensitivity = 1.0; // Default sensitivity
	_offsetX = _offsetY = _offsetZ = 0.0;
}

template <typename T>
void ObservableAccelerometer<T>::Subscribe(IObserver<AccelerometerData> &observer)
{
	this->_childObservers.Add(&observer);
}

template <typename T>
void ObservableAccelerometer<T>::UnSubscribe(IObserver<AccelerometerData> &observer)
{
	this->_childObservers.Remove(&observer);
}

template <typename T>
void ObservableAccelerometer<T>::Update()
{
	unsigned long currentTime = millis();
	if (currentTime - _lastUpdate >= _intervalMillis)
	{
		AccelerometerData data;
		data.x = readAxis(_pinX, _offsetX);
		data.y = readAxis(_pinY, _offsetY);
		data.z = readAxis(_pinZ, _offsetZ);
		data.magnitude = sqrt(data.x * data.x + data.y * data.y + data.z * data.z);
		
		this->_childObservers.OnNext(data);
		_lastUpdate = currentTime;
		_lastData = data;
	}
}

template <typename T>
void ObservableAccelerometer<T>::Reset()
{
	_lastUpdate = 0;
}

template <typename T>
void ObservableAccelerometer<T>::SetSensitivity(float sensitivity)
{
	_sensitivity = sensitivity;
}

template <typename T>
void ObservableAccelerometer<T>::SetZeroOffset(float offsetX, float offsetY, float offsetZ)
{
	_offsetX = offsetX;
	_offsetY = offsetY;
	_offsetZ = offsetZ;
}

template <typename T>
float ObservableAccelerometer<T>::readAxis(int pin, float offset)
{
	int rawValue = analogRead(pin);
	float voltage = (rawValue / 1023.0) * 5.0; // Assuming 5V reference
	return (voltage - 2.5 - offset) * _sensitivity; // Convert to g-force
}

#endif
