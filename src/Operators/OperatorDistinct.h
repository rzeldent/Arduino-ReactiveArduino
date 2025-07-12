/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOPERATORDISTINCT_h
#define _REACTIVEOPERATORDISTINCT_h

template <typename T>
class OperatorDistinct : public Operator<T, T>
{
public:
	OperatorDistinct();

	void OnNext(T value) override;
	void Reset() override;

private:
	static const size_t MAX_DISTINCT_VALUES = 32; // Configurable limit for Arduino memory constraints
	T _seenValues[MAX_DISTINCT_VALUES];
	size_t _seenCount;
	
	bool hasSeenValue(T value);
	void addValue(T value);
};

template <typename T>
OperatorDistinct<T>::OperatorDistinct()
{
	_seenCount = 0;
}

template <typename T>
void OperatorDistinct<T>::OnNext(T value)
{
	if (!hasSeenValue(value))
	{
		addValue(value);
		this->_childObservers.OnNext(value);
	}
}

template <typename T>
void OperatorDistinct<T>::Reset()
{
	_seenCount = 0;
}

template <typename T>
bool OperatorDistinct<T>::hasSeenValue(T value)
{
	for (size_t i = 0; i < _seenCount; i++)
	{
		if (_seenValues[i] == value)
		{
			return true;
		}
	}
	return false;
}

template <typename T>
void OperatorDistinct<T>::addValue(T value)
{
	if (_seenCount < MAX_DISTINCT_VALUES)
	{
		_seenValues[_seenCount] = value;
		_seenCount++;
	}
	// If we exceed the limit, we stop tracking new values
	// This is a memory-conscious approach for Arduino
}

#endif
