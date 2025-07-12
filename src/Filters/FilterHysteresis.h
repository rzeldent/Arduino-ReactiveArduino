/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEFILTERHYSTERESIS_h
#define _REACTIVEFILTERHYSTERESIS_h

template <typename T>
class FilterHysteresis : public Operator<T, T>
{
public:
	FilterHysteresis(T lowThreshold, T highThreshold);

	void OnNext(T value) override;
	void Reset() override;

private:
	T _lowThreshold;
	T _highThreshold;
	T _lastOutput;
	bool _hasOutput;
};

template <typename T>
FilterHysteresis<T>::FilterHysteresis(T lowThreshold, T highThreshold)
{
	_lowThreshold = lowThreshold;
	_highThreshold = highThreshold;
	_hasOutput = false;
}

template <typename T>
void FilterHysteresis<T>::OnNext(T value)
{
	T output = value;
	
	if (_hasOutput)
	{
		// Apply hysteresis logic
		if (_lastOutput > value && value < _lowThreshold)
		{
			output = _lowThreshold;
		}
		else if (_lastOutput < value && value > _highThreshold)
		{
			output = _highThreshold;
		}
		else
		{
			output = _lastOutput; // Keep previous state
		}
	}
	else
	{
		// First value - determine initial state
		if (value <= _lowThreshold)
		{
			output = _lowThreshold;
		}
		else if (value >= _highThreshold)
		{
			output = _highThreshold;
		}
		_hasOutput = true;
	}
	
	if (!_hasOutput || output != _lastOutput)
	{
		this->_childObservers.OnNext(output);
		_lastOutput = output;
		_hasOutput = true;
	}
}

template <typename T>
void FilterHysteresis<T>::Reset()
{
	_hasOutput = false;
}

#endif
