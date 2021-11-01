#include "HuffmanTree.hpp"
#include <utility>
#include <string>
#include <iostream>
using std::pair;
using std::string;

int nodes_num=0;
std::map<int, HuffmanTreeNode *> idx2nodes;
HuffmanTreeNode *word2nodes_addr[256] = {};

HuffmanTreeNode::HuffmanTreeNode(int v, HuffmanTreeNode *f, char _lr)
{
    value = v;
    father = f;
    lr = _lr;
    idx = nodes_num;
    idx2nodes.insert(pair<int, HuffmanTreeNode *>(nodes_num++, this));
}
bool HuffmanTreeNode::operator<(const HuffmanTreeNode &x) const
{
    return value < x.value;
}
void HuffmanTreeNode::SetValue(int v)
{
    value = v;
}
void HuffmanTreeNode::SetFather(HuffmanTreeNode *f)
{
    father = f;
}
void HuffmanTreeNode::SetLR(char _lr)
{
    lr = _lr;
}
int HuffmanTreeNode::GetValue()
{
    return value;
}
char HuffmanTreeNode::GetLR()
{
    return lr;
}
string HuffmanTreeNode::GenerateCode()
{
    string ret;
    HuffmanTreeNode *temp = this;
    while(temp->father)
    {
        ret = temp->GetLR() + ret;
        temp = temp->father;
    }
    return ret;
}

bool HuffmanForest_Queue_Cmp::operator()(int a, int b){
    if (idx2nodes.at(a)->GetValue() == idx2nodes.at(b)->GetValue()){
        return idx2nodes.at(a)->idx > idx2nodes.at(b)->idx;
    }
    else return idx2nodes.at(a)->GetValue() > idx2nodes.at(b)->GetValue();
}


HuffmanForest::HuffmanForest()
{
    nodes = *new HuffmanForest_Queue;
}
bool HuffmanForest::is_Tree()
{
    return nodes.size() == 1;
}
void HuffmanForest::AddNode(HuffmanTreeNode *x, char word)
{
    word2nodes_addr[word] = x;
    nodes.push(x->idx);
}
void HuffmanForest::Pop()
{
    nodes.pop();
}
void HuffmanForest::Push(HuffmanTreeNode *x){
    nodes.push(x->idx);
}
HuffmanTreeNode* HuffmanForest::Top(){
    return idx2nodes.at(nodes.top());
}
HuffmanMap_T HuffmanForest::GenerateWord2Code()
{
    HuffmanMap_T temp;
    for (unsigned char i = 0;; ++i)
    {
        if(word2nodes_addr[i]){
            temp.insert(pair<string, string>(uchar2string(i), word2nodes_addr[i]->GenerateCode()));
        }
        if(i==255) break;
    }
    return temp;
}
HuffmanMap_T HuffmanForest::GenerateCode2Word()
{
    HuffmanMap_T temp1 = GenerateWord2Code(), temp2;
    HuffmanMap_T::iterator it;
    for (it = temp1.begin(); it != temp1.end(); ++it)
    {
        temp2.insert(pair<string, string>(it->second, it->first));
    }
    return temp2;
}

HuffmanTreeNode* MergeHuffmanTree(HuffmanTreeNode *x, HuffmanTreeNode *y)
{
    HuffmanTreeNode* ret = new HuffmanTreeNode(x->GetValue() + y->GetValue(),nullptr,0);
    x->SetFather(ret);
    y->SetFather(ret);
    x->SetLR('0');
    y->SetLR('1');
    return ret;
}
void GenerateHuffmanTree(HuffmanForest *x)
{
    while (!x->is_Tree())
    {
        HuffmanTreeNode* temp1 = x->Top();
        x->Pop();
        HuffmanTreeNode* temp2 = x->Top();
        x->Pop();
        x->Push(MergeHuffmanTree(temp1,temp2));
    }
}

string uchar2string(unsigned char x)
{
    string temp;
    while (x)
    {
        temp = std::to_string(x & 1) + temp;
        x >>= 1;
    }
    return temp;
}