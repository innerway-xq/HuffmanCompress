#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <string>
#include <map>
#include <queue>
#include <vector>
#include <functional>

typedef std::map<std::string, std::string> HuffmanMap_T;
extern int nodes_num;

class HuffmanTreeNode
{
private:
    int value = 0;
    HuffmanTreeNode *father = nullptr;
    char lr = 0; // 0-left_son, 1-right_son
public:
    int idx;
    HuffmanTreeNode(int v, HuffmanTreeNode *f, char _lr);
    bool operator<(const HuffmanTreeNode &x) const;
    void SetValue(int v);
    void SetFather(HuffmanTreeNode *f);
    void SetLR(char _lr);
    int GetValue();
    char GetLR();
    std::string GenerateCode();
};

extern std::map<int, HuffmanTreeNode*> idx2nodes;
extern HuffmanTreeNode *word2nodes_addr[256];

struct HuffmanForest_Queue_Cmp
{
    bool operator()(int a, int b);
};
typedef std::priority_queue<int, std::vector<int>, HuffmanForest_Queue_Cmp> HuffmanForest_Queue;

class HuffmanForest
{
private:
    HuffmanForest_Queue nodes;
public:
    HuffmanForest();
    bool is_Tree();
    void AddNode(HuffmanTreeNode *x, char word);
    void Pop();
    HuffmanTreeNode* Top();
    void Push(HuffmanTreeNode *x);
    HuffmanMap_T GenerateWord2Code();
    HuffmanMap_T GenerateCode2Word();
};

HuffmanTreeNode* MergeHuffmanTree(HuffmanTreeNode *x, HuffmanTreeNode *y);
void GenerateHuffmanTree(HuffmanForest *x);
std::string uchar2string(unsigned char x);

#endif