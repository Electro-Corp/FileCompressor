CXX = gcc
CXXFLAGS = -g -O0

# The first target is the default one executed when you just type 'make'
all: compress

compress: compress.o mem.o tree.o
	$(CXX) $(CXXFLAGS) -o compress compress.o mem.o tree.o

compress.o: compress.c
	$(CXX) $(CXXFLAGS) -c compress.c

mem.o: mem.c
	$(CXX) $(CXXFLAGS) -c mem.c

tree.o: tree.c
	$(CXX) $(CXXFLAGS) -c tree.c

# A clean target to wipe out binaries and start fresh
clean:
	rm -f *.o compress