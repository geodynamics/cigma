CXX = g++
CXXFLAGS = -Wall
LDFLAGS =

OBJS = \
	Numeric.o

default: $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -f $(OBJS)

.PHONY: clean
