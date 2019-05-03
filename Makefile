all: huff

SOURCES = main.cpp node.cpp bits.cpp hoff.cpp ahoff.cpp model.cpp


huff:
	g++ -std=c++11 -g -pedantic -Wextra -Wall -o huff_codec $(SOURCES)

clean:
	rm -rf *.o
	rm -rf huff_codec
