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
    
    void read_int16(const unsigned char* buff, int16& val);
    
    int encode_message(const Message* pb_msg, std::string& result);
    
    int decode_message(const std::string & en_msg, Message** pb_msg);
    
    void dump_pb_message(const google::protobuf::Message* pb_mssage);
    
    std::string hex_encode(const std::string& input);

private:
    Importer*    _import;
    DiskSourceTree*  _sourceTree;
    CompilerErrorCollector _error;
    DynamicMessageFactory* _factory;
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
    _factory = new DynamicMessageFactory(_import->pool());
    
}

ProtoBuffMgr::~ProtoBuffMgr()
{
    delete _factory;

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
    
        const Descriptor *descriptor =  _import->pool()->FindMessageTypeByName("message1");
        
        if (descriptor == NULL) {
            fprintf(stdout, "importer pool FindMessageTypeByName ERROR ! \n");
            
            return;
        }
        
        cout << "descriptor debug string "<< descriptor->DebugString() << endl;
        
       
        
        const Message *message = _factory->GetPrototype(descriptor);
        
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


void ProtoBuffMgr::read_int16(const unsigned char *buff, int16 &val)
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

int ProtoBuffMgr::encode_message(const google::protobuf::Message *pb_msg, std::string& result)
{
    
    int succ = -1;

    result.clear();
    const Descriptor *descriptor =   _import->pool()->FindMessageTypeByName("messageHeader");
    
    if (descriptor == NULL) {
        fprintf(stdout, "importer pool FindMessageTypeByName ERROR ! \n");
        succ = -1;
        return succ;
    }
    
    cout << "descriptor debug string "<< descriptor->DebugString() << endl;
    
    const Message *message = _factory->GetPrototype(descriptor);
    
    // create a real instance of "Pair"
    
    Message *pair = message->New();
    
    // write the "Pair" instance by reflection
    
    const Reflection *reflection = pair->GetReflection();
    
    const FieldDescriptor *field = NULL;
    
    field = descriptor->FindFieldByName("cmd");
    
    reflection->SetInt64(pair, field, 10001);
    
    field = descriptor->FindFieldByName("msg_name");
    
    reflection->SetString(pair, field, pb_msg->GetTypeName().c_str());
    
    string s = pair->SerializeAsString();

    int16 len = (int16)s.length();
    
    unsigned char len_buf[25] = {0};
    write_int16(len_buf, len);

    result.append((char*)len_buf, 2);
    result.append(s);
    
    delete pair;
    
    if (true) {
        s = pb_msg->SerializeAsString();
        len = (int16)s.length();
        write_int16(len_buf, len);
        result.append((char*)len_buf, 2);
        result.append(s);
    }
    
   
    return succ;
}


int ProtoBuffMgr::decode_message(const std::string &en_msg, google::protobuf::Message **pb_msg)
{
    int succ = -1;
    std::string msg_name;
    int16 header_len = 0;
    if (true) {
    
        const Descriptor *descriptor =   _import->pool()->FindMessageTypeByName("messageHeader");
        
        if (descriptor == NULL) {
            fprintf(stdout, "importer pool FindMessageTypeByName ERROR ! \n");
            succ = -1;
            return succ;
        }
        
        cout << "descriptor debug string "<< descriptor->DebugString() << endl;
        
        const Message *message = _factory->GetPrototype(descriptor);
        
        // create a real instance of "Pair"
        
        Message *pair = message->New();
        
        
        read_int16((const unsigned char*)en_msg.c_str(), header_len);
        
        
        pair->ParseFromArray(en_msg.c_str() + 2, header_len);
        
        // write the "Pair" instance by reflection
        
        const Reflection *ref = pair->GetReflection();
        
        const FieldDescriptor *field = NULL;
        
        field = descriptor->FindFieldByName("cmd");
        
        int64 cmd = ref->GetInt64(*pair, field);
        
        field = descriptor->FindFieldByName("msg_name");
        
        msg_name = ref->GetString(*pair, field);
        
        delete pair;
        pair = NULL;
    }
    
    
    // message body
    if (true) {
        const Descriptor *desc =   _import->pool()->FindMessageTypeByName(msg_name);
        
        if (desc == NULL) {
            fprintf(stdout, "importer pool FindMessageTypeByName ERROR ! \n");
            succ = -1;
            return succ;
        }
        
        const Message *msg = _factory->GetPrototype(desc);
        
        // create a real instance of "Pair"
        
        Message *p = msg->New();

        int16 body_len = 0;
        
        read_int16((const unsigned char*)en_msg.c_str() + 2 + header_len, body_len);
        
        p->ParseFromArray(en_msg.c_str() + 2 + header_len + 2, body_len);
        
        *pb_msg = p;
        
        
        if (true) {
            // test message body
            Message* m = p;
            const Reflection *ref = m->GetReflection();
            const FieldDescriptor *field = NULL;
            const Descriptor *desc = m->GetDescriptor();
            
            field = desc->FindFieldByName("cmd");
            
            int64 cmd = ref->GetInt64(*m, field);
            
            field = desc->FindFieldByName("content");
            
            std::string content = ref->GetString(*m, field);
            
            fprintf(stdout, "recv %lld %s \n", cmd, content.c_str());
        }
        

    }
    
    return succ;
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
    
    if (true) {
    
        google::protobuf::Message* pb_msg = NULL;
        std::string en_msg;
        en_msg.append((char*)indata, fsize);
        decode_message(en_msg, &pb_msg);
        
        if (pb_msg == NULL) {
            fprintf(stdout, "decode message error !! \n");
            return;
        }
        
        Message* m = pb_msg;
        const Reflection *ref = m->GetReflection();
        const FieldDescriptor *field = NULL;
        const Descriptor *desc = m->GetDescriptor();
        
        field = desc->FindFieldByName("cmd");
        
        int64 cmd = ref->GetInt64(*m, field);
        
        field = desc->FindFieldByName("content");
        
        std::string content = ref->GetString(*m, field);
        
        fprintf(stdout, "recv %lld %s \n", cmd, content.c_str());
        
        dump_pb_message(pb_msg);
        delete pb_msg;
    }
    
}

void ProtoBuffMgr::write_message()
{
    
    if (true) {
        const Descriptor *descriptor =   _import->pool()->FindMessageTypeByName("message1");
        if (descriptor == NULL) {
            fprintf(stdout, "importer pool FindMessageTypeByName ERROR ! \n");
            return;
        }
        
        cout << "descriptor debug string "<< descriptor->DebugString() << endl;
        
        const Message *message = _factory->GetPrototype(descriptor);
        
        // create a real instance of "Pair"
        
        Message *pair = message->New();
        
        // write the "Pair" instance by reflection
        
        const Reflection *reflection = pair->GetReflection();
        
        const FieldDescriptor *field = NULL;
        
        field = descriptor->FindFieldByName("cmd");
        
        reflection->SetInt64(pair, field, 10002);
        
        field = descriptor->FindFieldByName("content");
        
        reflection->SetString(pair, field, "=============11122222====|||3333");

        std::string result;
        encode_message(pair, result);
    
        fprintf(stdout, "encode message len %zu \n", result.length());
        
        FILE* f = fopen(_file_name.c_str(), "wb");
        
        fwrite(result.c_str(), result.length(), 1, f);
        fclose(f);
        
        delete pair;
    }
    
}

//using std::string;
std::string ProtoBuffMgr::hex_encode(const std::string& input)
{
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();
    
    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

void ProtoBuffMgr::dump_pb_message(const google::protobuf::Message *msg)
{

    const google::protobuf::Descriptor *d = msg->GetDescriptor();
    if(!d) {
        fprintf(stderr, "dump pb message error getDescriptor ! \n");
        return;
    }
    
    size_t count = d->field_count();
    
    for (size_t i = 0; i != count ; ++i)
    {
        const google::protobuf::FieldDescriptor *field = d->field(i);
        if(!field) {
            fprintf(stderr, "dump pb message error field ! \n");
            return ;
        }
        
        const google::protobuf::Reflection *ref = msg->GetReflection();
        if(!ref) {
            fprintf(stderr, "dump pb message error GetReflection ! \n");
            return ;
        }
        const char *name = field->name().c_str();
        if(!field->is_repeated() && ref->HasField(*msg,field)) {
            double value1;
            float value2;
            int64_t value3;
            uint64_t value4;
            int32_t value5;
            uint32_t value6;
            bool value7;
            std::string value8;
            const google::protobuf::Message *value9;
            const google::protobuf::EnumValueDescriptor *value10;
            
            switch (field->cpp_type())
            {
                case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE:
                    value1 = ref->GetDouble(*msg,field);
                    fprintf(stdout, "got dubble [%s %.2f] \n", name, value1);
                    break;
                case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT:
                    value2 = ref->GetFloat(*msg,field);
                    fprintf(stdout, "got float [%s %.2f] \n", name, value2);
                    break;
                case google::protobuf::FieldDescriptor::CPPTYPE_INT64:
                    value3 = ref->GetInt64(*msg,field);
                    fprintf(stdout, "got int64 [%s %lld] \n", name, value3);
                    break;
                case google::protobuf::FieldDescriptor::CPPTYPE_UINT64:
                    value4 = ref->GetUInt64(*msg,field);
                    fprintf(stdout, "got uint64 [%s %llu] \n", name, value4);
                    break;
                case google::protobuf::FieldDescriptor::CPPTYPE_INT32:
                    value5 = ref->GetInt32(*msg,field);
                    fprintf(stdout, "got int32 [%s %d] \n", name, value5);
                    break;
                case google::protobuf::FieldDescriptor::CPPTYPE_UINT32:
                    value6 = ref->GetUInt32(*msg,field);
                    fprintf(stdout, "got uin32 [%s %u] \n", name, value6);
                    break;
                case google::protobuf::FieldDescriptor::CPPTYPE_BOOL:
                    value7 = ref->GetBool(*msg,field);
                    fprintf(stdout, "got bool [%s %d] \n", name, value7);
                    break;
                case google::protobuf::FieldDescriptor::CPPTYPE_STRING:
                    if (field->type() == google::protobuf::FieldDescriptor::TYPE_BYTES) {
                        value8 = hex_encode(ref->GetString(*msg,field));
                    } else {
                        value8 = ref->GetString(*msg,field);
                    }
                    fprintf(stdout, "got string [%s %s] \n", name, value8.c_str());
                    break;
                case google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE:
                    value9 = &(ref->GetMessage(*msg,field));
                    if (value9) {
                        fprintf(stdout, "got message [%s %s] \n", name, value9->GetTypeName().c_str());
                        dump_pb_message(value9);
                    }
                    break;
                case google::protobuf::FieldDescriptor::CPPTYPE_ENUM:
                    value10 = ref->GetEnum(*msg,field);
                    fprintf(stdout, "got enum [%s %d] \n", name, value10);
                    break;
                default:
                    break;
        
            }
        }
    }
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
