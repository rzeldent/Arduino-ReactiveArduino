/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _AGGREGATECOUNT_h
#define _AGGREGATECOUNT_h

template <typename T>
class AggregateCount : public Operator<T, int>
{
public:
	AggregateCount();

	void OnNext(T value);
	//void OnComplete();

private:
	int _count = false;
};

template <typename T>
AggregateCount<T>::AggregateCount()
{
}

template <typename T>
void AggregateCount<T>::OnNext(T value)
{
	_count++;
	this->_childObserver->OnNext(_count);
}

//template <typename T>
//void AggregateCount<T>::OnComplete()
//{
//	this->_childObserver->OnComplete();
//}

#endif