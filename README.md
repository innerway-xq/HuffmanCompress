# HuffmanCompress

- [x] 判断文件夹还是文件
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
- [ ] 获取文件夹下文件目录

  > https://blog.csdn.net/leo_888/article/details/80681184
  >
  > https://blog.csdn.net/sagghab/article/details/81436515 - \_finddata_t

  > access( )函数：用于文件读取，判断文件是否存在，并判断文件是否可写。Linux中，函数为access();标准C++中，该函数为_access，两者的使用方法基本相同
  >
  > access(const char *pathname, int mode);//位于<unistd.h>中
  >
  > int _access(const char *pathname, int mode);//位于<io.h>中
  >
  > pathname 为文件路径或目录路径 mode 为访问权限
  >
  > 如果文件具有指定的访问权限，则函数返回0；如果文件不存在或者不能访问指定的权限，则返回-1.
  >
  > mode的值和含义如下所示：
  >
  > 00——只检查文件是否存在
  >
  > 02——写权限
  >
  > 04——读权限
  >
  > 06——读写权限

  > mkdir() in direct.h

- [x] destfile 记下 编码后长度

- [ ] 检测是否已经有压缩好的文件

- [ ] 隐藏文件

- [ ] 逐行读取

  ```cpp
  ifstream fin;
  fin.open(target, ios::in|ios::binary);
  for(;;){
      string tmp;
      getline(fin,tmp);
      cout<<tmp;
      sp;
  }
  ```

- [ ] 空文件夹处理

- [ ] 注释





## 全局变量

+ int nodes_num
+ std::map<int, HuffmanTreeNode *> idx2nodes
+ Word2Code_T word2code
+ Code2Word_T code2word
+ HuffmanTreeNode *word2nodes_addr[256];
+ ull word_freq[256];
+ const int MAX_IO_N;
+ vector\<string> files
