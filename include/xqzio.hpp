#ifndef XQZIO_H
#define XQZIO_H

#include <map>
#include <string>

#define ull unsigned long long
#define uc unsigned char
typedef std::map<uc, std::string> Word2Code_T;
typedef std::map<std::string, uc> Code2Word_T;

extern ull word_freq[256];
extern const int MAX_IO_N;

void GetRootPath(char* root, char* src);
void xqz_read_src(const char *filename);
void cnt_freq(uc *x, int l);
// ull cnt_code_bytes();
void xqz_write_dest(const char *srcfilename, const char *destfilename, char *relative_addr);

#endif