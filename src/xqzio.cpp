#include "xqzio.hpp"
#include "HuffmanTree.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/stat.h>

using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::ofstream;
using std::string;

ull word_freq[256] = {};
const int MAX_IO_N = 0x1000;
std::vector<string> files;

bool isFile(const char *path)
{
    struct stat buf;
    stat(path, &buf);
    if (_S_IFDIR & buf.st_mode)
    {
        cout << "folder" << endl;
        return false;
    }
    else if (_S_IFREG & buf.st_mode)
    {
        cout << "file" << endl;
        return true;
    }
}
int GetFileSize(const char *file_path)
{
    struct stat buf;
    stat(file_path, &buf);
    return buf.st_size;
}

string GetRootPath(string &src)
{
    int target_l = src.size();
    string ret;
    for (int i = target_l - 1; i >= 0; --i)
    {
        if (src[i] == '/')
        {
            ret.assign(src).erase(i);
            return ret;
        }
    }
}

void GetFolderFiles(string root)
{
    long file_handle;
    struct _finddata_t fileinfo;
    string tmp;
    if ((file_handle = _findfirst(tmp.assign(root).append("/*").c_str(), &fileinfo)))
    {
        do
        {
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if ((strcmp(fileinfo.name, ".")) && (strcmp(fileinfo.name, "..")))
                    GetFolderFiles(tmp.assign(root).append("/").append(fileinfo.name));
            }
            else
            {
                files.push_back(tmp.assign(root).append("/").append(fileinfo.name));
            }
        } while (!(_findnext(file_handle, &fileinfo)));
        _findclose(file_handle);
    }
}

void xqz_read_src(const char *filename)
{
    uc buf[MAX_IO_N] = {};
    ifstream fin;
    fin.open(filename, ios::in | ios::binary);
    if (!fin)
    {
        cout << "open file failed" << endl;
        return;
    }
    fin.seekg(0, ios::end);
    ull length = fin.tellg();
    ull max_i = length / MAX_IO_N;
    fin.seekg(0, ios::beg);
    for (register ull i = 1; i <= max_i; ++i)
    {
        fin.read((char *)buf, MAX_IO_N);
        cnt_freq(buf, MAX_IO_N);
    }
    fin.read((char *)buf, length % MAX_IO_N);
    cnt_freq(buf, length % MAX_IO_N);
    fin.close();
    cout << filename << " input finished" << endl;
}
void cnt_freq(uc *x, int l)
{
    for (register int i = 0; i < l; ++i)
    {
        ++word_freq[x[i]];
    }
}
ull cnt_code_bytes()
{
    ull ret;
    for (int i = 0; i < 256; ++i)
    {
        ret += word_freq[i] * word2code[(uc)i].size();
    }
    return ret;
}
void xqz_write_dest(const char *srcfilename, const char *relative_addr, const char *destfilename)
{
    uc buf_out[MAX_IO_N] = {};
    ofstream fout;
    fout.open(destfilename, ios::app | ios::binary);
    if (!fout)
    {
        cout << "open destfile failed" << endl;
        return;
    }
    fout.write(relative_addr, strlen(relative_addr));
    fout.write("\n", 1);
    char dest_length[25] = {};
    sprintf(dest_length,"%llu", cnt_code_bytes());
    fout.write(dest_length, strlen(dest_length));
    fout.write("\n", 1);
    Code2WordinFile(fout);

    uc buf_in[MAX_IO_N] = {};
    ifstream fin;
    fin.open(srcfilename, ios::in | ios::binary);
    if (!fin)
    {
        cout << "open srcfile failed" << endl;
        return;
    }

    int out_l = 0;
    uc buf_char = '\x00';
    int buf_bit = 7;

    fin.seekg(0, ios::end);
    ull length = fin.tellg();
    ull max_i = length / MAX_IO_N;
    fin.seekg(0, ios::beg);

    for (register ull i = 1; i <= max_i; ++i)
    {
        fin.read((char *)buf_in, MAX_IO_N);
        for (register int j = 0; j < MAX_IO_N; ++j)
        {
            string temp = word2code[buf_in[j]];
            for (register int k = 0; k < temp.size(); ++k)
            {
                if (buf_bit >> 31)
                {
                    if (out_l == MAX_IO_N)
                    {
                        fout.write((const char *)buf_out, out_l);
                        memset(buf_out, 0, sizeof(buf_out));
                        out_l &= 0;
                    }
                    else
                    {
                        buf_out[out_l++] = buf_char;
                        buf_char <<= 8;
                        buf_bit = 7;
                    }
                }
                buf_char |= temp[k] << (buf_bit--);
            }
        }
    }

    fin.read((char *)buf_in, length % MAX_IO_N);
    for (register int j = 0; j < length % MAX_IO_N; ++j)
    {
        string temp = word2code[buf_in[j]];
        for (register int k = 0; k < temp.size(); ++k)
        {
            if (buf_bit >> 31)
            {
                if (out_l == MAX_IO_N)
                {
                    fout.write((const char *)buf_out, out_l);
                    memset(buf_out, 0, sizeof(buf_out));
                    out_l &= 0;
                }
                else
                {
                    buf_out[out_l++] = buf_char;
                    buf_char <<= 8;
                    buf_bit = 7;
                }
            }
            buf_char |= temp[k] << (buf_bit--);
        }
    }

    if (buf_char)
    {
        if (out_l >= MAX_IO_N)
        {
            fout.write((const char *)buf_out, out_l);
            memset(buf_out, 0, sizeof(buf_out));
            out_l &= 0;
        }
        else
        {
            buf_out[out_l++] = buf_char;
            buf_char <<= 8;
            //这里的buf_bit即为压缩文件末尾多的零，要记下来
        }
    }
    if (out_l)
    {
        fout.write((const char *)buf_out, out_l);
        memset(buf_out, 0, sizeof(buf_out));
        out_l = 0;
    }

    fout.write("\n", 1);
    fin.close();
    fout.close();
    cout << srcfilename << " compress finished" << endl;
}
