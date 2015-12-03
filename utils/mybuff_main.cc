#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>


using namespace std;


class TBuff
{
public:
	TBuff()
	{
		_list.reserve(128);
	}

	~TBuff()
	{
		clear();
	}

	void clear()
	{
		std::vector<unsigned char> v;
		_list.swap(v);
	}

	void push_back(unsigned char* buff, int len)
	{
		int i = 0;

		while(len > 0) {
			_list.push_back(buff[i++]);
			len--;
		}
	}


	unsigned char* pop_front(int len)
	{
		int s = _list.size();

		if (s < len)
			return NULL;

		unsigned char* ptr = new unsigned char[len];
		unsigned char* src = &(_list.at(0));
		memset(ptr, 0, len);
		memcpy(ptr, src, len);
		_list.erase(_list.begin(), _list.begin() + len);

		return ptr;
	}

private:
	std::vector<unsigned char> _list;
};




int main()
{

	TBuff mybuff;

	for(int i = 0; i < 10; i++) {

		char* ptr = new char[800];
		const char* c = "hello";
		memcpy(ptr, c, strlen(c));
		mybuff.push_back((unsigned char*)ptr, 800);
		delete [] ptr;
	}

	int c = (10 * 800) / 812;
	printf("c = %d \n", c);
	int n = 0;
	while(1) {
		unsigned char* ptr = mybuff.pop_front(812);
		if (ptr == NULL) {
			break;
		}

		n++;
		printf("n == %d\n", n);

	}

    return 0;
}

