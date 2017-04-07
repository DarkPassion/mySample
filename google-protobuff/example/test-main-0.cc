#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/compiler/importer.h"
// g++ test-main-0.cc -I./../include/ -L./../libs/ -lprotobuf -o test-main-0.out


using namespace google::protobuf;
using namespace google::protobuf::compiler;
using namespace std;


#define ASSERT_FALSE(x, s) if (x) { printf("%s func [%s] \n", s, __FUNCTION__); abort(); }
#define ASSERT_TRUE(x, s) if (!x) { printf("%s func [%s] \n", s, __FUNCTION__); abort(); }

const char* outfile = "outfile.bin";

void test_dynamic_compile();
void test_read_message();
Message* create_message(std::string name);


class CompilerErrorCollector : public MultiFileErrorCollector {
    public:
        void AddError(const string &filename, int line, int /*column*/, const string &message) {
            if (line == -1) {
                printf("compile proto file error: %s, %s", filename.c_str(), message.c_str());
            } else {
                printf("compile proto file error: %s:%d, %s", filename.c_str(), line+1, message.c_str());
            }
        }
};

class MyImporter
{
	public:

		MyImporter()
		{
			_importer = NULL;

		}

		~MyImporter()
		{
			delete _importer;
		}

		void map_path(std::string vpath, std::string rpath)
		{
			_sourceTree.MapPath(vpath, rpath);
			_importer = new Importer(&_sourceTree, &_compilerErr);
		}

		void import(std::string name)
		{
			if (_importer) {

				_importer->Import(name);
				printf("== import name [%s] \n", name.c_str());
			}

		}


		Message* create_message(std::string name)
		{

			google::protobuf::Message* message = NULL;
			const google::protobuf::Descriptor* descriptor = _importer->pool()->FindMessageTypeByName(name);
			if (descriptor)
			{
				DynamicMessageFactory factory;
				const Message* prototype = factory.GetPrototype(descriptor);
				if (prototype) {
					message = prototype->New();
				}
			}
			return message;

		}


	private:
		Importer* 		_importer;
		DiskSourceTree 	_sourceTree;
		CompilerErrorCollector _compilerErr;


};


void test_dynamic_compile()
{

	MyImporter import;
	import.map_path("", ".");

	import.import("test-message.proto");




	Message* request = import.create_message("BaoBao.hello.Request");

	if (request) {
		printf("create message succ ! \n");
	} else {
		printf("create message fail! \n");
		return ;
	}

	if (true) {

		const Reflection* ref = request->GetReflection();
		const FieldDescriptor* field = NULL;
		const Descriptor* desc = request->GetDescriptor();

		ASSERT_TRUE(ref, "GetReflection --- fail ! \n");

		ASSERT_TRUE(desc, "GetDescriptor --- fail ! \n");

		field = desc->FindFieldByName("name");
		ASSERT_TRUE(field, "find filed by name (name) fail");
		ref->SetString(request, field, "name==1");

		field = desc->FindFieldByName("addres");
		ASSERT_TRUE(field, "find filed by name (addres) fail");
		ref->SetString(request, field, "addres==1");

		field = desc->FindFieldByName("from");
		ASSERT_TRUE(field, "find field by name (from) fail");
		ref->SetString(request, field, "from==1");

		fstream fs(outfile, ios::out | ios::trunc | ios::binary);
		if (!request->SerializeToOstream(&fs)) {
			printf("request SerializeToOstream fs fail! \n");
		}

		delete request;
	}

}

void test_read_message()
{
	Message* request = create_message("BaoBao.hello.Request");
	if (request) {


	}


}

Message* create_message(std::string name)
{
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor =
			google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(name);
	if (descriptor)
	{
		const google::protobuf::Message* prototype =
				google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
			message = prototype->New();
		}
	}
	return message;
}





int main()
{


	test_dynamic_compile();

	test_read_message();


    return 0;
}



