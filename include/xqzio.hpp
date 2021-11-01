#ifndef XQZIO_H
#define XQZIO_H

extern unsigned long long word_freq[256];
extern const int MAX_INPUT_N;

void xqz_in(const char *filename);
void cnt_freq(char* x,int l);

#endif