#include "HuffmanTree.hpp"
#include "xqzio.hpp"
#include "xq_debug.hpp"
#include <iostream>
#include <cstring>
#include <windows.h>

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
void compress_file(const char *srcfilename, const char *destfilename, char *relative_addr)
{
    init_externs();
    xqz_read_src(srcfilename);
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
    xqz_write_dest(srcfilename, destfilename, relative_addr);
}

int main(int argc, char **argv)
{
    LARGE_INTEGER m_nFreq;
    LARGE_INTEGER m_nBeginTime;
    LARGE_INTEGER nEndTime;
    QueryPerformanceFrequency(&m_nFreq);    // 获取时钟周期
    QueryPerformanceCounter(&m_nBeginTime); // 获取时钟计数

    const char *target = "./sample/test1";
    char root[200] = {};
    strcpy(root, target);
    char relative_addr[200] = {};
    GetRootPath(root, relative_addr);
    char dest[200] = {};
    strcpy(dest, target);
    strcat(dest, ".xqz");
    printf("root:          %s\nrelative_addr: %s\ndest:          %s\n", root, relative_addr, dest);

    compress_file(target, dest, relative_addr);

    QueryPerformanceCounter(&nEndTime);
    std::cout << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "ms" << std::endl;
    system("pause");
    return 0;
}