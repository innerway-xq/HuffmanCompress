# HuffmanCompress

## 判断文件夹还是文件
```cpp
const char* filename = "./test.cpp";
struct stat buf;
stat(filename, &buf);
if (_S_IFDIR & buf.st_mode) {
    printf("folder\n");
}
else if (_S_IFREG & buf.st_mode) {
    printf("file\n");
}
```
## 获取文件绝对路径
##### 例如："C:/Users/innerway/Desktop/test/test.cpp"
1. 取出根目录 "C:/Users/innerway/Desktop/test"
2. 文件名存入压缩文件 "./test.cpp"

## 获取文件夹下文件目录



## 全局变量

+ int nodes_num
+ std::map<int, HuffmanTreeNode *> idx2nodes
+ Word2Code_T word2code
+ Code2Word_T code2word
+ HuffmanTreeNode *word2nodes_addr[256];
+ ull word_freq[256];
+ const int MAX_IO_N;
