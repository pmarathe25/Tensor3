BUILDDIR = build/
BINDIR = ~/bin/
TESTDIR = test/
SRCDIR = src/
# Objects
OBJS =
TESTOBJS = $(addprefix $(BUILDDIR)/, test.o)
# Headers
INCLUDEPATH = include/
INCLUDE = -I$(INCLUDEPATH)
HEADERS = $(addprefix $(INCLUDEPATH)/, TileMapBase.hpp TileMap.hpp ForwardDeclarations.hpp utils.hpp \
	Expressions/ElemWiseBinaryExpr.hpp Expressions/UnaryExpr.hpp Expressions/BlockExpr.hpp Expressions/TileMapRandomGenerator.hpp \
	Operations/BinaryOperations.hpp Operations/UnaryOperations.hpp Operations/BlockOperations.hpp \
	Functors/BinaryFunctors.hpp Functors/UnaryFunctors.hpp)
# Compiler settings
CXX = g++
CFLAGS = -fPIC -c -std=c++17 $(INCLUDE) -flto -O3 -march=native -Wpedantic
LFLAGS = -shared -flto -O3 -march=native -Wpedantic
TESTLFLAGS = -flto -O3 -march=native -Wpedantic -lstealthbenchmark
EXECLFLAGS = -flto -O3 -march=native -Wpedantic

all: $(TESTOBJS)

$(TESTDIR)/test: $(BUILDDIR)/test.o $(HEADERS) $(OBJS)
	$(CXX) $(BUILDDIR)/test.o $(OBJS) $(TESTLFLAGS) -o $(TESTDIR)/test

$(BUILDDIR)/test.o: $(TESTDIR)/test.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/test.cpp -o $(BUILDDIR)/test.o

.PHONY: clean
clean:
	rm $(OBJS) $(TESTOBJS) $(TESTDIR)/test

.PHONY: test
test: $(TESTDIR)/test
	$(TESTDIR)/test

.PHONY: install
install:
	sudo ln -snf $(CURDIR)/$(INCLUDEPATH) /usr/local/include/TileMap

.PHONY: uninstall
uninstall:
	sudo rm /usr/local/include/TileMap
