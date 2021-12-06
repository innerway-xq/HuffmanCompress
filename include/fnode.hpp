#ifndef FNODE_H
#define FNODE_H

#include<set>
#include<string>
#include<vector>

typedef std::set<std::string> folders_set_t;

class FNode
{
private:
    int type;                 // 目录节点类型: 0-文件夹, 1-文件
    std::string name;              // 文件(夹)名
    std::vector<FNode *> children; // 子目录项(文件/文件夹)
public:
    FNode(std::string N, int T);
    bool addChild(FNode *node);
    void printDirTree(int h);
    FNode* findChild(std::string x);
};

#endif