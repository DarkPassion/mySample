#include <stdlib.h>
#include <stdio.h>
#include "st_msg.h"


bool EncodeMsg::parse_message(int type, int len, const char* data, char* buff)
{
	if (len >= MAX_MSG_LEN) {
		return false;
	}

	utility::write_int32(buff + 0, type);
	utility::write_int32(buff + 4, len);
	memcpy(buff + 4 + 4, data, len);
	
	return true;

}


DecodeMsg::DecodeMsg()
{
	_on_notify = NULL;
}


DecodeMsg::~DecodeMsg()
{
	_on_notify = NULL;
}


bool DecodeMsg::parse_data(int len, char* data)
{
	for (int i = 0; i < len; i++)
	{
		_data.push_back(data[i]);
	}

	while (1) {
		if (_data.size() < 4 + 4) break;
		char* ptr = &(_data.at(0));
		int32_t cur_type = utility::read_int32(ptr);
		int32_t cur_len = utility::read_int32(ptr + 4);

		if (cur_len > MAX_MSG_LEN) {
			fprintf(stderr, "error len > MAX_MSG_LEN");
			_data.clear();
			return false;
		}

		if (_data.size() >= cur_len + 4 + 4) {
			Message msg;
			msg.len = cur_len;
			msg.type = cur_type;
			msg.data = ptr + 4 + 4;
			if (_on_notify) {
				_on_notify->on_message(&msg);
			} else {
				on_message(&msg);
			}
			

			int total_len = cur_len + 4 + 4;
			_data.erase(_data.begin(), _data.begin() + total_len);
			//printf("DecodeMsg size [%lu] \n", _data.size());
		} else {
			break;
		}
	}
	




	return true;
}

void DecodeMsg::on_message(Message* msg)
{
	printf("type [%d] len [%d] data[%s]\n", msg->type, msg->len, msg->data);
}
