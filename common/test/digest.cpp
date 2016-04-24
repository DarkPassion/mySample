#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcmp memcpy
#include <string>
#include <assert.h>

#include "digest/crc32.h"
#include "digest/md5.h"
#include "string/stringencode.h"


// g++ digest.cpp -I../src/ -L../libs/ -lcommon -o digest.out



using namespace rtc;

void test_crc32_func();

void test_md5_func();


void test_crc32_func()
{
    int crc32 = ComputeCrc32("");
    assert(crc32 == 0);
    crc32 = ComputeCrc32("abc");
    assert(crc32 == 0x352441C2U);
    crc32 = ComputeCrc32("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
    assert(crc32 == 0x171A3F5FU);
    
    std::string input =
    "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    uint32 c = 0;
    for (size_t i = 0; i < input.size(); ++i) {
        c = UpdateCrc32(c, &input[i], 1);
    }
}



// A simple wrapper for our MD5 implementation.
class Md5Digest {
public:
    enum { kSize = 16 };
    
    Md5Digest() {
        MD5Init(&ctx_);
    }
    
    size_t Size() const {
        return kSize;
    }
    
    void Update(const void* buf, size_t len) {
        MD5Update(&ctx_, static_cast<const uint8*>(buf), len);
    }
    
    size_t Finish(void* buf, size_t len) {
        if (len < kSize) {
            return 0;
        }
        MD5Final(&ctx_, static_cast<uint8*>(buf));
        std::string hexstr = hex_encode(static_cast<const char*>(buf), kSize);
        memcpy(buf, hexstr.data(), hexstr.length());
        MD5Init(&ctx_);  // Reset for next use.
        return hexstr.length();
    }
private:
    MD5_CTX ctx_;
};

void test_md5_func()
{
    
    Md5Digest md5;
    char md5buff[64] = {0};
    const char* md5_0 = "d41d8cd98f00b204e9800998ecf8427e";
    md5.Update("", 0);
    md5.Finish(md5buff, sizeof(md5buff));
    //std::string hexstr = hex_encode(md5buff, md5.Size());
    printf("[%s] \n[%s] \n", md5_0, md5buff);
    
    if (memcmp(md5_0, md5buff, strlen(md5buff)) == 0)
    {
        printf("md5 test SUCC!\n");
    }
    
    
    const char* md5_1 = "0cc175b9c0f1b6a831c399e269772661";
    const char* input = "a";
    memset(md5buff, 0, sizeof(md5buff));
    md5.Update(input, strlen(input));
    md5.Finish(md5buff, sizeof(md5buff));
    
    printf("[%s] \n[%s] \n", md5_1, md5buff);
    if (memcmp(md5_1, md5buff, strlen(md5buff)) == 0) {
        printf("md5 test SUCC!\n");
    }

}

int main()
{

    test_md5_func();
    test_crc32_func();
    

    return 0;
}


