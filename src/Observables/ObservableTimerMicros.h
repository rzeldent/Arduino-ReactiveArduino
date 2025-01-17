/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOBSERVABLETIMERMICROS_h
#define _REACTIVEOBSERVABLETIMERMICROS_h

template <typename T>
class ObservableTimerMicros : public Observable<unsigned long>
{
public:
	ObservableTimerMicros(unsigned long microsInterval, unsigned long delay = 0);
	void Subscribe(IObserver<T> &observer) override;
	void UnSubscribe(IObserver<T> &observer) override;

	void Start();
	void Reset() override;
	void Stop();
	void Update();

	void SetInterval(unsigned long interval);

	unsigned long GetStartTime() const;
	unsigned long GetElapsedTime() const;
	unsigned long GetRemainingTime() const;

	bool IsActive() const;

private:
	bool _isActive;
	bool _isExpired;
	unsigned long _startTime;
	unsigned long _delay;
	unsigned long _offset;
	unsigned long _interval;

	ObserverList<T> _childObservers;
};

template <typename T>
ObservableTimerMicros<T>::ObservableTimerMicros(unsigned long interval, unsigned long delay)
{
	_isActive = true;
	_delay = delay;
	_offset = delay;
	_interval = interval;
	_startTime = micros();
}

template <typename T>
void ObservableTimerMicros<T>::Subscribe(IObserver<T> &observer)
{
	this->_childObservers.Add(&observer);
}

template <typename T>
void ObservableTimerMicros<T>::UnSubscribe(IObserver<T> &observer)
{
	this->_childObservers.Remove(&observer);
}

template <typename T>
void ObservableTimerMicros<T>::Update()
{
	if (_isActive == false) return;

	auto elapsed = static_cast<unsigned long>(micros() - _startTime);
	if (elapsed >= _interval + _offset)
	{
		this->_childObservers.OnNext(elapsed);
		_offset = 0;
	}
}

template <typename T>
void ObservableTimerMicros<T>::Reset()
{
	_isActive = true;
	_offset = _delay;
	_startTime = micros();
}

template <typename T>
void ObservableTimerMicros<T>::Start()
{
	Reset();
	_isActive = true;
}

template <typename T>
void ObservableTimerMicros<T>::Stop()
{
	_isActive = false;
}

template <typename T>
void ObservableTimerMicros<T>::SetInterval(unsigned long interval)
{
	_interval = interval;
}

template <typename T>
unsigned long ObservableTimerMicros<T>::GetStartTime() const
{
	return _startTime;
}

template <typename T>
unsigned long ObservableTimerMicros<T>::GetElapsedTime() const
{
	return micros() - _startTime;
}

template <typename T>
unsigned long ObservableTimerMicros<T>::GetRemainingTime() const
{
	return _interval - micros() + _startTime;
}

template <typename T>
bool ObservableTimerMicros<T>::IsActive() const
{
	return _isActive;
}

#endif
