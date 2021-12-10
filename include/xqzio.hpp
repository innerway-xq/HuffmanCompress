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
int GetFolderSize(const char* folder_path);
// GetRootPath - 得到根目录
std::string GetRootPath(std::string src);
// GetFolderFiles - 得到root下所有文件，存在files里
void GetFolderFiles(std::string root);
// xqz_read_src_compress - 读文件，并计数得到word_freq
void xqz_read_src_compress(const char *filename);
// cnt_freq - 对x中字符计数
void cnt_freq(uc *x, int l);
// compressed_length - 压缩文件长度(按bit)
ull compressed_length();
// Code2WordinFile - 将code2word写入压缩文件
void Code2WordinFile(std::ofstream &fout);
// xqz_write_dest_compress - 读入src文件，压缩后，写入dest文件
void xqz_write_dest_compress(const char *srcfilename,const char *relative_addr, const char *destfilename);
// read_code2word - 从压缩文件读取code2word
void read_code2word(std::ifstream &fin);
// xqz_write_dest_decompress - 读入fin流，解压后，写入dest文件 
void xqz_write_dest_decompress(std::ifstream &fin, const char* dest, ull src_l);
// dest_exist - 检查目标文件是否存在
bool dest_exist(std::string dest);
// my_mkdir - 创建多级目录
void my_mkdir(std::string path);
// update_bar - 更新进度条
void update_bar(int i, int max_i);
#endif