SOURCES = $(shell ls *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

CXX = g++
CXXFLAGS = -Wall -ansi -pedantic -fPIC -O3

AR = ar
ARFLAGS = rc

ARLIB = libsmppparser.a
SHLIB = libsmppparser.so

.PHONY: all
all: $(ARLIB) $(SHLIB)

$(ARLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

$(SHLIB): $(OBJECTS)
	$(CXX) -shared -o $@ $(CCCOPTS) $(OBJECTS) -fPIC

.PHONY: clean
clean:
	rm -f core *.o *.d *~ $(ARLIB) $(SHLIB)
