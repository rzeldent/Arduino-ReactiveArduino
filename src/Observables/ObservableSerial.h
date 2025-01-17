/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOBSERVABLESERIAL_h
#define _REACTIVEOBSERVABLESERIAL_h

template <typename T>
class ObservableSerial : public Observable<T>
{
public:
	ObservableSerial();
	void Subscribe(IObserver<T> &observer) override;
	void UnSubscribe(IObserver<T> &observer) override;
	virtual void Receive();

private:
	ObserverList<T> _childObservers;
};

template <typename T>
ObservableSerial<T>::ObservableSerial()
{
}

template <typename T>
void ObservableSerial<T>::Subscribe(IObserver<T> &observer)
{
	this->_childObservers.Add(&observer);
}

template <typename T>
void ObservableSerial<T>::UnSubscribe(IObserver<T> &observer)
{
	this->_childObservers.Remove(&observer);
}

template <typename T>
void ObservableSerial<T>::Receive()
{
}
#endif