include ../makefile.defines
BUILDDIR = build/
BINDIR = ~/bin/
TESTDIR = test/
SRCDIR = src/
# Objects
OBJS =
TESTOBJS = $(call generate_obj_names,$(TESTDIR),$(BUILDDIR))
# Headers
INCLUDEPATH = include/
INCLUDE = -I$(INCLUDEPATH)
HEADERS = $(call find_headers,$(INCLUDEPATH))
# Compiler settings
CXX = g++
CFLAGS = -fPIC -c -std=c++17 -flto -O3 -march=native -Wpedantic -fopenmp
LFLAGS = -shared -flto -O3 -march=native -Wpedantic -fopenmp
TESTLFLAGS = -flto -O3 -march=native -Wpedantic -fopenmp
EXECLFLAGS = -flto -O3 -march=native -Wpedantic -fopenmp

.PHONY: clean test install uninstall
all: $(TESTOBJS)

$(TESTDIR)/test: $(BUILDDIR)/test.o $(HEADERS) $(OBJS)
	$(CXX) $(BUILDDIR)/test.o $(OBJS) $(TESTLFLAGS) -o $(TESTDIR)/test

$(BUILDDIR)/test.o: $(TESTDIR)/test.cpp $(HEADERS)
	$(CXX) $(CFLAGS) $(TESTDIR)/test.cpp -o $(BUILDDIR)/test.o

clean:
	-rm $(OBJS) $(TESTOBJS) $(TESTDIR)/test

test: $(TESTDIR)/test
	$(TESTDIR)/test

install:
	$(call install_headers,$(INCLUDEPATH),Tensor3,$(HEADERS))

uninstall:
	$(call uninstall_headers,Tensor3)
