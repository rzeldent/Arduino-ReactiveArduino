/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOPERATORSTARTWITH_h
#define _REACTIVEOPERATORSTARTWITH_h

template <typename T>
class OperatorStartWith : public Operator<T, T>
{
public:
	OperatorStartWith(T startValue);
	OperatorStartWith(T* startValues, size_t count);

	void OnNext(T value) override;
	void Subscribe(IObserver<T> &observer) override;

private:
	T* _startValues;
	size_t _startCount;
	bool _hasStarted;
};

template <typename T>
OperatorStartWith<T>::OperatorStartWith(T startValue)
{
	_startValues = new T[1];
	_startValues[0] = startValue;
	_startCount = 1;
	_hasStarted = false;
}

template <typename T>
OperatorStartWith<T>::OperatorStartWith(T* startValues, size_t count)
{
	_startValues = new T[count];
	for (size_t i = 0; i < count; i++)
	{
		_startValues[i] = startValues[i];
	}
	_startCount = count;
	_hasStarted = false;
}

template <typename T>
void OperatorStartWith<T>::OnNext(T value)
{
	this->_childObservers.OnNext(value);
}

template <typename T>
void OperatorStartWith<T>::Subscribe(IObserver<T> &observer)
{
	this->_childObservers.Add(&observer);
	
	if (!_hasStarted)
	{
		for (size_t i = 0; i < _startCount; i++)
		{
			observer.OnNext(_startValues[i]);
		}
		_hasStarted = true;
	}
}

#endif
