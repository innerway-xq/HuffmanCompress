#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <string>
#include <map>
#include <queue>
#include <vector>
#include <functional>

#define uc unsigned char

typedef std::map<uc, std::string> Word2Code_T;
typedef std::map<std::string, uc> Code2Word_T;

extern int nodes_num;
extern Word2Code_T word2code;
extern Code2Word_T code2word;

class HuffmanTreeNode
{
private:
    int value = 0;
    HuffmanTreeNode *father = nullptr;
    char lr = 0; // 0-left_son, 1-right_son
public:
    int idx = 0;
    HuffmanTreeNode();
    HuffmanTreeNode(int v);
    HuffmanTreeNode(int v, HuffmanTreeNode *real_addr);
    bool operator<(const HuffmanTreeNode &x) const;
    void SetValue(int v);
    void SetFather(HuffmanTreeNode *f);
    void SetLR(char _lr);
    int GetValue();
    char GetLR();
    // GenerateCode - 从此节点向上生成01序列，仅用于叶节点
    std::string GenerateCode(); 
};

extern std::map<int, HuffmanTreeNode *> idx2nodes;
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
    // is_Tree - 是否是一棵树，即是否生成完毕
    bool is_Tree();
    // AddNode - 用于添加初始节点（叶节点）
    void AddNode(HuffmanTreeNode *x, uc word);
    void Pop();
    HuffmanTreeNode *Top();
    void Push(HuffmanTreeNode *x);
    Word2Code_T GenerateWord2Code();
    Code2Word_T GenerateCode2Word();
};

// MergeHuffmanTree - 合并两个节点
HuffmanTreeNode *MergeHuffmanTree(HuffmanTreeNode *x, HuffmanTreeNode *y);
// GenerateHuffmanTree - 生成哈夫曼树
void GenerateHuffmanTree(HuffmanForest *x);


#endif