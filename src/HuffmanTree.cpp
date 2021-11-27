#include "HuffmanTree.hpp"
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
using std::pair;
using std::string;

int nodes_num = 0;
std::map<int, HuffmanTreeNode *> idx2nodes;
Word2Code_T word2code;
Code2Word_T code2word;
HuffmanTreeNode *word2nodes_addr[256] = {};

HuffmanTreeNode::HuffmanTreeNode()
{
    value = 0;
    father = nullptr;
    lr = 0;
    idx = 0;
}

HuffmanTreeNode::HuffmanTreeNode(int v)
{
    value = v;
    idx = nodes_num;
    idx2nodes.insert(pair<int, HuffmanTreeNode *>(nodes_num++, this));
}

HuffmanTreeNode::HuffmanTreeNode(int v, HuffmanTreeNode *real_addr)
{
    value = v;
    idx = nodes_num;
    idx2nodes.insert(pair<int, HuffmanTreeNode *>(nodes_num++, real_addr));
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
    string ret = "";
    HuffmanTreeNode *temp = this;
    while (temp->father)
    {
        ret = temp->GetLR() + ret;
        temp = temp->father;
    }
    return ret;
}

bool HuffmanForest_Queue_Cmp::operator()(int a, int b)
{
    if (idx2nodes.at(a)->GetValue() == idx2nodes.at(b)->GetValue())
    {
        return idx2nodes.at(a)->idx > idx2nodes.at(b)->idx;
    }
    else
        return idx2nodes.at(a)->GetValue() > idx2nodes.at(b)->GetValue();
}

HuffmanForest::HuffmanForest()
{
    nodes = *new HuffmanForest_Queue;
}

bool HuffmanForest::is_Tree()
{
    return nodes.size() == 1;
}

void HuffmanForest::AddNode(HuffmanTreeNode *x, uc word)
{
    word2nodes_addr[word] = x;
    nodes.push(x->idx);
}

void HuffmanForest::Pop()
{
    nodes.pop();
}

void HuffmanForest::Push(HuffmanTreeNode *x)
{
    nodes.push(x->idx);
}

HuffmanTreeNode *HuffmanForest::Top()
{
    return idx2nodes.at(nodes.top());
}

Word2Code_T HuffmanForest::GenerateWord2Code()
{
    Word2Code_T temp;
    for (register uc i = 0;; ++i)
    {
        if (word2nodes_addr[i])
        {
            temp.insert(pair<uc, string>(i, word2nodes_addr[i]->GenerateCode()));
        }
        if (i == 255)
            break;
    }
    return temp;
}

Code2Word_T HuffmanForest::GenerateCode2Word()
{
    Word2Code_T temp1 = GenerateWord2Code();
    Code2Word_T temp2;
    Word2Code_T::iterator it;
    for (it = temp1.begin(); it != temp1.end(); ++it)
    {
        temp2.insert(pair<string, uc>(it->second, it->first));
    }
    return temp2;
}

HuffmanTreeNode *MergeHuffmanTree(HuffmanTreeNode *x, HuffmanTreeNode *y)
{
    HuffmanTreeNode *ret = new HuffmanTreeNode(x->GetValue() + y->GetValue());
    x->SetFather(ret);
    y->SetFather(ret);
    x->SetLR(0);
    y->SetLR(1);
    return ret;
}

void GenerateHuffmanTree(HuffmanForest *x)
{
    while (!x->is_Tree())
    {
        HuffmanTreeNode *temp1 = x->Top();
        x->Pop();
        HuffmanTreeNode *temp2 = x->Top();
        x->Pop();
        x->Push(MergeHuffmanTree(temp1, temp2));
    }
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
                temp[l++] = '1';
            }
            else
            {
                temp[l++] = '0';
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
