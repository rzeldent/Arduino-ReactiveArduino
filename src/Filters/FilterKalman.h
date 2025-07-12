/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEFILTERKALMAN_h
#define _REACTIVEFILTERKALMAN_h

template <typename T>
class FilterKalman : public Operator<T, T>
{
public:
	FilterKalman(T processVariance, T measurementVariance, T estimatedError = 1.0);

	void OnNext(T value) override;
	void Reset() override;

private:
	T _processVariance;     // Process noise covariance
	T _measurementVariance; // Measurement noise covariance  
	T _estimatedError;      // Estimation error covariance
	T _kalmanGain;          // Kalman gain
	T _currentEstimate;     // Current estimate
	bool _isFirst;
};

template <typename T>
FilterKalman<T>::FilterKalman(T processVariance, T measurementVariance, T estimatedError)
{
	_processVariance = processVariance;
	_measurementVariance = measurementVariance;
	_estimatedError = estimatedError;
	_isFirst = true;
}

template <typename T>
void FilterKalman<T>::OnNext(T value)
{
	if (_isFirst)
	{
		_currentEstimate = value;
		_isFirst = false;
	}
	else
	{
		// Prediction update
		_estimatedError += _processVariance;
		
		// Measurement update
		_kalmanGain = _estimatedError / (_estimatedError + _measurementVariance);
		_currentEstimate = _currentEstimate + _kalmanGain * (value - _currentEstimate);
		_estimatedError = (1 - _kalmanGain) * _estimatedError;
	}
	
	this->_childObservers.OnNext(_currentEstimate);
}

template <typename T>
void FilterKalman<T>::Reset()
{
	_isFirst = true;
	_estimatedError = 1.0;
}

#endif
