VPATH = src
objects = main.o HuffmanTree.o xqzio.o
target = HuffmanCompress.exe

$(target) : $(objects)
	g++ $^ -o $@ -Iinclude
	del $^

$(objects) : %.o : %.cpp
	g++ -c $? -o $@ -Iinclude

.PHONY : clean
clean :
	del $(target) $(objects)