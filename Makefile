VPATH = src:include
objects = main.o HuffmanTree.o xqzio.o
cxxopts = cxxopts.hpp
target = xqz.exe

$(target) : $(objects) $(cxxopts)
	g++ $^ -o $@ -Iinclude
	del $(objects)

$(objects) : %.o : %.cpp
	g++ -c $? -o $@ -Iinclude -O2 -w


.PHONY : clean
clean :
	del $(target) $(objects)