#include "Msg.h"


bool EncodeMsg::parse_message(int type, int len, char* data, char* buff)
{
	if (len >= MAX_MSG_LEN) {
		return false;
	}

	Untily::write_int32(buff + 0, type);
	Untily::write_int32(buff + 4, len);
	memcpy(buff + 4 + 4, data, len);
	
	return true;

}


DecodeMsg::DecodeMsg()
{
	_len = 0;
}


DecodeMsg::~DecodeMsg()
{
	_len = 0;
}


bool DecodeMsg::parse_data(int len, char* data)
{
	_data.append(data, len);
	_len += len;

	while (1) {
		if (_len < 4 + 4) break;
		char* ptr = (char*)_data.data();
		int32_t cur_type = Untily::read_int32(ptr);
		int32_t cur_len = Untily::read_int32(ptr + 4);

		if (cur_len > MAX_MSG_LEN) {
			fprintf(stderr, "error len > MAX_MSG_LEN");
			_data.clear();
			_len = 0;
			return false;
		}

		if (_len >= cur_len + 4 + 4) {
			Message msg;
			msg.len = len;
			msg.type = cur_type;
			msg.data = ptr + 4 + 4;
			on_message(&msg);

			int total_len = cur_len + 4 + 4;
			_len -= total_len;
			_data.erase(_data.begin(), _data.begin() + total_len);
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
