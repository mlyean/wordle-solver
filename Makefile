CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -Ofast -march=native
LDFLAGS=-ltbb

SRCS=${wildcard src/*.cpp}
OBJS=${patsubst src/%.cpp, build/%.o, ${SRCS}}

all: bin/main

bin/:
	mkdir -p $@

build/:
	mkdir -p $@

bin/main: ${OBJS} | bin/
	${CXX} ${LDFLAGS} -o $@ $^

build/%.o: src/%.cpp | build/
	${CXX} ${CXXFLAGS} -c -o $@ $<

.PHONY: clean

clean:
	rm -f ${OBJS}
	rm -f bin/*
