#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

class ring_buff
{

public:
	ring_buff() : _buff(NULL), _size(0), _write_pos(0), _read_pos(0)
	{

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
		} else {
			r = 0;
		}

		return r;
	}


	void push_back(char* data, int len)
	{
		int l = left_size();
		if (l < len) {
			int s = _size * 2 > len + _size ? _size * 2 : len + _size;
			resize(s);
			memcpy(_buff + _write_pos, data, len);
			_write_pos += len;
			assert(_write_pos <= _size);
		} else {
			memcpy(_buff + _write_pos, data, len);
			_write_pos += len;
			assert(_write_pos <= _size);
		}

	}

	char* pop_front(int len)
	{
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


	void resize(int len)
	{
		if (len < _size) {
			return;
		}

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

	char foo[10] = {0};
	const char* pfo = "hello ";
	sprintf(foo, pfo, strlen(pfo));
	for (int i = 0; i < 10; i++) {
		buff.push_back(foo, strlen(foo));
	}
	
	printf("total write == %lu \n", strlen(pfo) * 10);

	int n = 0;
	while(1) {
		char* ptr = buff.pop_front(7);
		if (ptr == NULL) {
			break;
		}
		n++;
		char ff[10] = {0};
		memcpy(ff, ptr, 7);
		printf(" ptr -- %s   --- n %d \n", ff, n);
		delete[] ptr;
	}
	return 0;
}