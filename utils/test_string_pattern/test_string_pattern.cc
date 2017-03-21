#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// g++ test_string_pattern.cc -o test_string_pattern.out

#define DEBUG_EQ(a, b, s) if (a != b) { printf("%s \n", s); }

int stringmatchlen(const char *pattern, int patternLen, const char *string, int stringLen, int nocase);
int stringmatch(const char *pattern, const char *string, int nocase);



int stringmatchlen(const char *pattern, int patternLen, const char *string, int stringLen, int nocase) {
  while (patternLen) {
    switch (pattern[0]) {
      case '*':
        while (pattern[1] == '*') {
          pattern++;
          patternLen--;
        }
        if (patternLen == 1)
          return 1; /* match */
        while (stringLen) {
          if (stringmatchlen(pattern+1, patternLen-1, string, stringLen, nocase))
            return 1; /* match */
          string++;
          stringLen--;
        }
        return 0; /* no match */
        break;
      case '?':
        if (stringLen == 0)
          return 0; /* no match */
        string++;
        stringLen--;
        break;
      case '[':
        {
          int no, match;

          pattern++;
          patternLen--;
          no = pattern[0] == '^';
          if (no) {
            pattern++;
            patternLen--;
          }
          match = 0;
          while (1) {
            if (pattern[0] == '\\') {
              pattern++;
              patternLen--;
              if (pattern[0] == string[0])
                match = 1;
            } else if (pattern[0] == ']') {
              break;
            } else if (patternLen == 0) {
              pattern--;
              patternLen++;
              break;
            } else if (pattern[1] == '-' && patternLen >= 3) {
              int start = pattern[0];
              int end = pattern[2];
              int c = string[0];
              if (start > end) {
                int t = start;
                start = end;
                end = t;
              }
              if (nocase) {
                start = tolower(start);
                end = tolower(end);
                c = tolower(c);
              }
              pattern += 2;
              patternLen -= 2;
              if (c >= start && c <= end)
                match = 1;
            } else {
              if (!nocase) {
                if (pattern[0] == string[0])
                  match = 1;
              } else {
                if (tolower((int)pattern[0]) == tolower((int)string[0]))
                  match = 1;
              }
            }
            pattern++;
            patternLen--;
          }
          if (no)
            match = !match;
          if (!match)
            return 0; /* no match */
          string++;
          stringLen--;
          break;
        }
      case '\\':
        if (patternLen >= 2) {
          pattern++;
          patternLen--;
        }
        /* fall through */
      default:
        if (!nocase) {
          if (pattern[0] != string[0])
            return 0; /* no match */
        } else {
          if (tolower((int)pattern[0]) != tolower((int)string[0]))
            return 0; /* no match */
        }
        string++;
        stringLen--;
        break;
    }
    pattern++;
    patternLen--;
    if (stringLen == 0) {
      while (*pattern == '*') {
        pattern++;
        patternLen--;
      }
      break;
    }
  }
  if (patternLen == 0 && stringLen == 0)
    return 1;
  return 0;
}

int stringmatch(const char *pattern, const char *string, int nocase) {
  return stringmatchlen(pattern,strlen(pattern),string,strlen(string),nocase);
}



int main()
{

    DEBUG_EQ(1, stringmatch("www_baidu_com", "www_baidu_com", 0), "debug error case 1")
    DEBUG_EQ(1, stringmatch("ww_*", "ww_baidu_com_error", 0), "debug error case 2")
    DEBUG_EQ(1, stringmatch("zhihu.*", "zhihu.antispam.log", 0), "debug error case 3")
    DEBUG_EQ(1, stringmatch("*.request", "zhihu.request", 0), "debug error case 4")
    DEBUG_EQ(0, stringmatch("*.request", "zhihu.response", 0), "debug error case 5")
    DEBUG_EQ(1, stringmatch("zhihu.*.access", "zhihu.nginx.access", 0), "debug error case 6")
    DEBUG_EQ(1, stringmatch("zhihu.*.access", "zhihu.tornado.noti.access", 0), "debug error case 7")
    DEBUG_EQ(0, stringmatch("zhihu.*.access", "zhihu.tornado.noti", 0), "debug error case 8")


    DEBUG_EQ(1, stringmatch("www_*", "WWW_ZHIHU_COM_ERROR", 1), "debug error case 9")
    DEBUG_EQ(1, stringmatch("ZHIHU.*", "ZHIHU.antispam.log", 0), "debug error case 10")
    DEBUG_EQ(1, stringmatch("ZHIHU.*", "ZHIHU.REQUEST", 1), "debug error case 11")


    printf(" DEBUG_EQ test Done! \n");
    return 0;
}
