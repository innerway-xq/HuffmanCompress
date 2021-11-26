#ifndef XQZIO_H
#define XQZIO_H

#include <map>
#include <string>
#include <vector>

#define ull unsigned long long
#define uc unsigned char
typedef std::map<uc, std::string> Word2Code_T;
typedef std::map<std::string, uc> Code2Word_T;

extern ull word_freq[256];
extern const int MAX_IO_N;
extern std::vector<std::string> files;

bool isFile(const char* path);
int GetFileSize(const char* file_path);
std::string GetRootPath(std::string &src);
void GetFolderFiles(std::string root);
void xqz_read_src(const char *filename);
void cnt_freq(uc *x, int l);
ull cnt_code_bytes();
void xqz_write_dest(const char *srcfilename,const char *relative_addr, const char *destfilename);

#endif