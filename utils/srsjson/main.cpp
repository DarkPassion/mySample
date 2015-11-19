#include <stdio.h>
#include <stdlib.h>
#include <srs_app_json.hpp>
#include <iostream>
#include <string>
#include <assert.h>
#include <sstream>

using namespace std;

int main()
{

	const char* ptr = "zhangzhifan";
	cout << SRS_JFIELD_STR("ss", ptr) << endl;
    cout << SRS_JOBJECT_START << SRS_JFIELD_STR("start_index", "1") << SRS_JFIELD_CONT << SRS_JFIELD_STR("current_index", "20") << SRS_JFIELD_CONT
        << SRS_JFIELD_NAME("data") << SRS_JARRAY_START 
        << SRS_JOBJECT_START << SRS_JFIELD_STR("1", "dir1") << SRS_JOBJECT_END << SRS_JFIELD_CONT
        << SRS_JOBJECT_START << SRS_JFIELD_STR("2", "dir2") << SRS_JOBJECT_END 
        << SRS_JARRAY_END
        << SRS_JOBJECT_END << endl;


    std::stringstream ss;
    ss << SRS_JOBJECT_START << SRS_JFIELD_STR("name", "srs") << SRS_JFIELD_CONT
            << SRS_JFIELD_STR("rtmp", "released") << SRS_JFIELD_CONT
            << SRS_JFIELD_STR("hls", "released")
            << SRS_JOBJECT_END;
    std::string buffer = ss.str();

    printf("json -- %s\n", buffer.c_str());

    SrsJsonAny* pany = NULL;
   
    pany = SrsJsonAny::loads((char*)buffer.c_str());

    assert(pany);

    if (pany->is_object()) {
        printf("v  = %s\n", "pany is jsonObject!");

        SrsJsonObject* obj = pany->to_object();
        int count = obj->count();
        printf("jsonObject count = %d \n", count);

        SrsJsonAny* name = NULL;
        name = obj->ensure_property_string("name");
        if (name  == NULL) {
            printf("jsonObject no name property!\n");
        } else {
            string val = obj->ensure_property_string("name")->to_str();
            printf("jsonObject name property value %s \n", val.c_str());
        }

        for (int i = 0; i < count; i++) {
            string key = obj->key_at(i);
            string val = obj->value_at(i)->to_str();
            printf(" i  %d k : %s v: %s\n", i, key.c_str(), val.c_str());
        }
    }

    if (pany->is_string()) {
        printf("pany is json string type!\n");
    }


    if (pany) {
        delete pany;
        pany = NULL;
    }
    return 0;
}
