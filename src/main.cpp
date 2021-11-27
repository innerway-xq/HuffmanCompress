#include "HuffmanTree.hpp"
#include "xqzio.hpp"
#include "xq_debug.hpp"
#include <iostream>
#include <cstring>
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
    xqz_read_src(src);
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
    xqz_write_dest(src, relative_addr.c_str(), dest.c_str());
}

int main(int argc, char **argv)
{
    LARGE_INTEGER m_nFreq;
    LARGE_INTEGER m_nBeginTime;
    LARGE_INTEGER nEndTime;
    QueryPerformanceFrequency(&m_nFreq);    // 获取时钟周期
    QueryPerformanceCounter(&m_nBeginTime); // 获取时钟计数

    char *target = (char *)"./sample/test1";

    if (target[strlen(target) - 1] == '/')
        target[strlen(target) - 1] = '\0';

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
        cout << "root:" << root << endl
             << "rela:" << relative_addr << endl
             << "dest:" << dest << endl;

        ofstream fout(dest,ios::app|ios::binary);
        fout.write("1\n",2);
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

        ofstream fout(dest, ios::app | ios::binary);
        fout.write("0\n", 2);
        relative_addr.assign(target);
        relative_addr = "." + relative_addr.substr(GetRootPath(root).size()) + "\n";
        fout.write(relative_addr.c_str(), relative_addr.size());
        fout.close();

        for(string i : files)
        {
            relative_addr.assign(i).erase(0,strlen(target));
            // cout<<i<<" "<<relative_addr<<endl;
            compress_file(i.c_str(), relative_addr, dest);
        }
    }

    QueryPerformanceCounter(&nEndTime);
    std::cout << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "ms" << std::endl;
    sp;
    return 0;
}