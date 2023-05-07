# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -std=c++11

default = all

all: serverM serverA serverB client

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c


serverM: serverM.cpp scheduling.o network.o
	$(CC) $(CFLAGS) -o serverM serverM.cpp scheduling.o network.o

serverA: serverA.cpp scheduling.o network.o
	$(CC) $(CFLAGS) -o serverA serverA.cpp scheduling.o network.o

serverB: serverB.cpp scheduling.o network.o
	$(CC) $(CFLAGS) -o serverB serverB.cpp scheduling.o network.o

client: client.cpp scheduling.o network.o
	$(CC) $(CFLAGS) -o client client.cpp scheduling.o network.o


scheduling.o: scheduling.cpp scheduling.hpp
	$(CC) $(CFLAGS) -c scheduling.cpp

network.o: network.cpp network.hpp
	$(CC) $(CFLAGS) -c network.cpp

clean:
	rm -rf *.out *.dSYM *.o serverM serverA serverB client