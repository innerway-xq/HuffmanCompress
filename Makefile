VPATH = src:include
objects = main.o HuffmanTree.o xqzio.o fnode.o CodeTree.o
cxxopts = cxxopts.hpp
target = xqz.exe

$(target) : $(objects) $(cxxopts)
	g++ $^ -o $@ -Iinclude
	@move $(target) ./bin
	del $(objects)

$(objects) : %.o : %.cpp
	g++ -c $? -o $@ -Iinclude -O2 -w


.PHONY : clean
clean :
	del $(target) $(objects)