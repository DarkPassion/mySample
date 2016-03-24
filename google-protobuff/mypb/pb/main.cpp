//
//  main.cpp
//  pb
//
//  Created by zhangzhifan on 16/3/23.
//  Copyright © 2016年 zhangzhifan. All rights reserved.
//

#include <iostream>
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"
#include "google/protobuf/dynamic_message.h"
#include "google/protobuf/compiler/importer.h"


using namespace google::protobuf;
using namespace google::protobuf::compiler;
using namespace std;


void test_dynamic_complie_proto();



void test_dynamic_complie_proto()
{
    DiskSourceTree sourceTree;
    
    //look up .proto file in current directory
    
    sourceTree.MapPath("","/Users/zhangzhifan/work/git-code/mybaobao/protobuff/mpb/pb/pb/res/");
    
    Importer importer(&sourceTree, NULL);
    
    //runtime compile foo.proto
    
    importer.Import("message.proto");
    
    const Descriptor *descriptor =    importer.pool()->
    
    FindMessageTypeByName("LogonReqMessage");
    
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


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    test_dynamic_complie_proto();

    return 0;
}
