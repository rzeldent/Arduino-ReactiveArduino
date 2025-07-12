/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOBSERVABLEULTRASONIC_h
#define _REACTIVEOBSERVABLEULTRASONIC_h

template <typename T>
class ObservableUltrasonic : public Observable<float>
{
public:
	ObservableUltrasonic(int triggerPin, int echoPin, unsigned long intervalMillis = 100);
	
	void Subscribe(IObserver<float> &observer) override;
	void UnSubscribe(IObserver<float> &observer) override;
	void Update();
	void Reset() override;
	
	void SetMaxDistance(float maxDistanceCm);
	void SetTemperature(float temperatureCelsius); // For sound speed correction

private:
	int _triggerPin, _echoPin;
	unsigned long _intervalMillis;
	unsigned long _lastUpdate;
	float _maxDistance;
	float _soundSpeed; // cm/microsecond
	
	ObserverList<float> _childObservers;
	
	float measureDistance();
};

template <typename T>
ObservableUltrasonic<T>::ObservableUltrasonic(int triggerPin, int echoPin, unsigned long intervalMillis)
{
	_triggerPin = triggerPin;
	_echoPin = echoPin;
	_intervalMillis = intervalMillis;
	_lastUpdate = 0;
	_maxDistance = 400.0; // 4 meters default
	_soundSpeed = 0.0343; // cm/microsecond at 20°C
	
	pinMode(_triggerPin, OUTPUT);
	pinMode(_echoPin, INPUT);
}

template <typename T>
void ObservableUltrasonic<T>::Subscribe(IObserver<float> &observer)
{
	this->_childObservers.Add(&observer);
}

template <typename T>
void ObservableUltrasonic<T>::UnSubscribe(IObserver<float> &observer)
{
	this->_childObservers.Remove(&observer);
}

template <typename T>
void ObservableUltrasonic<T>::Update()
{
	unsigned long currentTime = millis();
	if (currentTime - _lastUpdate >= _intervalMillis)
	{
		float distance = measureDistance();
		if (distance > 0 && distance <= _maxDistance)
		{
			this->_childObservers.OnNext(distance);
		}
		_lastUpdate = currentTime;
	}
}

template <typename T>
void ObservableUltrasonic<T>::Reset()
{
	_lastUpdate = 0;
}

template <typename T>
void ObservableUltrasonic<T>::SetMaxDistance(float maxDistanceCm)
{
	_maxDistance = maxDistanceCm;
}

template <typename T>
void ObservableUltrasonic<T>::SetTemperature(float temperatureCelsius)
{
	// Sound speed = 331.3 + (0.606 * temperature) m/s
	// Convert to cm/microsecond
	_soundSpeed = (331.3 + (0.606 * temperatureCelsius)) / 10000.0;
}

template <typename T>
float ObservableUltrasonic<T>::measureDistance()
{
	// Send trigger pulse
	digitalWrite(_triggerPin, LOW);
	delayMicroseconds(2);
	digitalWrite(_triggerPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(_triggerPin, LOW);
	
	// Measure echo duration
	unsigned long duration = pulseIn(_echoPin, HIGH, 30000); // 30ms timeout
	
	if (duration == 0) return -1; // No echo received
	
	// Calculate distance: duration/2 * speed of sound
	return (duration / 2.0) * _soundSpeed;
}

#endif
