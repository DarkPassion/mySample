#include <stdio.h>
extern int _binary_data_txt_end;
extern int _binary_data_txt_size;
extern int _binary_data_txt_start;
int main(void)
{
	int size = (int)&_binary_data_txt_size;
	char *data = (char *)&_binary_data_txt_start;
	char *end = (char *)&_binary_data_txt_end;
	    
	printf("taille: %d\n", size);
	printf("buffer: %s", data);
	printf("start: 0x%p ; end: 0x%p\n",data,end);

	return 0;
}
