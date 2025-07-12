/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOPERATORDEBOUNCE_h
#define _REACTIVEOPERATORDEBOUNCE_h

template <typename T>
class OperatorDebounce : public Operator<T, T>
{
public:
	OperatorDebounce(unsigned long debounceMillis);

	void OnNext(T value) override;
	void Reset() override;
	void Update();

private:
	unsigned long _debounceMillis;
	unsigned long _lastChangeTime;
	T _pendingValue;
	T _lastEmittedValue;
	bool _hasPendingValue;
	bool _hasEmittedValue;
};

template <typename T>
OperatorDebounce<T>::OperatorDebounce(unsigned long debounceMillis)
{
	_debounceMillis = debounceMillis;
	_lastChangeTime = 0;
	_hasPendingValue = false;
	_hasEmittedValue = false;
}

template <typename T>
void OperatorDebounce<T>::OnNext(T value)
{
	if (!_hasEmittedValue || _lastEmittedValue != value)
	{
		_pendingValue = value;
		_lastChangeTime = millis();
		_hasPendingValue = true;
	}
}

template <typename T>
void OperatorDebounce<T>::Update()
{
	if (_hasPendingValue && (millis() - _lastChangeTime >= _debounceMillis))
	{
		this->_childObservers.OnNext(_pendingValue);
		_lastEmittedValue = _pendingValue;
		_hasEmittedValue = true;
		_hasPendingValue = false;
	}
}

template <typename T>
void OperatorDebounce<T>::Reset()
{
	_lastChangeTime = 0;
	_hasPendingValue = false;
	_hasEmittedValue = false;
}

#endif
