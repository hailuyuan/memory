CC = gcc                                                 #编译器变量
#STATIC_LIB := libxxx.a                                  #静态库
#LINK_NAME := libxxx.so                                  #动态库
LIB_C_SOURCES := $(filter-out main.c, $(wildcard *.c))   #所有.C文件，过滤掉main.c文件
LIB_SOURCES   := $(patsubst %.c, %.o, $(LIB_C_SOURCES))  #所有目标文件  

CFLAGS = -I ./ -g -m32
	
%.o: %.c
	$(CC) -fPIC $(CFLAGS) -c -o $@ $^

test: main.o $(LIB_SOURCES) 
	$(CC) -o $@ $^ -m32
main.o: main.c
	$(CC)  $(CFLAGS) -c -o  $@ $^
clean:
	 -rm -f *.o test*
