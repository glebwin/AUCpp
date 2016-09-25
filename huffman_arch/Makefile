CC = g++
CFLAGS = -std=c++11 -O2

VPATH = src;test
BINDIR = bin

MAIND = bin/main.o bin/huffman.o bin/reader.o bin/writer.o bin/argparser.o
TESTD = bin/test.o bin/autotest.o bin/huffman.o bin/reader.o bin/writer.o bin/argparser.o

all: main

main: $(BINDIR) $(MAIND)
	$(CC) $(CFLAGS) $(MAIND) -o bin/main

test: $(BINDIR) $(TESTD)
	$(CC) $(CFLAGS) $(TESTD) -o bin/test

bin/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: test/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -f bin/*
