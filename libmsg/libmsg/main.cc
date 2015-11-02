#include <assert.h>
#include "Msg.h"


void on_message(Message* msg)
{
	printf("msg type[%x] len [%d] data[%s]\n", msg->type, msg->len, msg->data);
}
int main()
{

	char buff[256] = {0};
	
	char msg_array[100] = {0};

	sprintf(msg_array, "helloworld");
	EncodeMsg::parse_message(0x123456, strlen(msg_array), msg_array, buff);

	DecodeMsg de;
	de.set_call_back(on_message);
	for (int i = 0; i < strlen(msg_array) + 8; i++)
	{
		de.parse_data(1, &buff[i]);
	}
	
	memset(buff, 0, sizeof(buff));
	utility::write_int16(buff, 0x123);
	int n_read = utility::read_int16(buff);
	assert(n_read == 0x123);
	
	memset(buff, 0, sizeof(buff));
	
	AVMsg avmsg;
	memset(&avmsg, 0, sizeof(avmsg));
	char temp_buff[256] = "libmsg test console!";
	avmsg.data = temp_buff;
	avmsg.len = strlen(temp_buff);
	utility::write_vamsg(buff, &avmsg);

	int len = utility::read_vamsg(buff);
	AVMsg lmsg;
	lmsg.data = new char[len];
	lmsg.len = len;
	utility::read_vamsg(buff, lmsg);
	delete[] lmsg.data;

	
	memset(buff, 0, sizeof(buff));
	utility::write_int64(buff, 0x112233445566);
	int64_t r64 = utility::read_int64(buff);
	assert(r64 == 0x112233445566);

	gets(buff);
	return 0;
}