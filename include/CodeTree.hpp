#ifndef CODETREE_H
#define CODETREE_H

#define uc unsigned char

#include <vector>

class CodeTreeNode
{
public:
    bool is_word = false;
    uc word;
    int child[2] = {0,0}; // 0 - left, 1 - right
};
extern std::vector<CodeTreeNode> codetreenodes;
void GenerateCodeTree();

#endif