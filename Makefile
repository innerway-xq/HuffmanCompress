VPATH = src
objects = main.o HuffmanTree.o xqzio.o
target = xqz.exe

$(target) : $(objects)
	g++ $^ -o $@ -Iinclude
	del $^

$(objects) : %.o : %.cpp
	g++ -c $? -o $@ -Iinclude -O2

.PHONY : clean
clean :
	del $(target) $(objects)