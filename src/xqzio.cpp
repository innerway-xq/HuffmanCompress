#include "xqzio.hpp"
#include <iostream>
#include <fstream>
#define ull unsigned long long
using std::ifstream;
using std::ios;
using std::cout;
using std::endl;

ull word_freq[256]={};
const int MAX_INPUT_N = 0x1000;

void xqz_in(const char *filename){
    char buf[MAX_INPUT_N] = {};
    ifstream fin;
    fin.open(filename,ios::in|ios::binary);
    if(!fin){
        cout<<"open file failed"<<endl;
        return;
    }
    fin.seekg(0, ios::end);
    ull length = fin.tellg();
    ull max_i = length / MAX_INPUT_N;
    fin.seekg(0, ios::beg);
    for (ull i = 1; i <= max_i; ++i)
    {
        fin.read(buf, MAX_INPUT_N);
        cnt_freq(buf,MAX_INPUT_N);
    }
    fin.read(buf,length%MAX_INPUT_N);
    cnt_freq(buf, length % MAX_INPUT_N);
    cout<<"input finished"<<endl;
}
void cnt_freq(char *x,int l){
    for(ull i=0;i<l;i++){
        ++word_freq[x[i]];
    }
}