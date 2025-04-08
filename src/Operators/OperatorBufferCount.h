/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOPERATORBUFFERCOUNT_h
#define _REACTIVEOPERATORBUFFERCOUNT_h

template <typename T>
class OperatorBufferCount: public Operator<T, T[]>
{
public:
	OperatorBufferCount(size_t N);

	void OnNext(T value) override;
	
	void Reset() override;
private:
	std::vector<T> _buffer;
	size_t _num_elements = 0;
};

template <typename T>
OperatorBufferCount<T>::OperatorBufferCount(size_t N)
{
	_num_elements = N;
}

template <typename T>
void OperatorBufferCount<T>::Reset()
{
	_buffer.clear();
}

template <typename T>
void OperatorBufferCount<T>::OnNext(T value)
{
	_buffer.push_back(value);
	if (_buffer.size() >= _num_elements)
	{
		this->_childObservers.OnNext(_buffer.data());
		_buffer.clear();
	}
}

#endif