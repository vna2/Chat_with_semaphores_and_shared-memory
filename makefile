OBJS	= chan.o md5.o enc.o p.o enc2.o p2.o
OUT	= chan enc p enc2 p2

OBJS0	= chan.o md5.o
SOURCE0	= chan.cpp md5/md5.cpp
HEADER0	= helping_function.h md5/md5.h
OUT0	= chan

OBJS1	= enc.o md5.o
SOURCE1	= enc.cpp md5/md5.cpp
HEADER1	= helping_function.h md5/md5.h
OUT1	= enc

OBJS2	= p.o md5.o
SOURCE2	= p.cpp md5/md5.cpp
HEADER2	= helping_function.h md5/md5.h
OUT2	= p

OBJS3	= enc2.o md5.o
SOURCE3	= enc2.cpp md5/md5.cpp
HEADER3	= helping_function.h md5/md5.h
OUT3	= enc2

OBJS4	= p2.o md5.o
SOURCE4	= p2.cpp md5/md5.cpp
HEADER4	= helping_function.h md5/md5.h
OUT4	= p2

CC	 = g++
FLAGS	 = -g -c -Wall -g3
LFLAGS	 =

all: chan enc p enc2 p2

chan: $(OBJS0) $(LFLAGS)
	$(CC) -g $(OBJS0) -o $(OUT0)

enc: $(OBJS1) $(LFLAGS)
	$(CC) -g $(OBJS1) -o $(OUT1)

p: $(OBJS2) $(LFLAGS)
	$(CC) -g $(OBJS2) -o $(OUT2)

enc2: $(OBJS3) $(LFLAGS)
	$(CC) -g $(OBJS3) -o $(OUT3)

p2: $(OBJS4) $(LFLAGS)
	$(CC) -g $(OBJS4) -o $(OUT4)


chan.o: chan.cpp
	$(CC) $(FLAGS) chan.cpp -std=c++11

md5.o: md5/md5.cpp
	$(CC) $(FLAGS) md5/md5.cpp -std=c++11

enc.o: enc.cpp
	$(CC) $(FLAGS) enc.cpp -std=c++11

enc2.o: enc2.cpp
	$(CC) $(FLAGS) enc2.cpp -std=c++11

p.o: p.cpp
	$(CC) $(FLAGS) p.cpp -std=c++11

p2.o: p2.cpp
	$(CC) $(FLAGS) p2.cpp -std=c++11

clean:
	rm -f $(OBJS) $(OUT)
