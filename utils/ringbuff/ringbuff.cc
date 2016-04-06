#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

// g++ ringbuff.cc -o0 -g -Wall -o ringbuff
class ring_buff
{

public:
	ring_buff() : _buff(NULL), _size(0), _write_pos(0), _read_pos(0)
	{
		_size = 32;
		resize();
	}


	~ring_buff()
	{
		if (_buff) {
			delete [] _buff;
		}
		_size = 0;
		_write_pos = _read_pos = 0;
	}


	int left_size()
	{
		if (_read_pos != 0) {
			assert(_read_pos <= _size);
		}
		if (_write_pos != 0) {
			assert(_write_pos <= _size);
		}
		
		int r = 0;
		if (_write_pos > _read_pos) {
			r = _size - _write_pos + _read_pos;
		} else if (_write_pos < _read_pos) {
			r = _read_pos - _write_pos;
		} else if (_write_pos == _read_pos){
			if (_read_pos == 0) {
				r = _size;
			} else {
				r = 0;
			}
		}

		return r;
	}


	void push_back(char* data, int len)
	{
		int l = left_size();
		printf(" push_back [%d %d %d %d %d]\n", l, len, _size, _write_pos, _read_pos);

		if (l < len)
		{
			resize();
		}

		if (_write_pos + len <= _size)
		{
			/* code */
			memcpy(_buff + _write_pos, data, len);
			_write_pos += len;
			assert(_write_pos <= _size);
		} else
		{
			int n1 = _size - _write_pos;
			memcpy(_buff + _write_pos, data, n1);
			memcpy(_buff, data + n1, len - n1);
			_write_pos = len -n1;
			assert(_write_pos <= _size);
			assert(_write_pos <= _read_pos);
		}
	}

	char* pop_front(int len)
	{
		printf(" pop_front [%d %d %d ]\n", _size, _write_pos, _read_pos);
		int n_read = 0;

		if (_write_pos == _read_pos) {
			n_read = 0;
		} else if (_write_pos > _read_pos) {
			n_read = _write_pos - _read_pos;
		} else {
			n_read = _size - _read_pos + _write_pos;
		}

		if (len > n_read || n_read == 0) {
			return NULL;
		}

		char* ptr = new char[len];
		memset(ptr, 0, len);
		if (_write_pos > _read_pos) {
			memcpy(ptr, _buff + _read_pos, len);
			_read_pos += len;
		} else if (_write_pos < _read_pos) {

			if (len > _size - _read_pos) {
				int n1 = _size - _read_pos;
				int n2 = len - _size + _read_pos;
				assert(n1 > 0);
				assert(n2 > 0);
				memcpy(ptr, _buff + _read_pos, _size - _read_pos);
				memcpy(ptr, _buff, len - _size + _read_pos);
				_read_pos += len;
			} else {
				memcpy(ptr, _buff + _read_pos, len);
				_read_pos += len;
			}
		}

		return ptr;

	}


	void resize()
	{

		int len = _size * 2;
		char* tmp = new char[len];
		memset(tmp, 0, len);

		int n_read = 0;
		if (_read_pos < _write_pos) {
			n_read = _write_pos - _read_pos;
			memcpy(tmp, _buff + _read_pos, n_read);
			_write_pos = n_read;
			_read_pos = 0;
		} else if (_read_pos > _write_pos){
			n_read = _size - _read_pos + _write_pos;
			memcpy(tmp, _buff + _read_pos, _size - _read_pos);
			memcpy(tmp + _size - _read_pos, _buff, _write_pos);
			_write_pos = n_read;
			_read_pos = 0;
		} else {
			_write_pos = _read_pos = 0;
		}

		_size = len;
		if (_buff) {
			delete[] _buff;
		}
		_buff = tmp;

	}

private:
	char* _buff;
	int _size;
	int	_write_pos;
	int _read_pos;

};




int main()
{

	ring_buff buff;

	printf("ring buffer s %d \n", buff.left_size());
	char foo[10] = {0};
	const char* pfo = "hello ";
	sprintf(foo, pfo, strlen(pfo));
	const int cc = 10;
	for (int i = 0; i < cc; i++) {
		buff.push_back(foo, strlen(foo));
		printf("push back\n");
	}
	
	printf("total write == %lu \n", strlen(pfo) * cc);

	printf("ring buffer s %d \n", buff.left_size());

	for (int i = 0; i < 2; ++i)
	{
		char* temp_ptr = buff.pop_front(7);
		char temp_ff[11] = {0};
		memcpy(temp_ff, temp_ptr, 7);
		delete[] temp_ptr;
		printf(" pop front ptr ---- %s ---- n %d \n", temp_ff, i);
	}

	for (int i = 0; i < cc; i++) {
		buff.push_back(foo, strlen(foo));
		printf("push back \n");
	}

	
	int n = 0;
	while(1) {
		char* ptr = buff.pop_front(7);
		if (ptr == NULL) {
			break;
		}
		n++;
		char ff[10] = {0};
		memcpy(ff, ptr, 7);
		printf(" pop front ptr -- %s   --- n %d \n", ff, n);
		delete[] ptr;
	}

	printf("ring buffer s %d \n", buff.left_size());
	return 0;
}