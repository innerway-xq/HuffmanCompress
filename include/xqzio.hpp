#ifndef XQZIO_H
#define XQZIO_H

#define ull unsigned long long
#define uc unsigned char
extern unsigned long long word_freq[256];
extern const int MAX_INPUT_N;

void xqz_in(const char *filename);
void cnt_freq(uc *x, int l);

#endif