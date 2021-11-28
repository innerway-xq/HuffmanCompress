#include "HuffmanTree.hpp"
#include "xqzio.hpp"
#include "xq_debug.hpp"
#include "cxxopts.hpp"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <direct.h>

using namespace std;

void init_externs()
{
    nodes_num = 0;
    idx2nodes.clear();
    word2code.clear();
    code2word.clear();
    memset(word2nodes_addr, 0, sizeof(word2nodes_addr));
    memset(word_freq, 0, sizeof(word_freq));
}

bool dest_exist(string dest)
{
    int x = access(dest.c_str(), 0);
    if (x == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void compress_file(const char *src, string relative_addr, string dest)
{
    if (!GetFileSize(src))
    {
        ofstream fout;
        fout.open(dest, ios::app | ios::binary);
        fout.write(relative_addr.c_str(), relative_addr.size());
        fout.write("\n0\n", 3);
        cout << src << "(empty file) compress finished" << endl;
        return;
    }
    init_externs();
    xqz_read_src_compress(src);
    HuffmanForest *tree = new HuffmanForest;
    HuffmanTreeNode *nodes_array = new HuffmanTreeNode[256];
    for (int i = 0; i < 256; ++i)
    {
        if (word_freq[i])
        {
            nodes_array[i] = HuffmanTreeNode(word_freq[i], nodes_array + i);
            tree->AddNode(nodes_array + i, (uc)i);
        }
    }
    GenerateHuffmanTree(tree);
    cout << "GenerateHuffmanTree finished" << endl;
    word2code = tree->GenerateWord2Code();
    code2word = tree->GenerateCode2Word();
    // testword2code(word2code);
    xqz_write_dest_compress(src, relative_addr.c_str(), dest.c_str());
}

void compress(const char *target)
{
    if (isFile(target))
    {
        string root = "";          //根目录
        string relative_addr = ""; //相对目录
        string dest = "";          //压缩文件路径
        root.assign(target);
        root = GetRootPath(root);
        relative_addr.assign(target);
        relative_addr = "." + relative_addr.substr(root.size());
        dest.assign(target).append(".xqz");
        if (dest_exist(dest))
        {
            cout << "compressed file exists" << endl;
            return;
        }

        ofstream fout(dest, ios::app | ios::binary);
        fout.write("1\n", 2);
        fout.close();

        compress_file(target, relative_addr, dest);
    }
    else
    {
        string root = "";          //根目录
        string relative_addr = ""; //相对目录
        string dest = "";          //压缩文件路径
        root.assign(target);
        GetFolderFiles(root);
        dest.assign(target).append(".xqz");
        if (dest_exist(dest))
        {
            cout << "compressed file exists" << endl;
            return;
        }

        ofstream fout(dest, ios::app | ios::binary);
        fout.write("0\n", 2);
        relative_addr.assign(target);
        relative_addr = "." + relative_addr.substr(GetRootPath(root).size()) + "\n";
        fout.write(relative_addr.c_str(), relative_addr.size());

        if (!empty_folders.empty() && empty_folders[0] == root)
        {
            fout.write("0\n", 2);
        }
        else
        {
            char empty_folder_num[25] = {};
            sprintf(empty_folder_num, "%d\n", empty_folders.size());
            fout.write(empty_folder_num, strlen(empty_folder_num));
            for (string i : empty_folders)
            {
                i = i.substr(GetRootPath(i).size()) + "\n";
                fout.write(i.c_str(), i.size());
            }
        }

        char file_num[25] = {};
        sprintf(file_num, "%d\n", files.size());
        fout.write(file_num, strlen(file_num));
        fout.close();

        for (string i : files)
        {
            relative_addr.assign(i).erase(0, strlen(target));
            // cout<<i<<" "<<relative_addr<<endl;
            compress_file(i.c_str(), relative_addr, dest);
        }
    }
}

void decompress_file(ifstream &fin, const char *dest)
{
    init_externs();
    cout << "decompressing " << dest << endl;
    string src_l_tmp;
    ull src_l;
    getline(fin, src_l_tmp);
    src_l = strtoull(src_l_tmp.c_str(), nullptr, 10);
    if(!src_l){
        ofstream fout(dest,ios::out|ios::binary);
        fout.close();
        cout<<"decompress "<<dest<<"(empty file) finished"<<endl;
    }
    else
    {
        read_code2word(fin);
        cout<<"read code2word finished"<<endl;
        xqz_write_dest_decompress(fin, dest, src_l);
    }
}

void decompress(const char *target)
{
    string buf_in;
    ifstream fin;
    fin.open(target, ios::in | ios::binary);
    if (!fin)
    {
        cout << "open " << target << " failed" << endl;
    }

    getline(fin, buf_in);
    if (buf_in == "1")
    {
        getline(fin, buf_in);
        if (dest_exist(buf_in.c_str()))
        {
            cout << "decompressed file exists" << endl;
            return;
        }
        decompress_file(fin, buf_in.c_str());
        fin.close();
    }
    else
    {
        string root = "";
        string relative_addr = "";
        string dest = "";
        int empty_folder_num, file_num;

        getline(fin, root);
        if(dest_exist(root)){
            cout << "decompressed folder exists" << endl;
            return;
        }
        my_mkdir(root.c_str());
        getline(fin, buf_in);
        empty_folder_num = strtol(buf_in.c_str(), nullptr, 10);
        for (int i = 0; i < empty_folder_num; ++i)
        {
            getline(fin, relative_addr);
            dest = root + relative_addr;
            my_mkdir(dest.c_str());
        }

        getline(fin, buf_in);
        file_num = strtol(buf_in.c_str(), nullptr, 10);
        for (int i = 0; i < file_num; ++i)
        {
            getline(fin, relative_addr);
            dest = root + relative_addr;
            my_mkdir(GetRootPath(dest).c_str());
            decompress_file(fin, dest.c_str());
        }
        fin.close();
    }
}

int main(int argc, char **argv)
{
    LARGE_INTEGER m_nFreq;
    LARGE_INTEGER m_nBeginTime;
    LARGE_INTEGER nEndTime;
    QueryPerformanceFrequency(&m_nFreq);    // 获取时钟周期
    QueryPerformanceCounter(&m_nBeginTime); // 获取时钟计数

    char *target = (char *)"./sample/3.xqz";

    if (target[strlen(target) - 1] == '/' or target[strlen(target) - 1] == '\\')
        target[strlen(target) - 1] = '\0';

    decompress(target);

    QueryPerformanceCounter(&nEndTime);
    std::cout << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "ms" << std::endl;
    sp;
    return 0;
}