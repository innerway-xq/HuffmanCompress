#include "HuffmanTree.hpp"
#include "xqzio.hpp"
#include "xq_debug.hpp"
#include "cxxopts.hpp"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <direct.h>

using namespace std;



void init_externs()
{
    nodes_num = 0;
    idx2nodes.clear();
    word2code.clear();
    code2word.clear();
    memset(word2nodes_addr, 0, sizeof(word2nodes_addr));
    memset(word_freq, 0, sizeof(word_freq));
}

void compress_file(const char *src, string relative_addr, string dest)
{
    if (!GetFileSize(src))
    {
        ofstream fout;
        fout.open(dest, ios::app | ios::binary);
        fout.write(relative_addr.c_str(), relative_addr.size());
        fout.write("\n0\n", 3);
        cout << src << "(empty file) compress finished" << endl;
        return;
    }
    init_externs();
    xqz_read_src_compress(src);
    HuffmanForest *tree = new HuffmanForest;
    HuffmanTreeNode *nodes_array = new HuffmanTreeNode[256];
    for (int i = 0; i < 256; ++i)
    {
        if (word_freq[i])
        {
            nodes_array[i] = HuffmanTreeNode(word_freq[i], nodes_array + i);
            tree->AddNode(nodes_array + i, (uc)i);
        }
    }
    GenerateHuffmanTree(tree);
    cout << "GenerateHuffmanTree finished" << endl;
    word2code = tree->GenerateWord2Code();
    code2word = tree->GenerateCode2Word();
    // testword2code(word2code);
    xqz_write_dest_compress(src, relative_addr.c_str(), dest.c_str());
}

void compress(const char *target)
{
    if (isFile(target))
    {
        string root = "";          //根目录
        string relative_addr = ""; //相对目录
        string dest = "";          //压缩文件路径
        root.assign(target);
        root = GetRootPath(root);
        relative_addr.assign(target);
        relative_addr = "." + relative_addr.substr(root.size());
        dest.assign(target).append(".xqz");
        if (dest_exist(dest))
        {
            cout << "compressed file exists" << endl;
            return;
        }

        ofstream fout(dest, ios::app | ios::binary);
        fout.write("1\n", 2);
        fout.close();

        compress_file(target, relative_addr, dest);
    }
    else
    {
        string root = "";          //根目录
        string relative_addr = ""; //相对目录
        string dest = "";          //压缩文件路径
        root.assign(target);
        GetFolderFiles(root);
        dest.assign(target).append(".xqz");
        if (dest_exist(dest))
        {
            cout << "compressed file exists" << endl;
            return;
        }

        ofstream fout(dest, ios::app | ios::binary);
        fout.write("0\n", 2);
        relative_addr.assign(target);
        relative_addr = "." + relative_addr.substr(GetRootPath(root).size()) + "\n";
        fout.write(relative_addr.c_str(), relative_addr.size());

        if (!empty_folders.empty() && empty_folders[0] == root)
        {
            fout.write("0\n", 2);
        }
        else
        {
            char empty_folder_num[25] = {};
            sprintf(empty_folder_num, "%d\n", empty_folders.size());
            fout.write(empty_folder_num, strlen(empty_folder_num));
            for (string i : empty_folders)
            {
                i = i.substr(GetRootPath(i).size()) + "\n";
                fout.write(i.c_str(), i.size());
            }
        }

        char file_num[25] = {};
        sprintf(file_num, "%d\n", files.size());
        fout.write(file_num, strlen(file_num));
        fout.close();

        for (string i : files)
        {
            relative_addr.assign(i).erase(0, strlen(target));
            // cout<<i<<" "<<relative_addr<<endl;
            compress_file(i.c_str(), relative_addr, dest);
        }
    }
}

void compress(const char *target, const char *dest_directory)
{
    if (isFile(target))
    {
        string root = "";          //根目录
        string relative_addr = ""; //相对目录
        string dest = "";          //压缩文件路径
        root.assign(target);
        root = GetRootPath(root);
        relative_addr.assign(target);
        relative_addr = "." + relative_addr.substr(root.size());
        dest.assign(dest_directory).append(relative_addr.substr(1)).append(".xqz");
        if (dest_exist(dest))
        {
            cout << "compressed file exists" << endl;
            return;
        }

        ofstream fout(dest, ios::app | ios::binary);
        fout.write("1\n", 2);
        fout.close();

        compress_file(target, relative_addr, dest);
    }
    else
    {
        string root = "";          //根目录
        string relative_addr = ""; //相对目录
        string dest = "";          //压缩文件路径
        root.assign(target);
        GetFolderFiles(root);
        relative_addr.assign(target);
        relative_addr = "." + relative_addr.substr(GetRootPath(root).size());
        dest.assign(dest_directory).append(relative_addr.substr(1)).append(".xqz");
        if (dest_exist(dest))
        {
            cout << "compressed file exists" << endl;
            return;
        }

        ofstream fout(dest, ios::app | ios::binary);
        fout.write("0\n", 2);
        relative_addr += "\n";
        fout.write(relative_addr.c_str(), relative_addr.size());

        if (!empty_folders.empty() && empty_folders[0] == root)
        {
            fout.write("0\n", 2);
        }
        else
        {
            char empty_folder_num[25] = {};
            sprintf(empty_folder_num, "%d\n", empty_folders.size());
            fout.write(empty_folder_num, strlen(empty_folder_num));
            for (string i : empty_folders)
            {
                i = i.substr(GetRootPath(i).size()) + "\n";
                fout.write(i.c_str(), i.size());
            }
        }

        char file_num[25] = {};
        sprintf(file_num, "%d\n", files.size());
        fout.write(file_num, strlen(file_num));
        fout.close();

        for (string i : files)
        {
            relative_addr.assign(i).erase(0, strlen(target));
            // cout<<i<<" "<<relative_addr<<endl;
            compress_file(i.c_str(), relative_addr, dest);
        }
    }
}

void decompress_file(ifstream &fin, const char *dest)
{
    init_externs();
    cout << "decompressing " << dest << endl;
    string src_l_tmp;
    ull src_l;
    getline(fin, src_l_tmp);
    src_l = strtoull(src_l_tmp.c_str(), nullptr, 10);
    if (!src_l)
    {
        ofstream fout(dest, ios::out | ios::binary);
        fout.close();
        cout << "decompress " << dest << "(empty file) finished" << endl;
    }
    else
    {
        read_code2word(fin);
        cout << "read code2word finished" << endl;
        xqz_write_dest_decompress(fin, dest, src_l);
    }
}

void decompress(const char *target)
{
    string buf_in;
    ifstream fin;
    fin.open(target, ios::in | ios::binary);
    if (!fin)
    {
        cout << "open " << target << " failed" << endl;
    }

    getline(fin, buf_in);
    if (buf_in == "1")
    {
        getline(fin, buf_in);
        if (dest_exist(buf_in.c_str()))
        {
            cout << "decompressed file exists" << endl;
            return;
        }
        decompress_file(fin, buf_in.c_str());
        fin.close();
    }
    else
    {
        string root = "";
        string relative_addr = "";
        string dest = "";
        int empty_folder_num, file_num;

        getline(fin, root);
        if (dest_exist(root))
        {
            cout << "decompressed folder exists" << endl;
            return;
        }
        my_mkdir(root.c_str());
        getline(fin, buf_in);
        empty_folder_num = strtol(buf_in.c_str(), nullptr, 10);
        for (int i = 0; i < empty_folder_num; ++i)
        {
            getline(fin, relative_addr);
            dest = root + relative_addr;
            my_mkdir(dest.c_str());
        }
        cout<<"decompress empty_folder finished"<<endl;

        getline(fin, buf_in);
        file_num = strtol(buf_in.c_str(), nullptr, 10);
        for (int i = 0; i < file_num; ++i)
        {
            getline(fin, relative_addr);
            dest = root + relative_addr;
            my_mkdir(GetRootPath(dest).c_str());
            decompress_file(fin, dest.c_str());
        }
        fin.close();
    }
}

void decompress(const char *target, const char* dest_directory)
{
    string buf_in;
    ifstream fin;
    fin.open(target, ios::in | ios::binary);
    if (!fin)
    {
        cout << "open " << target << " failed" << endl;
    }

    getline(fin, buf_in);
    if (buf_in == "1")
    {    
        string dest = "";
        getline(fin, buf_in);
        dest = dest_directory + buf_in.substr(1);
        if (dest_exist(dest.c_str()))
        {
            cout << "decompressed file exists" << endl;
            return;
        }
        decompress_file(fin, dest.c_str());
        fin.close();
    }
    else
    {
        string root = "";
        string relative_addr = "";
        string dest = "";
        int empty_folder_num, file_num;

        getline(fin, root);
        root = dest_directory + root.substr(1);
        if (dest_exist(root))
        {
            cout << "decompressed folder exists" << endl;
            return;
        }
        my_mkdir(root.c_str());
        getline(fin, buf_in);
        empty_folder_num = strtol(buf_in.c_str(), nullptr, 10);
        for (int i = 0; i < empty_folder_num; ++i)
        {
            getline(fin, relative_addr);
            dest = root + relative_addr;
            my_mkdir(dest.c_str());
        }
        cout << "decompress empty_folder finished" << endl;

        getline(fin, buf_in);
        file_num = strtol(buf_in.c_str(), nullptr, 10);
        for (int i = 0; i < file_num; ++i)
        {
            getline(fin, relative_addr);
            dest = root + relative_addr;
            my_mkdir(GetRootPath(dest).c_str());
            decompress_file(fin, dest.c_str());
        }
        fin.close();
    }
}

cxxopts::Options options("xqz", "");
void init_options()
{
    options
        .positional_help("[optional args]")
        .show_positional_help();
    options
        .set_width(70)
        .set_tab_expansion()
        .allow_unrecognised_options()
        .add_options()
        ("c,compress", "compress something", cxxopts::value<vector<string>>(),"<file/folder>")
        ("n,name","set output name",cxxopts::value<string>(),"<name>")
        ("x,extract", "extract .xqz file", cxxopts::value<string>(), "<file>")
        ("t,to", "place the output into <directory>", cxxopts::value<string>(), "<directory>")
        ("s,show", "show files", cxxopts::value<string>(), "<file>")
        ("h,help", "print help")
        ;
}

void parse_path(char *x)
{
    if (x[strlen(x) - 1] == '/' or x[strlen(x) - 1] == '\\')
        x[strlen(x) - 1] = '\0';
    for(int i = 0; i < strlen(x); ++i){
        if(x[i] == '/'){
            x[i] = '\\';
        }
    }
    return;
}

int main(int argc, char **argv)
{
    LARGE_INTEGER m_nFreq;
    LARGE_INTEGER m_nBeginTime;
    LARGE_INTEGER nEndTime;
    QueryPerformanceFrequency(&m_nFreq);    // 获取时钟周期
    QueryPerformanceCounter(&m_nBeginTime); // 获取时钟计数

    //设置光标不显示
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible = FALSE;
    cci.dwSize = sizeof(cci);
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(handle, &cci);

    try
    {
        init_options();
        auto result = options.parse(argc, argv);
        if (result.count("h"))
        {
            cout << options.help() << endl;
            return 0;
        }
        if (result.count("c"))
        {
            vector<string> compress_src = result["c"].as<vector<string>>();
            if (result.count("t"))
            {
                char *dest_directory = (char *)result["t"].as<string>().c_str();
                parse_path(dest_directory);
                if (compress_src.size() == 1)
                {
                    char *target = (char *)compress_src[0].c_str();
                    parse_path(target);
                    compress(target, dest_directory);
                }
                else{     
                    char code[100]={};
                    char tmp[100] = {};
                    if (result.count("n"))
                    {
                        strcpy(tmp, (char *)(".\\" + result["n"].as<string>()).c_str());
                    }
                    else
                        strcpy(tmp, ".\\tmp");
                    mkdir(tmp);
                    for(string i : compress_src){
                        char* target = (char*)i.c_str();
                        parse_path(target);
                        sprintf(code, "copy %s %s",target,tmp);
                        system(code);
                    }
                    compress(tmp,dest_directory);
                    sprintf(code, "rmdir /Q/S %s", tmp);
                    system(code);
                }
            }
            else
            {
                if (compress_src.size() == 1)
                {
                    char *target = (char *)compress_src[0].c_str();
                    parse_path(target);
                    compress(target);
                }
                else
                {
                    char code[100] = {};
                    char tmp[100] = {};
                    if (result.count("n"))
                    {
                        strcpy(tmp,(char *)(".\\" + result["n"].as<string>()).c_str());
                    }
                    else
                        strcpy(tmp,".\\tmp");
                    mkdir(tmp);
                    for (string i : compress_src)
                    {
                        char *target = (char *)i.c_str();
                        parse_path(target);
                        sprintf(code, "copy %s %s", target, tmp);
                        system(code);
                    }
                    compress(tmp);
                    sprintf(code, "rmdir /Q/S %s", tmp);
                    system(code);
                }
            }
        }
        if (result.count("x")){
            string decompress_src = result["x"].as<string>();
            if(result.count("t")){
                char *dest_directory = (char *)result["t"].as<string>().c_str();
                parse_path(dest_directory);
                char *target = (char *)decompress_src.c_str();
                parse_path(target);
                decompress(target,dest_directory);
            }
            else
            {
                char *target = (char *)decompress_src.c_str();
                parse_path(target);
                decompress(target);
            }
        }

    }
    catch (const cxxopts::OptionException &e)
    {
        cout << "error parsing options: " << e.what() << endl;
        return 1;
    }


    QueryPerformanceCounter(&nEndTime);
    std::cout << (double)(nEndTime.QuadPart - m_nBeginTime.QuadPart) * 1000 / m_nFreq.QuadPart << "ms" << std::endl;
    return 0;
}