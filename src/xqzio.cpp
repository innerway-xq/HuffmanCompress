#include "xqzio.hpp"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::ios;

ull word_freq[256] = {};
const int MAX_INPUT_N = 0x1000;

void xqz_in(const char *filename)
{
    uc buf[MAX_INPUT_N] = {};
    ifstream fin;
    fin.open(filename, ios::in | ios::binary);
    if (!fin)
    {
        cout << "open file failed" << endl;
        return;
    }
    fin.seekg(0, ios::end);
    ull length = fin.tellg();
    ull max_i = length / MAX_INPUT_N;
    fin.seekg(0, ios::beg);
    for (ull i = 1; i <= max_i; ++i)
    {
        fin.read((char *)buf, MAX_INPUT_N);
        cnt_freq(buf, MAX_INPUT_N);
    }
    fin.read((char *)buf, length % MAX_INPUT_N);
    cnt_freq(buf, length % MAX_INPUT_N);
    cout << "input finished" << endl;
}
void cnt_freq(uc *x, int l)
{
    for (ull i = 0; i < l; i++)
    {
        ++word_freq[x[i]];
    }
}