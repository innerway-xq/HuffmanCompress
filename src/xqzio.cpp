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
std::vector<string> empty_folders;

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
        if (src[i] == '/' || src[i] == '\\')
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
    int i = 0;
    string tmp;
    if ((file_handle = _findfirst(tmp.assign(root).append("/*").c_str(), &fileinfo)))
    {
        do
        {
            ++i;
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
    if (i == 2)
    {
        empty_folders.push_back(tmp.assign(root));
    }
}

void xqz_read_src_compress(const char *filename)
{
    uc buf[MAX_IO_N] = {};
    ifstream fin;
    fin.open(filename, ios::in | ios::binary);
    if (!fin)
    {
        cout << "open " << filename << " failed" << endl;
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

ull compressed_length()
{
    ull ret = 0;
    for (int i = 0; i < 256; ++i)
    {
        ret += word_freq[i] * word2code[(uc)i].size();
    }
    return ret;
}

void Code2WordinFile(std::ofstream &fout)
{
    char temp[10000] = {};
    int l = 0;
    Code2Word_T::iterator it;
    string::iterator i;
    for (it = code2word.begin(); it != code2word.end(); ++it)
    {
        string tmp_str = it->first;
        for (i = (tmp_str).begin(); i != (tmp_str).end(); ++i)
        {
            if (*i)
            {
                temp[l++] = 1;
            }
            else
            {
                temp[l++] = 0;
            }
        }
        temp[l++] = ':';
        temp[l++] = it->second;
        temp[l++] = '\n';
    }
    temp[l++] = '*';
    temp[l++] = '\n';
    fout.write(temp, l);
}

void xqz_write_dest_compress(const char *srcfilename, const char *relative_addr, const char *destfilename)
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
    sprintf(dest_length, "%llu", compressed_length());
    fout.write(dest_length, strlen(dest_length));
    fout.write("\n", 1);

    Code2WordinFile(fout);

    uc buf_in[MAX_IO_N] = {};
    ifstream fin;
    fin.open(srcfilename, ios::in | ios::binary);
    if (!fin)
    {
        cout << "open " << srcfilename << " failed" << endl;
        return;
    }

    int out_l = 0;
    uc buf_char = '\x00';
    int buf_bit = 7;

    fin.seekg(0, ios::end);
    ull length = fin.tellg();
    ull max_i = length / MAX_IO_N;
    ull left_j = length % MAX_IO_N;
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
                        out_l = 0;
                    }
                    buf_out[out_l++] = buf_char;
                    buf_char <<= 8;
                    buf_bit = 7;
                }
                buf_char |= temp[k] << (buf_bit--);
            }
        }
    }

    fin.read((char *)buf_in, left_j);
    for (register int j = 0; j < left_j; ++j)
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
                    out_l = 0;
                }
                buf_out[out_l++] = buf_char;
                buf_char <<= 8;
                buf_bit = 7;
            }
            buf_char |= temp[k] << (buf_bit--);
        }
    }

    if (buf_bit < 7)
    {
        if (out_l >= MAX_IO_N)
        {
            fout.write((const char *)buf_out, out_l);
            memset(buf_out, 0, sizeof(buf_out));
            out_l = 0;
        }
        buf_out[out_l++] = buf_char;
        buf_char <<= 8;
        //此时buf_bit即为压缩文件末尾多的零
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

void read_code2word(ifstream &fin)
{
    char buf_in;
    for (;;)
    {
        string code_tmp = "";
        uc word_tmp;
        fin.read(&buf_in, 1);
        if (buf_in == '*'){
            fin.read(&buf_in, 1);
            return;
        }
        else
            code_tmp += buf_in;
        for (;;)
        {
            fin.read(&buf_in, 1);
            if (buf_in == ':')
                break;
            code_tmp += buf_in;
        }
        fin.read((char *)&word_tmp, 1);
        code2word.insert(std::pair<string, uc>(code_tmp, word_tmp));
        fin.read(&buf_in, 1);
    }
}

void xqz_write_dest_decompress(ifstream &fin, const char *dest, ull src_l){
    char buf_in[MAX_IO_N] = {};
    string buf_string = "";

    char buf_out[MAX_IO_N] = {};
    int out_l = 0;
    ofstream fout;
    fout.open(dest, ios::out | ios::binary);
    if(!fout){
        cout<<"open destfile failed"<<endl;
    }

    ull src_bytes_l = src_l/8 + bool(src_l%8);
    ull max_i,left_j;
    uc left_k;
    left_k = ((uc)(0x80)>>(src_l%8))<<1;
    if (!(src_bytes_l % MAX_IO_N)){
        max_i = src_bytes_l / MAX_IO_N - 1;
        left_j = MAX_IO_N -1;
    }
    else{
        max_i = src_bytes_l / MAX_IO_N;
        left_j = src_bytes_l%MAX_IO_N - 1;
    }

    for(register ull i = 0;i < max_i; ++i){
        fin.read(buf_in,MAX_IO_N);
        for(register int j = 0; j < MAX_IO_N; ++j){
            for(register uc k = 0x80; k ; k>>=1){
                buf_string += bool(buf_in[j]&k);
                if(code2word.count(buf_string)){
                    if(out_l>=MAX_IO_N){
                        fout.write(buf_out,out_l);
                        memset(buf_out,0,sizeof(buf_out));
                        out_l = 0;
                    }
                    buf_out[out_l++] = code2word[buf_string];
                    buf_string = "";
                }
            }
        }
    }

    fin.read(buf_in, left_j);
    for (register int j = 0; j < left_j; ++j)
    {
        for (register uc k = 0x80; k; k >>= 1)
        {
            buf_string += bool(buf_in[j] & k);
            if (code2word.count(buf_string))
            {
                if (out_l >= MAX_IO_N)
                {
                    fout.write(buf_out, out_l);
                    memset(buf_out, 0, sizeof(buf_out));
                    out_l = 0;
                }
                buf_out[out_l++] = code2word[buf_string];
                buf_string = "";
            }
        }
    }
    fin.read(buf_in, 1);
    for (register uc k = 0x80; k >= left_k; k >>= 1)
    {
        buf_string += bool(buf_in[0] & k);
        if (code2word.count(buf_string))
        {
            if (out_l >= MAX_IO_N)
            {
                fout.write(buf_out, out_l);
                memset(buf_out, 0, sizeof(buf_out));
                out_l = 0;
            }
            buf_out[out_l++] = code2word[buf_string];
            buf_string = "";
        }
    }
    if(out_l){
        fout.write(buf_out, out_l);
        memset(buf_out, 0, sizeof(buf_out));
        out_l = 0;
    }
    fout.close();
    cout<<"decompress "<<dest<<" finished"<<endl;
}