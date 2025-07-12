/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOBSERVABLEANALOGINPUT_h
#define _REACTIVEOBSERVABLEANALOGINPUT_h

template <typename T = int>
class ObservableAnalogInput : public Observable<T>
{
public:
	ObservableAnalogInput(uint8_t pin, unsigned long intervalMillis = 100);
	void Subscribe(IObserver<T> &observer) override;
	void UnSubscribe(IObserver<T> &observer) override;

	void Next();
	void Update();
	void Reset() override;
	
	void SetInterval(unsigned long intervalMillis);

private:
	uint8_t _pin;
	unsigned long _intervalMillis;
	unsigned long _lastUpdate;

	ObserverList<T> _childObservers;
};

template <typename T>
ObservableAnalogInput<T>::ObservableAnalogInput(uint8_t pin,  unsigned long intervalMillis)
{
	_pin = pin;
	_intervalMillis = intervalMillis;
	_lastUpdate = 0;
	pinMode(pin, INPUT);
}

template <typename T>
void ObservableAnalogInput<T>::Subscribe(IObserver<T> &observer)
{
	this->_childObservers.Add(&observer);
}

template <typename T>
void ObservableAnalogInput<T>::UnSubscribe(IObserver<T> &observer)
{
	this->_childObservers.Remove(&observer);
}

template <typename T>
void ObservableAnalogInput<T>::Next()
{
	this->_childObservers.OnNext(analogRead(_pin));
}

template <typename T>
void ObservableAnalogInput<T>::Update()
{
	unsigned long currentTime = millis();
	if (currentTime - _lastUpdate >= _intervalMillis)
	{
		this->_childObservers.OnNext(analogRead(_pin));
		_lastUpdate = currentTime;
	}
}

template <typename T>
void ObservableAnalogInput<T>::Reset()
{
	_lastUpdate = 0;
}

template <typename T>
void ObservableAnalogInput<T>::SetInterval(unsigned long intervalMillis)
{
	_intervalMillis = intervalMillis;
}

#endif