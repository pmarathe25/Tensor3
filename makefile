BUILDDIR = build/
BINDIR = ~/bin/
TESTDIR = test/
SRCDIR = src/
# Objects
OBJS =
TESTOBJS = $(addprefix $(BUILDDIR)/, test.o)
# Headers
INCLUDEPATH = include
INCLUDE = -I$(INCLUDEPATH)
HEADERS = $(addprefix $(INCLUDEPATH)/, TileMapBase.hpp TileMap.hpp ForwardDeclarations.hpp \
	OpStructs/BinaryOp.hpp OpStructs/UnaryOp.hpp OpStructs/UnaryOp.hpp OpStructs/TileMapView.hpp\
	Ops/BinaryOperations.hpp Ops/InternalOperations.hpp Ops/UnaryOperations.hpp Ops/TileMapViewOperations.hpp Ops/UnaryOperations.hpp)
# Compiler settings
CXX = g++
CFLAGS = -fPIC -c -std=c++17 $(INCLUDE) -O3 -Wpedantic -march=native -flto
LFLAGS = -shared -flto -march=native
TESTLFLAGS = -pthread -flto -march=native
EXECLFLAGS = -flto -march=native

all: $(TESTOBJS)

$(TESTDIR)/test: $(BUILDDIR)/test.o $(HEADERS) $(OBJS)
	$(CXX) $(BUILDDIR)/test.o $(OBJS) $(TESTLFLAGS) -o $(TESTDIR)/test

$(BUILDDIR)/test.o: $(TESTDIR)/test.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/test.cpp -o $(BUILDDIR)/test.o

clean:
	rm $(OBJS) $(TESTOBJS) $(TESTDIR)/test

test: $(TESTDIR)/test
	$(TESTDIR)/test
