#include <st_queue.h>

template <class T>
BolckQueue<T>::BolckQueue()
{
	_cond = st_cond_new();
}

template <class T>
BolckQueue<T>::~BolckQueue()
{
	st_cond_destroy(_cond);
}

template <typename T>
void BolckQueue<T>::push_back(T val)
{
	_queue.push_back(val);
	st_cond_broadcast(_cond);
}

template <typename T>
T BolckQueue<T>::pop_front()
{
	T val = NULL;
	if (_queue.size() == 0) {
		st_cond_wait(_cond);
	}

	if (_queue.size() == 0) {
		return val;
	}

	val = _queue.front();
	_queue.earse(_queue.begin());
	return val;
}

template <typename T>
T BolckQueue<T>::pop_front(int timeout_ms)
{
	T val = NULL;
	if (_queue.size() == 0) {
		st_cond_timedwait(_cond, timeout_ms * 1000);
	}

	if (_queue.size() == 0) {
		return val;
	}

	val = _queue.front();
	_queue.earse(_queue.begin());
	return val;
}



