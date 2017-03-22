#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>

// g++ test_get_file_line.cc



void get_file_line1(const char* file)
{
    std::fstream fs(file);

    std::string line;
    while (std::getline(fs, line)) {
        std::istringstream iss(line);

        int a,b;
        if (!(iss >> a >> b)) {
            std::istringstream iss(line);
            char buf[256];
            while (iss.getline(buf, sizeof(buf), ',')) {
                std::cout << "iss :" << buf << std::endl;
            }
            break;
        }

        std::cout << "line :" << line << " a :" << a << " b:" << b << std::endl;
    }
}


std::string string_ltrim(std::string input)
{
    std::string ret = input;

    size_t pos = ret.find_first_not_of(" \t");

    if (pos != std::string::npos) {
        ret = ret.substr(pos);
    }
    return ret;
}

std::string string_rtrim(std::string input)
{
    std::string ret = input;
    size_t pos = ret.find_last_not_of(" \t");
    if (pos != std::string::npos) {
        ret = ret.substr(0, pos+1);
    }
    return ret;
}

std::string string_trim(std::string input)
{

    return string_ltrim(string_rtrim(input));
}


void read_config_file(const char* file)
{

    std::fstream fs(file);
    std::string line;

    while (std::getline(fs, line)) {

        if (line.empty()) {
            continue;
        }

        if (strncmp(line.c_str(), "#", 1) == 0) {
            // this is comments
            std::cout << "this is comments line : " << line << std::endl;
            continue;
        }

        // process line
        size_t pos = line.find(":");
        if (pos != std::string::npos) {
            if (pos == 1) {
                std::cout << " error \":\" locate first char " << line << std::endl;
                continue;
            }

            std::string k = line.substr(0, pos - 1);
            std::string v = line.substr(pos + 1);

            k = string_trim(k);
            v = string_trim(v);
            std::cout << " line key [" << k << "]"<< " value [" << v << "]"<< std::endl;
        } else {

            std::cout << "get file line not find : " << std::endl;
        }
    }
}





int main()
{

    get_file_line1("test_file.dat");

    read_config_file("test_config.cfg");
    return 0;
}
