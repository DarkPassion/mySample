//
//  main.cpp
//  pb
//
//  Created by zhangzhifan on 16/3/23.
//  Copyright © 2016年 zhangzhifan. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/compiler/importer.h"


using namespace google::protobuf;
using namespace google::protobuf::compiler;
using namespace std;


void test_dynamic_complie_proto();

void test_protpmgr_proto();


class CompilerErrorCollector : public MultiFileErrorCollector {
public:
    void AddError(const string &filename, int line, int /*column*/, const string &message) {
        if (line == -1) {
            fprintf(stdout, "compile proto file error: %s, %s", filename.c_str(), message.c_str());
        } else {
            fprintf(stdout, "compile proto file error: %s:%d, %s", filename.c_str(), line+1, message.c_str());
        }
    }
};

void test_dynamic_complie_proto()
{
    DiskSourceTree sourceTree;
    CompilerErrorCollector errorCollector;
    
    //look up .proto file in current directory
    
    sourceTree.MapPath("",".");
    
    Importer importer(&sourceTree, &errorCollector);
    
    //runtime compile foo.proto
    
    char buff_cwd[256] = {0};
    if (true) {
        getcwd(buff_cwd, sizeof(buff_cwd));
        const char* pwd = "/Users/zhangzhifan/work/git-code/mySample/google-protobuff/mypb/pb/res";
        
        chdir(pwd);
    }
    
    importer.Import("message.proto");
    
    if (true) {
        chdir(buff_cwd);
    }
    
    const Descriptor *descriptor =   importer.pool()->FindMessageTypeByName("LogonReqMessage");
    
    if (descriptor == NULL) {
        fprintf(stdout, "importer pool FindMessageTypeByName ERROR ! \n");
        
        return;
    }
    
    cout << "descriptor debug string "<< descriptor->DebugString() << endl;
    
    // build a dynamic message by "Pair" proto
    DynamicMessageFactory factory;
    
    const Message *message = factory.GetPrototype(descriptor);
    
    // create a real instance of "Pair"
    
    Message *pair = message->New();
    
    // write the "Pair" instance by reflection
    
    const Reflection *reflection = pair->GetReflection();
    
    const FieldDescriptor *field = NULL;
    
    field = descriptor->FindFieldByName("acctID");
    
    reflection->SetInt64(pair, field, 100011);
    
    field = descriptor->FindFieldByName("passwd");
    
    reflection->SetString(pair, field, "zhangzhifan");
    
    cout << " pari debug string : " << pair->DebugString() << endl;
    
    
    string encode_string = pair->SerializeAsString();
    cout << "encode string :" << encode_string <<  "encode string len :" << encode_string.length() <<endl;
}



class ProtoBuffMgr
{
public:
    ProtoBuffMgr();
    
    ~ProtoBuffMgr();
    
    void init_proto(std::string root_path);
    
private:
    Importer*    _import;
    DiskSourceTree*  _sourceTree;
    DynamicMessageFactory _factory;
    CompilerErrorCollector _error;
    std::string _cwd_path;
    std::string _root_path;
};



ProtoBuffMgr::ProtoBuffMgr()
{
    _sourceTree = new DiskSourceTree();
    _sourceTree->MapPath("", ".");
    _import = new Importer(_sourceTree, &_error);
    
    
}

ProtoBuffMgr::~ProtoBuffMgr()
{
    delete _import;
    delete _sourceTree;
    
}


void ProtoBuffMgr::init_proto(std::string root_path)
{
    char buff[256] = {0};
    getcwd(buff, sizeof(buff));
    
    _cwd_path = buff;
    _root_path = root_path;
    
    chdir(root_path.c_str());
    
    _import->Import("import.proto");
    
    chdir(buff);
    
    if (true) {
    
        const Descriptor *descriptor =   _import->pool()->FindMessageTypeByName("message1");
        
        if (descriptor == NULL) {
            fprintf(stdout, "importer pool FindMessageTypeByName ERROR ! \n");
            
            return;
        }
        
        cout << "descriptor debug string "<< descriptor->DebugString() << endl;
        
        // build a dynamic message by "Pair" proto
        DynamicMessageFactory factory;
        
        const Message *message = factory.GetPrototype(descriptor);
        
        // create a real instance of "Pair"
        
        Message *pair = message->New();
        
        // write the "Pair" instance by reflection
        
        const Reflection *reflection = pair->GetReflection();
        
        const FieldDescriptor *field = NULL;
        
        field = descriptor->FindFieldByName("acctID");
        
        reflection->SetInt64(pair, field, 100011);
        
        field = descriptor->FindFieldByName("passwd");
        
        reflection->SetString(pair, field, "sssppppp");
        
        cout << " pari debug string : " << pair->DebugString() << endl;
        
        
        string encode_string = pair->SerializeAsString();
        cout << "encode string :" << encode_string <<  "encode string len :" << encode_string.length() <<endl;
        
        if (true) {
            Message * msg = message->New();
            bool succ = msg->ParseFromString(encode_string);
            
            if (succ == false) {
                fprintf(stdout, "parseFromString encode string ERR ! \n");
                return;
            }
            
            const FieldDescriptor *field = NULL;
            
            field = descriptor->FindFieldByName("acctID");
            
            int64 i = reflection->GetInt64(*msg, field);
            
            fprintf(stdout, "reflection GetInt64 %lld \n", i);
            
            field = descriptor->FindFieldByName("passwd");
            
            string s = reflection->GetString(*msg, field);
            
            fprintf(stdout, "reflection GetString %s \n", s.c_str());
            
            delete msg;
        }
        
        delete pair;
    }
}



void test_protpmgr_proto()
{
    ProtoBuffMgr _mgr;
    const char* pwd = "/Users/zhangzhifan/work/git-code/mySample/google-protobuff/mypb/pb/res";
    _mgr.init_proto(pwd);
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    test_dynamic_complie_proto();
    test_protpmgr_proto();

    return 0;
}
