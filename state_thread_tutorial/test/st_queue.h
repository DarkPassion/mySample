
#ifndef _ST_QUEUE_H_
#define _ST_QUEUE_H_


#include <st.h>
#include <vector>

template<class T>
class BolckQueue
{
public:
	BolckQueue()
	{
		_cond = st_cond_new();
	}

	~BolckQueue()
	{
		st_cond_destroy(_cond);
	}


public:
	
	T pop_front()
	{
		T val = NULL;
		if (_queue.size() == 0) {
			st_cond_wait(_cond);
		}

		if (_queue.size() == 0) {
			return val;
		}

		val = _queue.front();
		_queue.erase(_queue.begin());
		return val;
	}

	T pop_front(int timeout_ms)
	{
		T val = NULL;
		if (_queue.size() == 0) {
			st_cond_timedwait(_cond, timeout_ms * 1000);
		}

		if (_queue.size() == 0) {
			return val;
		}

		val = _queue.front();
		_queue.erase(_queue.begin());
		return val;
	}
	
	void push_back(T val)
	{
		_queue.push_back(val);
		st_cond_broadcast(_cond);
	}

	int size()
	{
		return _queue.size();
	}

	void clear()
	{
		while(_queue.size() > 0) {
			_queue.erase(_queue.begin());
		}
	}

private:
	std::vector<T> _queue;
	st_cond_t	_cond;
};

#endif
