CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS =

OBJS = \
	Numeric.o \
	DataIO.o \
	TextIO.o \
	VtkIO.o \
	Cell.o \
	Tet.o \
	Hex.o \
	Field.o \
	MeshPart.o \
	DofHandler.o \


default: $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^

clean:
	rm -f $(OBJS)

.PHONY: clean
