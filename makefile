OBJS	= main.o md5.o
SOURCE	= main.cpp md5/md5.cpp
HEADER	= helping_function.h semaphores_sheared_mem.hpp md5/md5.h
OUT	= main
CC	 = g++
FLAGS	 = -g -c -Wall
#LFLAGS	 = -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp -std=c++11

md5.o: md5/md5.cpp
	$(CC) $(FLAGS) md5/md5.cpp -std=c++11


clean:
	rm $(OBJS) $(OUT) error.txt
