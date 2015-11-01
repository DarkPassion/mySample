#include <string>
#include <assert.h>
#include "hello.pb.h"

#pragma comment(lib, "libprotobuf.lib")
int main()
{

	Person ps;
	std::string name = "google-protobuff";
	std::string email = "protobuff@gmail.com";
	int id = 1120;
	ps.set_name(name.c_str(), name.length());
	ps.set_email(email.c_str(), email.length());
	ps.set_id(id);
	

	std::string ss = ps.SerializeAsString();
	
	Person ps2;
	ps2.ParseFromString(ss);

	assert(ps2.id() == id);
	assert(ps2.name() == name);
	assert(ps2.email() == email);
	
	printf("ps name[%s] email[%s] id[%d]\n", name.c_str(), email.c_str(), id);
	
	system("pause");
	
	return 0;
}