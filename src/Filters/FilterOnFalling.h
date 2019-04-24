/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _FILTERONFALLING_h
#define _FILTERONFALLING_h

template <typename T>
class FilterOnFalling : public Operator<T, T>
{
public:
	FilterOnFalling();

	void OnNext(T value);
	//void OnComplete();

private:
	T _last = false;
	bool _any = false;
};

template <typename T>
FilterOnFalling<T>::FilterOnFalling()
{
}

template <typename T>
void FilterOnFalling<T>::OnNext(T value)
{
	if (_any && (value < _last))
		if (this->_childObserver != nullptr) this->_childObserver->OnNext(value);

	_last = value;
	_any = true;
}
#endif
