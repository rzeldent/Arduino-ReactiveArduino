/***************************************************
Copyright (c) 2019 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _REACTIVEOPERATORREDUCE_h
#define _REACTIVEOPERATORREDUCE_h

template <typename Torig, typename Tdest>
class OperatorReduce : public Operator<Torig, Tdest>
{
public:
	typedef Tdest(*ReactiveReduceFunction)(Tdest, Torig);
	
	OperatorReduce(ReactiveReduceFunction function, Tdest initialValue);

	void OnNext(Torig value) override;
	void OnComplete() override;
	void Reset() override;

private:
	ReactiveReduceFunction _function;
	Tdest _accumulator;
	Tdest _initialValue;
	bool _hasValue;
};

template <typename Torig, typename Tdest>
OperatorReduce<Torig, Tdest>::OperatorReduce(ReactiveReduceFunction function, Tdest initialValue)
{
	_function = function;
	_initialValue = initialValue;
	_accumulator = initialValue;
	_hasValue = false;
}

template <typename Torig, typename Tdest>
void OperatorReduce<Torig, Tdest>::OnNext(Torig value)
{
	// Accumulate the value but don't emit yet
	_accumulator = _function(_accumulator, value);
	_hasValue = true;
}

template <typename Torig, typename Tdest>
void OperatorReduce<Torig, Tdest>::OnComplete()
{
	// Only emit the final accumulated result when the observable completes
	if (_hasValue || _accumulator != _initialValue)
	{
		this->_childObservers.OnNext(_accumulator);
	}
	
	// Forward the completion signal
	Operator<Torig, Tdest>::OnComplete();
}

template <typename Torig, typename Tdest>
void OperatorReduce<Torig, Tdest>::Reset()
{
	_accumulator = _initialValue;
	_hasValue = false;
}

#endif
