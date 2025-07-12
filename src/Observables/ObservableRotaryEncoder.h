/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOBSERVABLEROTARYENCODER_h
#define _REACTIVEOBSERVABLEROTARYENCODER_h

struct RotaryEncoderData
{
	int position;
	int direction; // -1, 0, or 1
	bool buttonPressed;
	
	// Comparison operators for use with debounce and other operators
	bool operator==(const RotaryEncoderData& other) const {
		return position == other.position && direction == other.direction && buttonPressed == other.buttonPressed;
	}
	
	bool operator!=(const RotaryEncoderData& other) const {
		return !(*this == other);
	}
};

template <typename T>
class ObservableRotaryEncoder : public Observable<RotaryEncoderData>
{
public:
	ObservableRotaryEncoder(int pinA, int pinB, int buttonPin = -1);
	
	void Subscribe(IObserver<RotaryEncoderData> &observer) override;
	void UnSubscribe(IObserver<RotaryEncoderData> &observer) override;
	void Update();
	void Reset() override;
	
	void SetPosition(int position);
	int GetPosition() const;

private:
	int _pinA, _pinB, _buttonPin;
	int _position;
	int _lastStateA;
	int _lastStateB;
	bool _lastButtonState;
	unsigned long _lastDebounceTime;
	static const unsigned long DEBOUNCE_DELAY = 50;
	
	ObserverList<RotaryEncoderData> _childObservers;
};

template <typename T>
ObservableRotaryEncoder<T>::ObservableRotaryEncoder(int pinA, int pinB, int buttonPin)
{
	_pinA = pinA;
	_pinB = pinB;
	_buttonPin = buttonPin;
	_position = 0;
	_lastDebounceTime = 0;
	
	pinMode(_pinA, INPUT_PULLUP);
	pinMode(_pinB, INPUT_PULLUP);
	if (_buttonPin >= 0)
	{
		pinMode(_buttonPin, INPUT_PULLUP);
	}
	
	_lastStateA = digitalRead(_pinA);
	_lastStateB = digitalRead(_pinB);
	_lastButtonState = (_buttonPin >= 0) ? digitalRead(_buttonPin) : HIGH;
}

template <typename T>
void ObservableRotaryEncoder<T>::Subscribe(IObserver<RotaryEncoderData> &observer)
{
	this->_childObservers.Add(&observer);
}

template <typename T>
void ObservableRotaryEncoder<T>::UnSubscribe(IObserver<RotaryEncoderData> &observer)
{
	this->_childObservers.Remove(&observer);
}

template <typename T>
void ObservableRotaryEncoder<T>::Update()
{
	int currentStateA = digitalRead(_pinA);
	int currentStateB = digitalRead(_pinB);
	bool currentButtonState = (_buttonPin >= 0) ? digitalRead(_buttonPin) : HIGH;
	
	RotaryEncoderData data;
	data.position = _position;
	data.direction = 0;
	data.buttonPressed = !currentButtonState; // Assuming active low
	
	bool stateChanged = false;
	
	// Check for rotation
	if (currentStateA != _lastStateA)
	{
		if (millis() - _lastDebounceTime > DEBOUNCE_DELAY)
		{
			if (currentStateA == currentStateB)
			{
				_position++;
				data.direction = 1;
			}
			else
			{
				_position--;
				data.direction = -1;
			}
			data.position = _position;
			stateChanged = true;
			_lastDebounceTime = millis();
		}
	}
	
	// Check for button press
	if (currentButtonState != _lastButtonState)
	{
		if (millis() - _lastDebounceTime > DEBOUNCE_DELAY)
		{
			stateChanged = true;
			_lastDebounceTime = millis();
		}
	}
	
	if (stateChanged)
	{
		this->_childObservers.OnNext(data);
	}
	
	_lastStateA = currentStateA;
	_lastStateB = currentStateB;
	_lastButtonState = currentButtonState;
}

template <typename T>
void ObservableRotaryEncoder<T>::Reset()
{
	_position = 0;
	_lastDebounceTime = 0;
}

template <typename T>
void ObservableRotaryEncoder<T>::SetPosition(int position)
{
	_position = position;
}

template <typename T>
int ObservableRotaryEncoder<T>::GetPosition() const
{
	return _position;
}

#endif
