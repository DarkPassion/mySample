//
//  main.cpp
//  pb
//
//  Created by zhangzhifan on 16/3/23.
//  Copyright © 2016年 zhangzhifan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
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
    
    const Descriptor *descriptor =   importer.pool()->FindMessageTypeByName("messageHeader");
    
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
    
    field = descriptor->FindFieldByName("cmd");
    
    reflection->SetInt64(pair, field, 100011);
    
    field = descriptor->FindFieldByName("msg_name");
    
    reflection->SetString(pair, field, "ffm");
    
    cout << " pari debug string : " << pair->DebugString() << endl;
    
    
    string encode_string = pair->SerializeAsString();
    cout << "encode string :" << encode_string <<  "encode string len :" << encode_string.length() <<endl;
}


// [ pb-header-len ] [ pb-header ] [ pb-contentxx-len ]  [ pb-contentxx ]
// [ pb-header-len ] [ pb-header ] [ pb-contentxx-len ]  [ pb-contentxx ]
//
// pb-header 固定消息格式
// pb-content-xx 消息实体
//
// 消息头部可以扩展
// 消息实体可以扩展
//
// 类似 HTTP 协议
// request header  可以扩展
// request content 可以扩展
//
// 消息结构描述
//
// 1. [长度 int16] [protobuff-header ] [长度 int16] [protobuff-content]
// 2. 长度 int16 表示后面紧接着的protobuff-header / protobuff-content 长度
// 3. protobuff-header 里面包含一个 "msg_name" 字段表示protobuff-content 消息名称
// 3. protobuff-content 表示真实消息实体


class ProtoBuffMgr
{
public:
    ProtoBuffMgr();
    
    ~ProtoBuffMgr();
    
    void init_proto(std::string root_path);
    
    void write_message();
    
    void read_message();
    
protected:
    void write_int16(unsigned char* buff, int16 val);
    
    void read_int16(unsigned char* buff, int16& val);
private:
    Importer*    _import;
    DiskSourceTree*  _sourceTree;
    CompilerErrorCollector _error;
    std::string _cwd_path;
    std::string _root_path;
    std::string _file_name;
};



ProtoBuffMgr::ProtoBuffMgr()
{
    _sourceTree = new DiskSourceTree();
    _sourceTree->MapPath("", ".");
    _import = new Importer(_sourceTree, &_error);
    _file_name = "mf.msg";
    
}

ProtoBuffMgr::~ProtoBuffMgr()
{
    delete _sourceTree;

    delete _import;
    
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
    
    if (false) {
    
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
        
        field = descriptor->FindFieldByName("cmd");
        
        reflection->SetInt64(pair, field, 100011);
        
        field = descriptor->FindFieldByName("content");
        
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


void ProtoBuffMgr::read_int16(unsigned char *buff, int16 &val)
{
    int i = 0;
    val = 0;
    val = ((int16) buff[i++] << 8);
    val = val | ((int16)buff[i++]);
}

void ProtoBuffMgr::write_int16(unsigned char *buff, int16 val)
{
    int i = 0;
    buff[i++] = (val >> 8) & 0xff;
    buff[i++] = (val) & 0xff;
}

void ProtoBuffMgr::read_message()
{
    
    FILE* f = fopen(_file_name.c_str(), "rb");
    long fsize = 0;
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char* indata = new unsigned char[fsize + 1];
    fread(indata, fsize, 1, f);
    fclose(f);
    
    std::string msg_name;
    int i = 0;

    if (true) {
    
        const Descriptor *descriptor =   _import->pool()->FindMessageTypeByName("messageHeader");
        
        if (descriptor == NULL) {
            fprintf(stdout, "importer pool FindMessageTypeByName ERROR ! \n");
            
            return;
        }
        
        // build a dynamic message by "Pair" proto
        DynamicMessageFactory factory;
        
        const Message *message = factory.GetPrototype(descriptor);
        
        // create a real instance of "Pair"
        
        Message *pair = message->New();
        
        // write the "Pair" instance by reflection
        
        const Reflection *reflection = pair->GetReflection();

        int16 len = 0;
        read_int16(indata + i, len);
        
        i += 2;
        
        char ff[256] = {0};
        sprintf(ff, "%.*s", len, indata + i);
        i += len;
        
        pair->ParseFromString(ff);
        
        const FieldDescriptor *field = NULL;
        
        field = descriptor->FindFieldByName("cmd");
        
        int64 cmd = reflection->GetInt64(*pair, field);
        
        field = descriptor->FindFieldByName("msg_name");
        
        msg_name = reflection->GetString(*pair, field);
        
        fprintf(stdout, "recv message hread %lld %s \n", cmd, msg_name.c_str());
        
        delete pair;
    }
    
    
    if (true) {
    
        const Descriptor *descriptor =   _import->pool()->FindMessageTypeByName(msg_name);
        
        if (descriptor == NULL) {
            fprintf(stdout, "importer pool FindMessageTypeByName ERROR ! \n");
            
            return;
        }
        
        // build a dynamic message by "Pair" proto
        DynamicMessageFactory factory;
        
        const Message *message = factory.GetPrototype(descriptor);
        
        // create a real instance of "Pair"
        
        Message *pair = message->New();
        
        // write the "Pair" instance by reflection
        
        const Reflection *reflection = pair->GetReflection();

        
        int16 len = 0;
        read_int16(indata + i, len);
        
        i += 2;
        char ff[256] = {0};
        sprintf(ff, "%.*s", len, indata + i);
        
        i += len;
        
        pair->ParseFromString(ff);

        const FieldDescriptor *field = NULL;
        
        field = descriptor->FindFieldByName("cmd");
        
        int64 cmd = reflection->GetInt64(*pair, field);
        
        field = descriptor->FindFieldByName("content");
        
        std::string content = reflection->GetString(*pair, field);
        
        fprintf(stdout, "recv message hread %s %lld %s \n", msg_name.c_str(), cmd, content.c_str());
        
        delete  pair;
    }
    
    fprintf(stdout, " read message end %d %d\n", i, (int)fsize);
    
    delete [] indata;
    
}

void ProtoBuffMgr::write_message()
{

    const Descriptor *descriptor =   _import->pool()->FindMessageTypeByName("messageHeader");
    
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
    
    field = descriptor->FindFieldByName("cmd");
    
    reflection->SetInt64(pair, field, 10001);
    
    field = descriptor->FindFieldByName("msg_name");
    
    reflection->SetString(pair, field, "message1");
    
    string encode_string = pair->SerializeAsString();
    
    if (true) {
        int16 len = (int16)encode_string.length();
        unsigned char buff[25] = {0};
        write_int16(buff, len);
        FILE* f = fopen(_file_name.c_str(), "wb");
        fwrite(buff, 2, 1, f);
        fwrite(encode_string.c_str(), encode_string.length(), 1, f);
        fclose(f);
    }
    
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
        
        field = descriptor->FindFieldByName("cmd");
        
        reflection->SetInt64(pair, field, 10002);
        
        field = descriptor->FindFieldByName("content");
        
        reflection->SetString(pair, field, "=============11122222====|||3333");
        
        string encode_string = pair->SerializeAsString();

        int16 len = (int16)encode_string.length();
        unsigned char buff[25] = {0};
        write_int16(buff, len);
        FILE* f = fopen(_file_name.c_str(), "ab");
        fwrite(buff, 2, 1, f);
        fwrite(encode_string.c_str(), encode_string.length(), 1, f);
        fclose(f);
    }
    
    
    delete pair;
}



void test_protpmgr_proto()
{
    ProtoBuffMgr _mgr;
    const char* pwd = "/Users/zhangzhifan/work/git-code/mySample/google-protobuff/mypb/pb/res";
    _mgr.init_proto(pwd);
    
    _mgr.write_message();
    _mgr.read_message();
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    //test_dynamic_complie_proto();
    test_protpmgr_proto();

    return 0;
}
