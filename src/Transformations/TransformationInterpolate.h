/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVETRANSFORMATIONINTERPOLATE_h
#define _REACTIVETRANSFORMATIONINTERPOLATE_h

template <typename T>
class TransformationInterpolate : public Operator<T, T>
{
public:
	TransformationInterpolate(T inputMin, T inputMax, T outputMin, T outputMax, bool constrain = true);

	void OnNext(T value) override;

private:
	T _inputMin, _inputMax;
	T _outputMin, _outputMax;
	bool _constrain;
	T _inputRange, _outputRange;
};

template <typename T>
TransformationInterpolate<T>::TransformationInterpolate(T inputMin, T inputMax, T outputMin, T outputMax, bool constrain)
{
	_inputMin = inputMin;
	_inputMax = inputMax;
	_outputMin = outputMin;
	_outputMax = outputMax;
	_constrain = constrain;
	_inputRange = _inputMax - _inputMin;
	_outputRange = _outputMax - _outputMin;
}

template <typename T>
void TransformationInterpolate<T>::OnNext(T value)
{
	T result;
	
	if (_inputRange == 0)
	{
		result = _outputMin;
	}
	else
	{
		// Linear interpolation: output = outputMin + (value - inputMin) * (outputRange / inputRange)
		result = _outputMin + ((value - _inputMin) * _outputRange) / _inputRange;
	}
	
	// Apply constraints if enabled
	if (_constrain)
	{
		if (_outputMin < _outputMax)
		{
			if (result < _outputMin) result = _outputMin;
			if (result > _outputMax) result = _outputMax;
		}
		else
		{
			if (result > _outputMin) result = _outputMin;
			if (result < _outputMax) result = _outputMax;
		}
	}
	
	this->_childObservers.OnNext(result);
}

#endif
