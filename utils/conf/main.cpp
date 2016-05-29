
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

typedef std::vector<std::string> Params;


void parse_conf(const char* filename);

int parse_len(char* line, int len, char* key, char* val);
int parse_app_name(char* line, int len, char* app);
// gcc main.cpp -lstdc++


void parse_conf(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (f == NULL) {
		printf("fopen error !\n");
		return;
	}

	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* buff = new char[size];
	fread(buff, size, 1, f);
	fclose(f);

	//printf("read conf %s\n", buff);
	


#if 1

	if (true) {

		int i = 0;
		while (i < size) {

			

			char line_buff[1024] = {0};
			char line_key[1024] = {0};
			char line_val[1024] = {0};
			char* line_start = buff + i;
			char* line_end = buff + i;
			int line_len = 0;
			while(*line_end != '\n') line_end++;
			line_len = int(line_end - line_start + 1);
			memcpy(line_buff, line_start, line_len);
			// printf("line_buff %s %d\n", line_buff, line_len);
			i += line_len;
			if (line_len > 1) {
				char app_name[1024] = {0};
				int ret = parse_app_name(line_start, line_len, app_name);
				if (ret == 0) {
					printf("===== app_name %s\n", app_name);
				}
				ret = parse_len(line_start, line_len, line_key, line_val);
				if (ret == 0) {
					//printf(" %s %s\n", line_key, line_val);
				}
			}
		}
	}

	delete[] buff;

#endif

}

int parse_len(char* line, int len, char* key, char* val)
{
	char* buff = new char[len];
	memset(buff, 0, len);
	memcpy(buff, line, len);
	char* k = NULL;
	char* v = NULL;
	int klen = 0;
	int vlen = 0;

	char* tmp = NULL;
	for (int i = 0; i < strlen(buff); ++i)
	{
		/* code */
		if (buff[i] != ' ' && buff[i] != '\t') {
			k = buff + i;
			break;
		}
	}


	if (k == NULL) {
		return -1;
	}

	tmp = strchr(k, ' ');
	if (tmp == NULL) {
		tmp = strchr(k, '\t');
		if (tmp == NULL) {
			return -1;
		}
	} 

	klen = tmp - k;

	for (int i = 0; i < strlen(tmp); ++i)
	{
		/* code */
		if (tmp[i] != ' ' && tmp[i] != '\t') {
			v = tmp + i;
			break;
		}
	}

	if (v == NULL) {
		return -1;
	}

	tmp = strchr(v, '\n');
	if (tmp == NULL) {
		return -1;
	}

	vlen = tmp - v;
	
	memcpy(key, k, klen);
	memcpy(val, v, vlen);

	printf("parse_len = %s %s  %d %d\n", key, val, klen, vlen);
	delete[] buff;
	return 0;
}


int parse_app_name(char* line, int len, char* app)
{

	char* buff = new char[len];
	memcpy(buff, line, len);

	if (strchr(buff, '{') == 0) {
		return -1;
	}

	char* a = NULL;
	int alen = 0;
	char* tmp = NULL;
	for (int i = 0; i < len; ++i)
	{
		/* code */
		if (buff[i] != ' ' && buff[i] != '\t') {
			a = buff + i;
			break;
		}
	}

	if (a == NULL) {
		return -1;
	}
	
	tmp = strchr(a, ' ');
	if (tmp == NULL) {
		tmp = strchr(a, '\t');
		if (tmp == NULL) {
			return -1;
		}
	}

	alen = tmp - a;


	memcpy(app, a, alen);
	delete[] buff;
	return 0;
}

int main()
{



	parse_conf("zzf.conf");
	return 0;
}

