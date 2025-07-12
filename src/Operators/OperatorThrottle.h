/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOPERATORTHROTTLE_h
#define _REACTIVEOPERATORTHROTTLE_h

template <typename T>
class OperatorThrottle : public Operator<T, T>
{
public:
	OperatorThrottle(unsigned long intervalMillis);

	void OnNext(T value) override;
	void Reset() override;

private:
	unsigned long _intervalMillis;
	unsigned long _lastEmitTime;
	bool _hasEmitted;
};

template <typename T>
OperatorThrottle<T>::OperatorThrottle(unsigned long intervalMillis)
{
	_intervalMillis = intervalMillis;
	_lastEmitTime = 0;
	_hasEmitted = false;
}

template <typename T>
void OperatorThrottle<T>::OnNext(T value)
{
	unsigned long currentTime = millis();
	
	if (!_hasEmitted || (currentTime - _lastEmitTime >= _intervalMillis))
	{
		this->_childObservers.OnNext(value);
		_lastEmitTime = currentTime;
		_hasEmitted = true;
	}
}

template <typename T>
void OperatorThrottle<T>::Reset()
{
	_lastEmitTime = 0;
	_hasEmitted = false;
}

#endif
