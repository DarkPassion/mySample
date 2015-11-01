#include "Msg.h"


int main()
{

	char buff[256] = {0};
	
	char msg_array[100] = {0};

	sprintf(msg_array, "helloworld");
	EncodeMsg::parse_message(1, strlen(msg_array), msg_array, buff);

	DecodeMsg de;

	for (int i = 0; i < strlen(msg_array) + 8; i++)
	{
		de.parse_data(1, &buff[i]);
	}
	
	gets(buff);
	return 0;
}