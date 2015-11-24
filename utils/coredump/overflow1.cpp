#include <string.h>
#include <stdio.h>
#include <stdlib.h>


class CAMem
{
public:
	CAMem()
	{
		buff = new char[5];
		len = 5;
		const char* ptr = "aabbccddeeffgghhkkllmmnnooppqq";
		printf("ptr len : %u\n", strlen(ptr));
		memcpy(buff, ptr, strlen(ptr));
	}

	~CAMem()
	{
		// will coredump
		delete buff;
	}

private:
	char* buff;
	int len;
};


int main()
{

	CAMem* m = new CAMem();
	delete m;

	printf("end!");
	return 0;
}
