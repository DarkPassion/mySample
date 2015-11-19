
#ifndef _ST_QUEUE_H_
#define _ST_QUEUE_H_


#include <st.h>
#include <vector>

template<class T>
class BolckQueue
{
public:
	BolckQueue();
	~BolckQueue();


public:
	
	T pop_front();

	T pop_front(int timeout_ms);
	
	void push_back(T val);

	int size();


private:
	std::vector<T> _queue;
	st_cond_t	_cond;
};

#endif