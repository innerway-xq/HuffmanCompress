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
extern std::vector<std::string> empty_folders;

bool isFile(const char* path);
int GetFileSize(const char* file_path);
// GetRootPath - 得到根目录
std::string GetRootPath(std::string &src);
// GetFolderFiles - 得到root下所有文件，存在files里
void GetFolderFiles(std::string root);
// xqz_read_src - 读文件，并计数得到word_freq
void xqz_read_src(const char *filename);
// cnt_freq - 对x中字符计数
void cnt_freq(uc *x, int l);
// compressed_length - 压缩文件长度
ull compressed_length();
// xqz_write_dest - 写入dest文件
void xqz_write_dest(const char *srcfilename,const char *relative_addr, const char *destfilename);

#endif